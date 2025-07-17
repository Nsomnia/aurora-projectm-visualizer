// src/core.cpp
#include "core.h"
#include "utils/common.h"
#include "Gui.h"
#include "VideoExporter.h"
#include "utils/Logger.h"
#include <chrono>
#include <SDL_mixer.h>
#include <GL/glew.h>
#include <unistd.h>
#include <limits.h>

#include <csignal>

// Global flag to signal termination
extern volatile sig_atomic_t g_quit_flag;

void signal_handler(int signal) {
    g_quit_flag = 1;
}

Core::Core(Config& config)
    : _config(config),
      _window(nullptr),
      _context(nullptr),
      _pM(nullptr),
      _renderer(),
      _event_handler(_config, _preset_manager, _animation_manager, _text_renderer, _text_manager),
      _audio_input(_config),
      _preset_manager(_config),
      _text_renderer(),
      _text_manager(_text_renderer),
      _animation_manager(_config, _text_renderer),
      _video_exporter(_config),
      _gui(std::make_unique<Gui>(_config, *this)),
      g_quit(false) {}

Core::~Core() {
    cleanup();
}

bool Core::init() {
    SDL_SetHint(SDL_HINT_AUDIO_INCLUDE_MONITORS, "1");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Error: SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    _window = SDL_CreateWindow("Aurora Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               _config.width, _config.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!_window) {
        std::cerr << "Error: SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    _context = SDL_GL_CreateContext(_window);
    if (!_context) {
        std::cerr << "Error: SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: glewInit failed." << std::endl;
        return false;
    }

    if (!_renderer.init(_window, &_context, _config)) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    if (!_text_renderer.init(_config.font_path, _config.songInfoFontSize)) {
        std::cerr << "Failed to load font: " << _config.font_path << std::endl;
    }
    _text_renderer.setProjection(_config.width, _config.height);

    _pM = projectm_create();
    projectm_set_window_size(_pM, _config.width, _config.height);
    projectm_set_mesh_size(_pM, 64, 48);
    projectm_set_soft_cut_duration(_pM, _config.presetBlendTime);

    if (!_config.use_default_projectm_visualizer) {
        _preset_manager.load_presets();
    }

    _audio_input.set_projectm_handle(_pM);

    if (!_audio_input.init()) {
        Logger::error("Failed to initialize audio input");
        return false;
    }

    if (!_gui->init(_window, _context)) {
        std::cerr << "Failed to initialize GUI" << std::endl;
        return false;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    return true;
}

void Core::run() {
    int current_audio_index = 0;
    double time_since_last_shuffle = 0.0;
    std::string currentPreset;
    if (!_config.use_default_projectm_visualizer) {
        currentPreset = _preset_manager.get_next_preset();
        if (!currentPreset.empty()) {
            projectm_load_preset_file(_pM, currentPreset.c_str(), true);
        }
    }

    if (_config.enable_recording) {
        _video_exporter.start_export(_config.width, _config.height);
    }

    const Uint32 frame_duration_ms = 1000 / _config.fps;

    while (!g_quit && static_cast<size_t>(current_audio_index) < _config.audio_file_paths.size()) {
        const std::string& current_audio_file = _config.audio_file_paths[current_audio_index];
        _audio_input.load_and_play_music(current_audio_file);
        SDL_Delay(100); // Add a small delay to ensure the music is loaded

        _config.songTitle = sanitize_filename(current_audio_file);
        std::vector<std::string> titleLines = _text_manager.split_text(_config.songTitle, _config.width, 1.0f);

        _animation_manager.reset(titleLines);

        auto last_frame_time = std::chrono::high_resolution_clock::now();

        double audio_duration_seconds = _audio_input.get_audio_duration();
        int target_frames = static_cast<int>(std::ceil(audio_duration_seconds * _config.video_framerate));
        int frame_count = 0;

        while (!g_quit && !g_quit_flag && frame_count < target_frames) {
            Uint32 frame_start_ticks = SDL_GetTicks();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto current_frame_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> delta_time = current_frame_time - last_frame_time;
            last_frame_time = current_frame_time;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                _gui->handle_event(event);

                _event_handler.handle_event(event, g_quit, current_audio_index, time_since_last_shuffle, currentPreset, _pM, titleLines);
            }

            if (_config.shuffleEnabled && !_config.use_default_projectm_visualizer) {
                time_since_last_shuffle += delta_time.count();
                if (time_since_last_shuffle >= _config.presetDuration) {
                    currentPreset = _preset_manager.get_next_preset();
                    if (!currentPreset.empty()) {
                        projectm_load_preset_file(_pM, currentPreset.c_str(), true);
                    }
                    time_since_last_shuffle = 0.0;
                }
            }

            double music_len = Mix_MusicDuration(_audio_input.get_music());
            double current_time = Mix_GetMusicPosition(_audio_input.get_music());

            if (_config.text_animation_enabled) {
                _animation_manager.update(music_len, current_time, titleLines);
            }

            _renderer.render(_pM);

            if (_text_renderer.is_initialized()) {
                float alpha = _config.text_animation_enabled ? _animation_manager.getAlpha() : 1.0f;
                float scale = _config.text_animation_enabled ? _animation_manager.getBreathingScale() : 1.0f;

                if (_config.show_song_title) {
                    const auto titlePositions = _animation_manager.getTitlePositions(titleLines);
                    for (size_t i = 0; i < titleLines.size(); ++i) {
                        _text_renderer.renderText(
                            titleLines[i], titlePositions[i].x, titlePositions[i].y,
                            scale, _config.songInfoFontColor, alpha, _config.show_text_border, _config.songInfoBorderColor,
                            _config.songInfoBorderThickness);
                    }
                }

                if (_config.show_artist_name) {
                    glm::vec2 artistPos = _animation_manager.getArtistPosition();
                    _text_renderer.renderText(
                        _config.artistName, artistPos.x, artistPos.y,
                        scale, _config.songInfoFontColor, alpha, _config.show_text_border, _config.songInfoBorderColor,
                        _config.songInfoBorderThickness);
                }

                if (_config.show_url) {
                    float url_scale = static_cast<float>(_config.urlFontSize) / static_cast<float>(_config.songInfoFontSize);
                    _text_renderer.renderText(_config.urlText, 10, 10, url_scale * scale, _config.urlFontColor,
                                            1.0f, _config.show_text_border, _config.urlBorderColor,
                                            _config.urlBorderThickness);
                }
            }

            _gui->render();

            if (_config.enable_recording) {
                std::vector<unsigned char> frame_buffer(_config.width * _config.height * 3);
                glReadPixels(0, 0, _config.width, _config.height, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer.data());

                // Flip the image vertically
                std::vector<unsigned char> flipped_buffer(_config.width * _config.height * 3);
                for (int y = 0; y < _config.height; ++y) {
                    memcpy(flipped_buffer.data() + y * _config.width * 3, frame_buffer.data() + (_config.height - 1 - y) * _config.width * 3, _config.width * 3);
                }

                _video_exporter.write_frame(flipped_buffer.data());
            }

            SDL_GL_SwapWindow(_window);

            // Frame pacing
            Uint32 frame_time = SDL_GetTicks() - frame_start_ticks;
            if (frame_time < frame_duration_ms) {
                SDL_Delay(frame_duration_ms - frame_time);
            }
            frame_count++;
        }

        current_audio_index++;
    }

    if (_config.enable_recording) {
        _video_exporter.end_export();
    }
}

void Core::cleanup() {
    _gui->cleanup();

    if (_pM) {
        projectm_destroy(_pM);
    }
    if (_context) {
        SDL_GL_DeleteContext(_context);
    }
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    SDL_Quit();
}
