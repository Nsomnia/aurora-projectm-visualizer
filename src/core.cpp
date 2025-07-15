// src/core.cpp
#include "core.h"
#include "utils/common.h"
#include "QtGui.h"
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
      _gui(nullptr),
      g_quit(false),
      _current_audio_index(0),
      _playback_state(PlaybackState::Stopped) {}

Core::~Core() {
    cleanup();
}

void Core::set_gui(std::unique_ptr<QtGui> gui) {
    _gui = std::move(gui);
}

void Core::set_audio_file_paths(const std::vector<std::string>& paths) {
    _config.audio_file_paths = paths;
    // When the playlist is reordered or changed, we might want to reset the current audio index
    // or ensure the currently playing track is still valid. For now, the run loop will pick up
    // the new list on its next iteration.
}

void Core::load_and_play_current_audio() {
    if (_config.audio_file_paths.empty()) {
        Logger::info("No audio files in playlist to play.");
        _playback_state = PlaybackState::Stopped;
        return;
    }

    if (_current_audio_index < 0) {
        _current_audio_index = _config.audio_file_paths.size() - 1;
    } else if (static_cast<size_t>(_current_audio_index) >= _config.audio_file_paths.size()) {
        _current_audio_index = 0;
    }

    const std::string& current_audio_file = _config.audio_file_paths[_current_audio_index];
    _audio_input.load_and_play_music(current_audio_file);
    SDL_Delay(100); // Small delay to ensure music is loaded

    _config.songTitle = sanitize_filename(current_audio_file);
    std::vector<std::string> titleLines = _text_manager.split_text(_config.songTitle, _config.width, 1.0f);
    _animation_manager.reset(titleLines);

    _playback_state = PlaybackState::Playing;
    Logger::info("Now playing: " + current_audio_file);
}

void Core::play_audio() {
    if (_playback_state == PlaybackState::Paused) {
        Mix_ResumeMusic();
        _playback_state = PlaybackState::Playing;
        Logger::info("Resumed audio playback.");
    } else if (_playback_state == PlaybackState::Stopped) {
        load_and_play_current_audio();
    } else {
        Logger::info("Audio is already playing.");
    }
}

void Core::pause_audio() {
    if (_playback_state == PlaybackState::Playing) {
        Mix_PauseMusic();
        _playback_state = PlaybackState::Paused;
        Logger::info("Paused audio playback.");
    } else {
        Logger::info("Audio is not playing or already paused.");
    }
}

void Core::stop_audio() {
    if (_playback_state != PlaybackState::Stopped) {
        Mix_HaltMusic();
        _playback_state = PlaybackState::Stopped;
        Logger::info("Stopped audio playback.");
    } else {
        Logger::info("Audio is already stopped.");
    }
}

void Core::next_audio() {
    if (!_config.audio_file_paths.empty()) {
        _current_audio_index++;
        load_and_play_current_audio();
    } else {
        Logger::warn("No audio files to skip to next.");
    }
}

void Core::prev_audio() {
    if (!_config.audio_file_paths.empty()) {
        _current_audio_index--;
        load_and_play_current_audio();
    } else {
        Logger::warn("No audio files to skip to previous.");
    }
}

void Core::next_preset() {
    std::string preset_path = _preset_manager.get_next_preset();
    if (!preset_path.empty()) {
        projectm_load_preset_file(_pM, preset_path.c_str(), true);
        Logger::info("Loaded next preset: " + preset_path);
    } else {
        Logger::warn("No next preset available.");
    }
}

void Core::prev_preset() {
    std::string preset_path = _preset_manager.get_prev_preset();
    if (!preset_path.empty()) {
        projectm_load_preset_file(_pM, preset_path.c_str(), true);
        Logger::info("Loaded previous preset: " + preset_path);
    } else {
        Logger::warn("No previous preset available.");
    }
}

std::string Core::get_current_preset_name() const {
    return _preset_manager.get_current_preset();
}

void Core::cleanup() {
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

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    return true;
}

void Core::run() {
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