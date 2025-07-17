## FILE START: include/AnimationManager.h
#pragma once

#include "Config.h"
#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>

enum class AnimationState {
    INITIAL_DELAY,
    BOUNCING,
    FADING_TO_TRANSPARENT,
    HOLDING_TRANSPARENT,
    FADING_TO_OPAQUE,
    RETURNING_TO_CENTER,
    FINISHED
};

class AnimationManager {
public:
    AnimationManager(Config& config, TextRenderer& textRenderer);

    void reset(const std::vector<std::string>& title_lines);
    void update(double music_len, double current_time, const std::vector<std::string>& title_lines);

    std::vector<glm::vec2> getTitlePositions(const std::vector<std::string>& title_lines) const;
    glm::vec2 getArtistPosition() const;
    float getAlpha() const;
    float getBreathingScale() const;

private:
    void initializePositions(const std::vector<std::string>& title_lines);
    void updateBouncing(float deltaTime, const std::vector<std::string>& title_lines);
    void updateReturning(float deltaTime);

    Config& _config;
    TextRenderer& _textRenderer;

    glm::vec2 _initialTitleBlockPosition;
    std::vector<glm::vec2> _titleLineOffsets;
    glm::vec2 _initialArtistPosition;

    glm::vec2 _titleBlockPosition;
    glm::vec2 _artistPosition;

    glm::vec2 _titleBlockVelocity;
    glm::vec2 _artistVelocity;

    float _alpha;
    float _breathingScale;
    AnimationState _currentState;
};
### FIELE END: include/AnimationManager.h
## FILE START: include/CliParser.h
#pragma once

#include "Config.h"
#include <string>

class CliParser {
public:
    static bool parse(Config& config, int argc, char* argv[]);

private:
    static void display_help(const std::string& program_name);
};
### FIELE END: include/CliParser.h
## FILE START: include/Config.h
// include/visualizer/Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <SDL.h>

// Define APP_VERSION if not already defined (e.g., by CMake)
#ifndef APP_VERSION
#define APP_VERSION "0.1.0-dev"
#endif

enum class AudioInputMode {
    SystemDefault,
    PipeWire,
    PulseAudio,
    File
};

struct Config {
    // Display
    int width = 1024;
    int height = 640;
    int fps = 30;

    // Font & Text
    std::string font_path = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf";
    std::string songTitle = "No Song Title";
    std::string artistName = "Unknown Artist";
    std::string urlText = "https://www.youtube.com/@theaudiovisualgod";
    int songInfoFontSize = 42;
    int urlFontSize = 28;
    glm::vec3 songInfoFontColor{1.0f, 1.0f, 1.0f};
    glm::vec3 urlFontColor{0.0f, 0.639f, 0.694f};
    glm::vec3 songInfoBorderColor{0.0f, 0.0f, 0.0f};
    glm::vec3 urlBorderColor{0.0f, 0.0f, 0.0f};
    float songInfoBorderThickness = 0.2f;
    float urlBorderThickness = 0.2f;
    bool show_text_border = true;
    bool show_song_title = true;
    bool show_artist_name = true;
    bool show_url = true;

    // Text Animation
    bool text_animation_enabled = true;
    float transitionTime = 10.0f;
    float pre_fade_delay = 10.0f;
    float bounce_duration = 7.0f;
    float bounce_speed = 67.0f;
    float bounce_randomness = 5.0f;
    float fade_to_min_duration = 10.0f;
    float minFadeTransparency = 0.15f;
    bool text_breathing_effect = true;
    float breathing_effect_amount = 0.05f;
    float breathing_effect_speed = 1.0f;

    // Presets
    std::string presetsDirectory = "presets/";
    std::string favoritesFile = "favorites.txt";
    bool shuffleEnabled = true;
    double presetDuration = 15.0;
    double presetBlendTime = 2.7;
    std::string preset_list_file;
    std::string broken_preset_directory = "broken_presets/";
    SDL_Keycode next_preset_key = SDLK_n;
    SDL_Keycode prev_preset_key = SDLK_p;
    SDL_Keycode mark_broken_preset_key = SDLK_b;
    SDL_Keycode favorite_preset_key = SDLK_f;
    bool use_default_projectm_visualizer = false;

    // Recording
    bool enable_recording = false;
    std::string video_directory = "videos";
    int video_framerate = 24;
    std::string ffmpeg_command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -framerate {FPS} -i - -i {AUDIO_FILE_PATH} -c:v libx265 -crf 28 -preset medium -c:a copy {OUTPUT_PATH}";

    // Audio
    std::vector<std::string> audio_file_paths;
    AudioInputMode audio_input_mode = AudioInputMode::PipeWire;
    std::string pipewire_sink_name = "AuroraSink";

    // Other
    bool show_version = false;
    bool verbose_logging = false;
};

// Utility function to convert hex color string to glm::vec3
inline glm::vec3 hexToVec3(std::string hex) {
    if (hex.length() == 6) {
        unsigned int r, g, b;
        sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
    }
    return glm::vec3(1.0f, 1.0f, 1.0f); // Default to white
}

#endif // CONFIG_H


### FIELE END: include/Config.h
## FILE START: include/ConfigLoader.h
#pragma once

#include "Config.h"
#include <string>

class ConfigLoader {
public:
    static bool load(Config& config, const std::string &executable_path_str);
};
### FIELE END: include/ConfigLoader.h
## FILE START: include/Gui.h
#pragma once

#include "Config.h"
#include "ImGuiIntegration.h"
#include <SDL.h>

class Core;

class Gui {
public:
    Gui(Config& config, Core& core);
    ~Gui();

    bool init(SDL_Window* window, SDL_GLContext context);
    void render();
    void handle_event(SDL_Event& event);
    void cleanup();

private:
    Config& _config;
    Core& _core;
    SDL_Window* _window;
};### FIELE END: include/Gui.h
## FILE START: include/ImGuiIntegration.h
#pragma once

#include <SDL.h>
#include "imgui.h"

class ImGuiIntegration {
public:
    static void init(SDL_Window* window, SDL_GLContext context);
    static void cleanup();
    static void new_frame();
    static void render();
    static void process_event(SDL_Event& event);
};
### FIELE END: include/ImGuiIntegration.h
## FILE START: include/TextManager.h
#pragma once

#include "TextRenderer.h"
#include <string>
#include <vector>

class TextManager {
public:
    TextManager(TextRenderer& textRenderer);

    std::vector<std::string> split_text(const std::string& text, int window_width, float scale);

private:
    std::string sanitize_text(const std::string& text);

    TextRenderer& _textRenderer;
};
### FIELE END: include/TextManager.h
## FILE START: include/TextRenderer.h
#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>

struct Character {
    unsigned int textureID;
    glm::ivec2   size;
    glm::ivec2   bearing;
    unsigned int advance;
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool init(const std::string& fontPath, int fontSize);
    void setProjection(int width, int height);
    void cleanup();
    void renderText(const std::string& text, float x, float y, float scale, 
                    const glm::vec3& color, float alpha = 1.0f, 
                    bool show_border = true, const glm::vec3& border_color = glm::vec3(0.0f), 
                    float border_thickness = 0.1f);
    float getTextWidth(const std::string& text, float scale);
    glm::vec4 getTextBounds(const std::string& text, float x, float y, float scale);
    bool is_initialized() const { return _initialized; }

private:
    bool initShaders();
    void renderTextPass(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, const glm::vec3& border_color, float border_thickness);

    GLuint _shaderProgram;
    GLuint _vao, _vbo;
    FT_Library _ft;
    FT_Face _face;
    std::map<char, Character> _characters;
    int _width, _height;
    bool _initialized;
};
### FIELE END: include/TextRenderer.h
## FILE START: include/VideoExporter.h
#pragma once

#include <string>
#include <cstdio>
#include "Config.h"

class VideoExporter {
public:
    VideoExporter(const Config& config);
    ~VideoExporter();

    bool start_export(int width, int height);
    void write_frame(const unsigned char* pixels);
    void end_export();

private:
    const Config& _config;
    FILE* _ffmpeg_pipe;
    int _width;
    int _height;
};
### FIELE END: include/VideoExporter.h
## FILE START: include/audio_input.h
#pragma once

#include "Config.h"
#include <SDL_mixer.h>
#include <projectM-4/projectM.h>

struct AudioData {
    projectm_handle pM;
};

class AudioInput {
public:
    AudioInput(Config& config);
    ~AudioInput();

    bool init();
    void load_and_play_music(const std::string& music_file);
    void cleanup();
    Mix_Music* get_music() const { return _music; }
    double get_audio_duration() const;

    void set_projectm_handle(projectm_handle pM);

    static void audio_callback(void* userdata, Uint8* stream, int len);

private:
    Config& _config;
    AudioData _audio_data;
    Mix_Music* _music;
};
### FIELE END: include/audio_input.h
## FILE START: include/backends/audio_backend.h
// include/visualizer/backends/audio_backend.h
#ifndef VISUALIZER_BACKENDS_AUDIO_BACKEND_H
#define VISUALIZER_BACKENDS_AUDIO_BACKEND_H

// Abstract interface for audio input (PulseAudio, PipeWire, ALSA, etc.)

#endif // VISUALIZER_BACKENDS_AUDIO_BACKEND_H

### FIELE END: include/backends/audio_backend.h
## FILE START: include/backends/display_backend.h
// include/visualizer/backends/display_backend.h
#ifndef VISUALIZER_BACKENDS_DISPLAY_BACKEND_H
#define VISUALIZER_BACKENDS_DISPLAY_BACKEND_H

// Abstract interface for display server interaction (X11, Wayland)

#endif // VISUALIZER_BACKENDS_DISPLAY_BACKEND_H

### FIELE END: include/backends/display_backend.h
## FILE START: include/core.h
#pragma once

#include "Config.h"
#include "renderer.h"
#include "event_handler.h"
#include "audio_input.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include "Gui.h"
#include "VideoExporter.h"

#include <SDL.h>
#include <projectM-4/projectM.h>
#include <memory>

class Core {
public:
    Core(Config& config);
    ~Core();

    bool init();
    void run();
    void cleanup();
    Renderer& get_renderer() { return _renderer; }

private:
    Config& _config;
    SDL_Window* _window;
    SDL_GLContext _context;
    projectm_handle _pM;
    Renderer _renderer;
    EventHandler _event_handler;
    AudioInput _audio_input;
    PresetManager _preset_manager;
    TextRenderer _text_renderer;
    TextManager _text_manager;
    AnimationManager _animation_manager;
    VideoExporter _video_exporter;
    std::unique_ptr<Gui> _gui;

    bool g_quit;
};
### FIELE END: include/core.h
## FILE START: include/event_handler.h
#pragma once

#include "Config.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <projectM-4/projectM.h>
#include <string>
#include <vector>

class EventHandler {
public:
    EventHandler(Config& config, PresetManager& presetManager, AnimationManager& animationManager, TextRenderer& textRenderer, TextManager& textManager);
    ~EventHandler();

    void handle_event(const SDL_Event& event, bool& g_quit, int& current_audio_index, double& time_since_last_shuffle, std::string& currentPreset, projectm_handle pM, std::vector<std::string>& titleLines);

private:
    Config& _config;
    PresetManager& _presetManager;
    AnimationManager& _animationManager;
    TextRenderer& _textRenderer;
    TextManager& _textManager;
};
### FIELE END: include/event_handler.h
## FILE START: include/preset_manager.h
#pragma once

#include "Config.h"
#include <string>
#include <vector>

class PresetManager {
public:
    PresetManager(const Config& config);

    void load_presets();
    std::string get_next_preset();
    std::string get_prev_preset();
    std::string get_current_preset() const;

    void mark_current_preset_as_broken();
    void toggle_favorite_current_preset();


private:
    void load_favorites();
    void save_favorites();
    std::string get_random_preset(const std::vector<std::string>& preset_list);


    const Config& _config;
    std::vector<std::string> _all_presets;
    std::vector<std::string> _favorite_presets;
    std::vector<std::string> _history;
    int _current_preset_index = -1;
    int _history_index = -1;
};
### FIELE END: include/preset_manager.h
## FILE START: include/renderer.h
#pragma once

#include "Config.h"
#include "TextRenderer.h"
#include "AnimationManager.h"
#include <SDL.h>
#include <projectM-4/projectM.h>
#include <GL/glew.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(SDL_Window* window, SDL_GLContext* context, Config& config);
    void render(projectm_handle pM);
    void cleanup();

    bool create_fbo(int width, int height);
    GLuint get_fbo_texture() const { return _fbo_texture; }

private:
    void render_to_fbo(projectm_handle pM);

    SDL_Window* _window;
    SDL_GLContext _context;
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;
};### FIELE END: include/renderer.h
## FILE START: include/utils/Logger.h
#pragma once

#include <string>
#include <iostream>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static void set_log_level(LogLevel level) {
        _log_level = level;
    }

    static void set_verbose_logging(bool verbose) {
        _verbose_logging = verbose;
    }

    static void debug(const std::string& message) {
        log(LogLevel::DEBUG, "DEBUG", message);
    }

    static void info(const std::string& message) {
        log(LogLevel::INFO, "INFO", message);
    }

    static void warn(const std::string& message) {
        log(LogLevel::WARN, "WARNING", message);
    }

    static void error(const std::string& message) {
        log(LogLevel::ERROR, "ERROR", message);
    }

private:
    static void log(LogLevel level, const std::string& prefix, const std::string& message) {
        if (!_verbose_logging && level == LogLevel::DEBUG) {
            return;
        }
        if (level >= _log_level) {
            std::lock_guard<std::mutex> lock(_mutex);
            std::cerr << "[" << prefix << "] " << message << std::endl;
        }
    }

    static LogLevel _log_level;
    static bool _verbose_logging;
    static std::mutex _mutex;
};

// Initialize static members
// These will be defined in the .cpp file
// LogLevel Logger::_log_level = LogLevel::INFO;
// bool Logger::_verbose_logging = false;
// std::mutex Logger::_mutex;
### FIELE END: include/utils/Logger.h
## FILE START: include/utils/common.h
// include/visualizer/utils/common.h
#ifndef VISUALIZER_UTILS_COMMON_H
#define VISUALIZER_UTILS_COMMON_H

#include <string>
#include <vector>

// Common utility functions and definitions
std::string sanitize_filename(const std::string &filepath);
std::string sanitize_for_filename(const std::string& input);
std::vector<std::string> wrapText(const std::string &text, int lineLengthTarget);

#endif // VISUALIZER_UTILS_COMMON_H


### FIELE END: include/utils/common.h
## FILE START: src/AnimationManager.cpp
// src/AnimationManager.cpp
#include "AnimationManager.h"
#include "TextRenderer.h"
#include <glm/gtc/random.hpp>
#include <algorithm> // For std::min/max
#include <iostream>
#include <cmath>

AnimationManager::AnimationManager(Config &config, TextRenderer &textRenderer)
    : _config(config), _textRenderer(textRenderer), _artistPosition(0.0f),
      _artistVelocity(0.0f), _alpha(1.0f), _breathingScale(1.0f),
      _currentState(AnimationState::BOUNCING) {}

void AnimationManager::reset(const std::vector<std::string>& title_lines) {
    initializePositions(title_lines);
    _titleBlockPosition = _initialTitleBlockPosition;
    _artistPosition = _initialArtistPosition;

    _titleBlockVelocity = glm::linearRand(glm::vec2(-_config.bounce_speed, -_config.bounce_speed), glm::vec2(_config.bounce_speed, _config.bounce_speed));
    _artistVelocity = glm::linearRand(glm::vec2(-_config.bounce_speed, -_config.bounce_speed), glm::vec2(_config.bounce_speed, _config.bounce_speed));

    _alpha = 1.0f;
    _breathingScale = 1.0f;
    _currentState = AnimationState::BOUNCING;
}

void AnimationManager::initializePositions(const std::vector<std::string>& title_lines) {
    _titleLineOffsets.clear();
    float total_height = title_lines.size() * _config.songInfoFontSize;
    float max_width = 0;
    for (const auto& line : title_lines) {
        max_width = std::max(max_width, _textRenderer.getTextWidth(line, 1.0f));
    }

    _initialTitleBlockPosition = {
        (_config.width - max_width) / 2.0f,
        (_config.height / 2.0f) + (total_height / 2.0f)
    };

    float current_y = 0;
    for (const auto& line : title_lines) {
        float titleWidth = _textRenderer.getTextWidth(line, 1.0f);
        _titleLineOffsets.push_back({
            (max_width - titleWidth) / 2.0f,
            current_y
        });
        current_y -= _config.songInfoFontSize;
    }

    float artistWidth = _textRenderer.getTextWidth(_config.artistName, 1.0f);
    _initialArtistPosition = {
        (_config.width - artistWidth) / 2.0f,
        _initialTitleBlockPosition.y - total_height - _config.songInfoFontSize
    };
}

void AnimationManager::update(double music_len, double current_time, const std::vector<std::string>& title_lines) {
    if (music_len <= 0) return;

    float deltaTime = 1.0f / _config.fps;
    float time_until_end = music_len - current_time;

    // Breathing effect
    if (_config.text_breathing_effect) {
        float sine_wave = sin(current_time * _config.breathing_effect_speed);
        _breathingScale = 1.0f + sine_wave * _config.breathing_effect_amount;
    }

    // State transitions
    switch (_currentState) {
        case AnimationState::BOUNCING:
            if (current_time >= _config.pre_fade_delay) {
                _currentState = AnimationState::FADING_TO_TRANSPARENT;
            }
            updateBouncing(deltaTime, title_lines);
            break;
        case AnimationState::FADING_TO_TRANSPARENT:
            {
                float fade_progress = (current_time - _config.pre_fade_delay) / _config.fade_to_min_duration;
                _alpha = 1.0f - fade_progress * (1.0f - _config.minFadeTransparency);
                _alpha = std::max(_config.minFadeTransparency, _alpha);
                if (_alpha <= _config.minFadeTransparency) {
                    _currentState = AnimationState::HOLDING_TRANSPARENT;
                }
                updateBouncing(deltaTime, title_lines);
            }
            break;
        case AnimationState::HOLDING_TRANSPARENT:
            if (time_until_end <= _config.transitionTime) {
                _currentState = AnimationState::FADING_TO_OPAQUE;
            }
            updateBouncing(deltaTime, title_lines);
            break;
        case AnimationState::FADING_TO_OPAQUE:
            {
                float fade_progress = (_config.transitionTime - time_until_end) / _config.transitionTime;
                _alpha = _config.minFadeTransparency + fade_progress * (1.0f - _config.minFadeTransparency);
                _alpha = std::min(1.0f, _alpha);
                if (_alpha >= 1.0f) {
                    _currentState = AnimationState::RETURNING_TO_CENTER;
                }
                updateBouncing(deltaTime, title_lines);
            }
            break;
        case AnimationState::RETURNING_TO_CENTER:
            updateReturning(deltaTime);
            if (glm::distance(_titleBlockPosition, _initialTitleBlockPosition) < 1.0f &&
                glm::distance(_artistPosition, _initialArtistPosition) < 1.0f) {
                _currentState = AnimationState::FINISHED;
                _titleBlockPosition = _initialTitleBlockPosition;
                _artistPosition = _initialArtistPosition;
            }
            break;
        case AnimationState::FINISHED:
            // Animation is done, do nothing.
            break;
    }
     _alpha = std::max(0.0f, std::min(1.0f, _alpha));
}


void AnimationManager::updateBouncing(float deltaTime, const std::vector<std::string>& title_lines) {
    _titleBlockPosition += _titleBlockVelocity * deltaTime;

    float block_width = 0;
    for(const auto& line : title_lines) {
        block_width = std::max(block_width, _textRenderer.getTextWidth(line, 1.0f));
    }
    float block_height = title_lines.size() * _config.songInfoFontSize;

    if (_titleBlockPosition.x < 0 || _titleBlockPosition.x + block_width > _config.width) {
        _titleBlockVelocity.x *= -1;
        _titleBlockVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _titleBlockPosition.x = std::max(0.0f, std::min(_titleBlockPosition.x, _config.width - block_width));
    }
    float half_block_height = block_height / 2.0f;
    if (_titleBlockPosition.y - half_block_height < 0 || _titleBlockPosition.y + half_block_height > _config.height) {
        _titleBlockVelocity.y *= -1;
        _titleBlockVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _titleBlockPosition.y = std::max(half_block_height, std::min(_titleBlockPosition.y, (float)_config.height - half_block_height));
    }


    _artistPosition += _artistVelocity * deltaTime;
    float artistWidth = _textRenderer.getTextWidth(_config.artistName, 1.0f);
    float artistHeight = _config.songInfoFontSize; // Assuming font size is the height for artist text
    float half_artist_height = artistHeight / 2.0f;

    if (_artistPosition.x < 0 || _artistPosition.x + artistWidth > _config.width) {
        _artistVelocity.x = -_artistVelocity.x;
        _artistVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _artistPosition.x = std::max(0.0f, std::min(_artistPosition.x, _config.width - artistWidth));
    }
    if (_artistPosition.y - half_artist_height < 0 || _artistPosition.y + half_artist_height > _config.height) {
        _artistVelocity.y = -_artistVelocity.y;
        _artistVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _artistPosition.y = std::max(half_artist_height, std::min(_artistPosition.y, (float)_config.height - half_artist_height));
    }
}

void AnimationManager::updateReturning(float deltaTime) {
    glm::vec2 direction = glm::normalize(_initialTitleBlockPosition - _titleBlockPosition);
    _titleBlockPosition += direction * _config.bounce_speed * deltaTime;

    glm::vec2 artistDirection = glm::normalize(_initialArtistPosition - _artistPosition);
    _artistPosition += artistDirection * _config.bounce_speed * deltaTime;
}

std::vector<glm::vec2> AnimationManager::getTitlePositions(const std::vector<std::string>& title_lines) const {
    std::vector<glm::vec2> positions;
    for (size_t i = 0; i < title_lines.size(); ++i) {
        positions.push_back(_titleBlockPosition + _titleLineOffsets[i]);
    }
    return positions;
}

glm::vec2 AnimationManager::getArtistPosition() const {
  return _artistPosition;
}

float AnimationManager::getAlpha() const { return _alpha; }

float AnimationManager::getBreathingScale() const { return _breathingScale; }### FIELE END: src/AnimationManager.cpp
## FILE START: src/CliParser.cpp
// src/CliParser.cpp
#include "CliParser.h"
#include "Config.h"
#include "utils/Logger.h"
#include <vector>
#include <functional>
#include <unordered_map>

void CliParser::display_help(const std::string &program_name) {
  // ANSI escape codes for colors
  const std::string RESET = "\033[0m";
  const std::string BOLD = "\033[1m";
  const std::string GREEN = "\033[32m";
  const std::string YELLOW = "\033[33m";
  const std::string CYAN = "\033[36m";
  const std::string MAGENTA = "\033[35m";

  std::cout << BOLD << GREEN << "Aurora Visualizer v" << APP_VERSION << RESET << "\n\n"
            << BOLD << YELLOW << "Usage: " << program_name << RESET
            << " [options] [audio_file_1 audio_file_2 ...]\n\n"

            << BOLD << MAGENTA << "Display & Performance" << RESET << "\n"
            << "  " << BOLD << GREEN << "--width <px>" << RESET << "               Set window width (default: 1024).\n"
            << "  " << BOLD << GREEN << "--height <px>" << RESET << "              Set window height (default: 640).\n"
            << "  " << BOLD << GREEN << "--fps <value>" << RESET << "              Set frames per second (default: 30).\n\n"

            << BOLD << MAGENTA << "Text & Font" << RESET << "\n"
            << "  " << BOLD << GREEN << "--font-path <path>" << RESET << "         Path to the font file (TTF/OTF).\n"
            << "  " << BOLD << GREEN << "--song-info-font-size <pt>" << RESET << " Font size for song title and artist.\n"
            << "  " << BOLD << GREEN << "--url-font-size <pt>" << RESET << "       Font size for the URL text.\n"
            << "  " << BOLD << GREEN << "--song-info-color <hex>" << RESET << "    Font color for song info (e.g., FFFFFF).\n"
            << "  " << BOLD << GREEN << "--url-color <hex>" << RESET << "          Font color for URL (e.g., 00A3B1).\n"
            << "  " << BOLD << GREEN << "--song-info-border-color <hex>" << RESET << " Color for the song info text border.\n"
            << "  " << BOLD << GREEN << "--url-border-color <hex>" << RESET << "   Color for the URL text border.\n"
            << "  " << BOLD << GREEN << "--song-info-border-thickness <val>" << RESET << " Thickness of the song info text border.\n"
            << "  " << BOLD << GREEN << "--url-border-thickness <val>" << RESET << "   Thickness of the URL text border.\n\n"

            << BOLD << MAGENTA << "Text Content & Visibility" << RESET << "\n"
            << "  " << BOLD << GREEN << "--title <text>" << RESET << "             Custom text to display as the title.\n"
            << "  " << BOLD << GREEN << "--artist-name <name>" << RESET << "       Set artist name overlay.\n"
            << "  " << BOLD << GREEN << "--url-text <text>" << RESET << "          Set URL text overlay.\n"
            << "  " << BOLD << GREEN << "--hide-title" << RESET << "               Hide the song title text.\n"
            << "  " << BOLD << GREEN << "--hide-artist" << RESET << "              Hide the artist name text.\n"
            << "  " << BOLD << GREEN << "--hide-url" << RESET << "                 Hide the URL text.\n"
            << "  " << BOLD << GREEN << "--hide-border" << RESET << "              Hide the text border.\n\n"

            << BOLD << MAGENTA << "Animation" << RESET << "\n"
            << "  " << BOLD << GREEN << "--disable-text-animation" << RESET << "   Disable the text animation.\n"
            << "  " << BOLD << GREEN << "--transition-time <sec>" << RESET << "    Duration of the fade effect at the end.\n"
            << "  " << BOLD << GREEN << "--pre-fade-delay <sec>" << RESET << "     Time to bounce at full opacity before fading.\n"
            << "  " << BOLD << GREEN << "--bounce-duration <sec>" << RESET << "    How long the text bounces before fading.\n"
            << "  " << BOLD << GREEN << "--bounce-speed <pixels/sec>" << RESET << " Speed of the bouncing text.\n"
            << "  " << BOLD << GREEN << "--bounce-randomness <val>" << RESET << "  Amount of randomness to apply to bounce angle.\n"
            << "  " << BOLD << GREEN << "--fade-to-min-duration <sec>" << RESET << " Time it takes to fade to minimum transparency.\n"
            << "  " << BOLD << GREEN << "--min-transparency <0-1>" << RESET << "   The minimum transparency for the text.\n"
            << "  " << BOLD << GREEN << "--disable-breathing-effect" << RESET << " Disable the text breathing effect.\n"
            << "  " << BOLD << GREEN << "--breathing-amount <val>" << RESET << "   The amount the text size changes for the breathing effect.\n"
            << "  " << BOLD << GREEN << "--breathing-speed <val>" << RESET << "    The speed of the text breathing effect.\n\n"

            << BOLD << MAGENTA << "Presets" << RESET << "\n"
            << "  " << BOLD << GREEN << "--preset-duration <sec>" << RESET << "    Time before switching to the next preset.\n"
            << "  " << BOLD << GREEN << "--preset-blend-time <sec>" << RESET << "  Time for the blend transition between presets.\n"
            << "  " << BOLD << GREEN << "--preset-list-file <path>" << RESET << "  Path to a file containing a list of .milk presets.\n"
            << "  " << BOLD << GREEN << "--broken-preset-directory <path>" << RESET << " Directory to move broken presets to.\n"
            << "  " << BOLD << GREEN << "--favorites-file <path>" << RESET << "    Path to the favorites file.\n"
            << "  " << BOLD << GREEN << "--next-preset-key <key>" << RESET << "    Key to load the next random preset (e.g., 'n').\n"
            << "  " << BOLD << GREEN << "--prev-preset-key <key>" << RESET << "    Key to load the previous preset (e.g., 'p').\n"
            << "  " << BOLD << GREEN << "--mark-broken-preset-key <key>" << RESET << " Key to mark the current preset as broken (e.g., 'b').\n"
            << "  " << BOLD << GREEN << "--favorite-preset-key <key>" << RESET << " Key to mark the current preset as a favorite (e.g., 'f').\n"
            << "  " << BOLD << GREEN << "--use-default-projectm-visualizer" << RESET << " Use projectM's default visualizer for testing audio input.\n\n"

            << BOLD << MAGENTA << "Recording" << RESET << "\n"
            << "  " << BOLD << GREEN << "--record-video" << RESET << "             Enable video recording.\n"
            << "  " << BOLD << GREEN << "--audio-input-mode <mode>" << RESET << "  Set audio input mode (SystemDefault, PipeWire, PulseAudio, File). Default: PipeWire.\n"
            << "  " << BOLD << GREEN << "--pipewire-sink-name <name>" << RESET << " Set the name of the virtual PipeWire sink (default: AuroraSink).\n"
            << "  " << BOLD << GREEN << "--output-directory <path>" << RESET << "  Directory to save recorded videos.\n"
            << "  " << BOLD << GREEN << "--video-framerate <value>" << RESET << "  Set video recording framerate.\n"
            << "  " << BOLD << GREEN << "--ffmpeg-command <cmd>" << RESET << "     The ffmpeg command template for recording.\n\n"

            << BOLD << MAGENTA << "Other" << RESET << "\n"
            << "  " << BOLD << GREEN << "--audio-file <path>" << RESET << "        Add an audio file to the playlist (can be used multiple times).\n"
            << "  " << BOLD << GREEN << "--version" << RESET << "                  Display application version.\n"
            << "  " << BOLD << GREEN << "--verbose" << RESET << "                  Enable verbose logging.\n"
            << "  " << BOLD << GREEN << "-h, --help" << RESET << "                 Display this help message.\n";
}

bool CliParser::parse(Config& config, int argc, char *argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    if (argc < 2) { // No arguments provided
        display_help(argv[0]);
        return false;
    }

    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            display_help(argv[0]);
            return false;
        }
    }

    std::unordered_map<std::string, std::function<void(const std::string&)>> parsers;
    parsers["--width"] = [&config](const std::string& v){ config.width = std::stoi(v); };
    parsers["--height"] = [&config](const std::string& v){ config.height = std::stoi(v); };
    parsers["--fps"] = [&config](const std::string& v){ config.fps = std::stoi(v); };
    parsers["--output-directory"] = [&config](const std::string& v){ config.video_directory = v; };
    parsers["--video-framerate"] = [&config](const std::string& v){ config.video_framerate = std::stoi(v); };
    parsers["--ffmpeg-command"] = [&config](const std::string& v){ config.ffmpeg_command = v; };
    parsers["--preset-duration"] = [&config](const std::string& v){ config.presetDuration = std::stod(v); };
    parsers["--preset-blend-time"] = [&config](const std::string& v){ config.presetBlendTime = std::stod(v); };
    parsers["--preset-list-file"] = [&config](const std::string& v){ config.preset_list_file = v; };
    parsers["--broken-preset-directory"] = [&config](const std::string& v){ config.broken_preset_directory = v; };
    parsers["--favorites-file"] = [&config](const std::string& v){ config.favoritesFile = v; };
    parsers["--next-preset-key"] = [&config](const std::string& v){ config.next_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--prev-preset-key"] = [&config](const std::string& v){ config.prev_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--mark-broken-preset-key"] = [&config](const std::string& v){ config.mark_broken_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--favorite-preset-key"] = [&config](const std::string& v){ config.favorite_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--url-text"] = [&config](const std::string& v){ config.urlText = v; };
    parsers["--artist-name"] = [&config](const std::string& v){ config.artistName = v; };
    parsers["--font-path"] = [&config](const std::string& v){ config.font_path = v; };
    parsers["--audio-file"] = [&config](const std::string& v){ config.audio_file_paths.push_back(v); };
    parsers["--title"] = [&config](const std::string& v){ config.songTitle = v; };
    parsers["--song-info-font-size"] = [&config](const std::string& v){ config.songInfoFontSize = std::stoi(v); };
    parsers["--url-font-size"] = [&config](const std::string& v){ config.urlFontSize = std::stoi(v); };
    parsers["--song-info-color"] = [&config](const std::string& v){ config.songInfoFontColor = hexToVec3(v); };
    parsers["--url-color"] = [&config](const std::string& v){ config.urlFontColor = hexToVec3(v); };
    parsers["--song-info-border-color"] = [&config](const std::string& v){ config.songInfoBorderColor = hexToVec3(v); };
    parsers["--url-border-color"] = [&config](const std::string& v){ config.urlBorderColor = hexToVec3(v); };
    parsers["--border-thickness"] = [&config](const std::string& v){ config.urlBorderThickness = std::stof(v); };
    parsers["--song-info-border-thickness"] = [&config](const std::string& v){ config.songInfoBorderThickness = std::stof(v); };
    parsers["--url-border-thickness"] = [&config](const std::string& v){ config.urlBorderThickness = std::stof(v); };
    parsers["--transition-time"] = [&config](const std::string& v){ config.transitionTime = std::stof(v); };
    parsers["--pre-fade-delay"] = [&config](const std::string& v){ config.pre_fade_delay = std::stof(v); };
    parsers["--bounce-duration"] = [&config](const std::string& v){ config.bounce_duration = std::stof(v); };
    parsers["--bounce-speed"] = [&config](const std::string& v){ config.bounce_speed = std::stof(v); };
    parsers["--bounce-randomness"] = [&config](const std::string& v){ config.bounce_randomness = std::stof(v); };
    parsers["--fade-to-min-duration"] = [&config](const std::string& v){ config.fade_to_min_duration = std::stof(v); };
    parsers["--min-transparency"] = [&config](const std::string& v){ config.minFadeTransparency = std::stof(v); };
    parsers["--breathing-amount"] = [&config](const std::string& v){ config.breathing_effect_amount = std::stof(v); };
    parsers["--breathing-speed"] = [&config](const std::string& v){ config.breathing_effect_speed = std::stof(v); };
    parsers["--audio-input-mode"] = [&config](const std::string& v){
        if (v == "SystemDefault") config.audio_input_mode = AudioInputMode::SystemDefault;
        else if (v == "PipeWire") config.audio_input_mode = AudioInputMode::PipeWire;
        else if (v == "PulseAudio") config.audio_input_mode = AudioInputMode::PulseAudio;
        else if (v == "File") config.audio_input_mode = AudioInputMode::File;
        else std::cerr << "Unknown audio input mode: " << v << std::endl;
    };
    parsers["--pipewire-sink-name"] = [&config](const std::string& v){ config.pipewire_sink_name = v; };

    std::unordered_map<std::string, std::function<void()>> flag_parsers;
    flag_parsers["--record-video"] = [&config](){ config.enable_recording = true; };
    flag_parsers["--disable-text-animation"] = [&config](){ config.text_animation_enabled = false; };
    flag_parsers["--hide-title"] = [&config](){ config.show_song_title = false; };
    flag_parsers["--hide-artist"] = [&config](){ config.show_artist_name = false; };
    flag_parsers["--hide-url"] = [&config](){ config.show_url = false; };
    flag_parsers["--hide-border"] = [&config](){ config.show_text_border = false; };
    flag_parsers["--disable-breathing-effect"] = [&config](){ config.text_breathing_effect = false; };
    flag_parsers["--version"] = [&config](){ config.show_version = true; };
    flag_parsers["--verbose"] = [&config](){ config.verbose_logging = true; };
    flag_parsers["--use-default-projectm-visualizer"] = [&config](){ config.use_default_projectm_visualizer = true; };

    for (size_t i = 0; i < args.size(); ++i) {
        std::string arg = args[i];
        std::string key = arg;
        std::string value;

        size_t equals_pos = arg.find('=');
        if (equals_pos != std::string::npos) {
            key = arg.substr(0, equals_pos);
            value = arg.substr(equals_pos + 1);
        }

        if (key.rfind("--", 0) == 0) {
            auto it = parsers.find(key);
            if (it != parsers.end()) {
                if (!value.empty()) {
                    it->second(value);
                } else if (i + 1 < args.size()) {
                    it->second(args[++i]);
                }
                continue;
            }

            auto flag_it = flag_parsers.find(key);
            if (flag_it != flag_parsers.end()) {
                if (equals_pos != std::string::npos) {
                     Logger::error("Flag option does not take a value: " + key);
                     display_help(argv[0]);
                     return false;
                }
                flag_it->second();
                continue;
            }

            // If we are here, it's an unknown option
            Logger::error("Unknown option: " + key);
            display_help(argv[0]);
            return false;
        } else {
            // Positional argument
            config.audio_file_paths.push_back(arg);
        }
    }
    return true;
}
### FIELE END: src/CliParser.cpp
## FILE START: src/ConfigLoader.cpp
// src/ConfigLoader.cpp
#include "ConfigLoader.h"
#include "Config.h"
#include "utils/Logger.h"
#include "toml.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Helper function to resolve paths, expanding ~
static std::string resolve_path(const std::string& path_str) {
    if (path_str.empty()) {
        return "";
    }
    if (path_str[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            return std::string(home) + path_str.substr(1);
        }
    }
    return path_str;
}

bool ConfigLoader::load(Config& config, const std::string &executable_path_str) {
    fs::path config_path_to_load;

    // 1. Check for user-specific config in ~/.config/aurora-visualizer/config.toml
    const char* home_dir = getenv("HOME");
    if (home_dir) {
        fs::path user_config_path = fs::path(home_dir) / ".config" / "aurora-visualizer" / "config.toml";
        if (fs::exists(user_config_path)) {
            config_path_to_load = user_config_path;
            Logger::info("Loading user config from: " + user_config_path.string());
        }
    }

    // 2. If no user config, fall back to default.toml in the project's config directory
    if (config_path_to_load.empty()) {
        fs::path exe_dir = fs::path(executable_path_str).parent_path();
        // This assumes the executable is in 'build/', so we go up one level.
        // This might need to be more robust for system-wide installs.
        fs::path project_root = exe_dir.parent_path();
        fs::path default_config_path = project_root / "config" / "default.toml";

        if (fs::exists(default_config_path)) {
            config_path_to_load = default_config_path;
            Logger::info("Loading default config from: " + default_config_path.string());
        }
    }

    if (config_path_to_load.empty() || !fs::exists(config_path_to_load)) {
        Logger::warn("No configuration file found. Using built-in default values.");
        return true; // Not a fatal error
    }

    try {
        toml::table tbl = toml::parse_file(config_path_to_load.string());

        // --- Display & Performance ---
        config.width = tbl["display"]["width"].value_or(config.width);
        config.height = tbl["display"]["height"].value_or(config.height);
        config.fps = tbl["display"]["fps"].value_or(config.fps);

        // --- Text & Font ---
        config.font_path = resolve_path(tbl["text"]["font_path"].value_or(config.font_path));
        config.songInfoFontSize = tbl["text"]["song_info_font_size"].value_or(config.songInfoFontSize);
        config.urlFontSize = tbl["text"]["url_font_size"].value_or(config.urlFontSize);
        config.songInfoFontColor = hexToVec3(tbl["text"]["song_info_color"].value_or("FFFFFF"));
        config.urlFontColor = hexToVec3(tbl["text"]["url_color"].value_or("00A3B1"));
        config.songInfoBorderColor = hexToVec3(tbl["text"]["song_info_border_color"].value_or("000000"));
        config.urlBorderColor = hexToVec3(tbl["text"]["url_border_color"].value_or("000000"));
        config.songInfoBorderThickness = tbl["text"]["song_info_border_thickness"].value_or(config.songInfoBorderThickness);
        config.urlBorderThickness = tbl["text"]["url_border_thickness"].value_or(config.urlBorderThickness);

        // --- Text Content & Visibility ---
        config.artistName = tbl["text"]["artist_name"].value_or(config.artistName);
        config.urlText = tbl["text"]["url_text"].value_or(config.urlText);
        config.show_song_title = tbl["text"]["show_song_title"].value_or(config.show_song_title);
        config.show_artist_name = tbl["text"]["show_artist_name"].value_or(config.show_artist_name);
        config.show_url = tbl["text"]["show_url"].value_or(config.show_url);
        config.show_text_border = tbl["text"]["show_text_border"].value_or(config.show_text_border);

        // --- Recording ---
        config.enable_recording = tbl["recording"]["enable_recording"].value_or(config.enable_recording);
        config.video_directory = resolve_path(tbl["recording"]["video_directory"].value_or(config.video_directory));
        config.video_framerate = tbl["recording"]["video_framerate"].value_or(config.video_framerate);
        config.ffmpeg_command = tbl["recording"]["ffmpeg_command"].value_or(config.ffmpeg_command);

        // ... (add other sections like animation, presets, etc. in the same way)

    } catch (const toml::parse_error& err) {
        Logger::error("Failed to parse config file: " + std::string(err.what()));
        return false;
    }

    return true;
}


### FIELE END: src/ConfigLoader.cpp
## FILE START: src/Gui.cpp
// src/Gui.cpp
#include "Gui.h"
#include "core.h"
#include "ImGuiIntegration.h"
#include "renderer.h"
#include <GL/glew.h>

Gui::Gui(Config& config, Core& core) : _config(config), _core(core), _window(nullptr) {}

Gui::~Gui() {}

bool Gui::init(SDL_Window* window, SDL_GLContext context) {
    _window = window;
    ImGuiIntegration::init(_window, context);
    return true;
}

void Gui::render() {
    ImGuiIntegration::new_frame();

    // Create a window to display the visualization
    ImGui::Begin("Visualization");
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    // The FBO texture is rendered upside down, so we need to flip the V coordinate
    ImGui::Image((ImTextureID)(intptr_t)_core.get_renderer().get_fbo_texture(), window_size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    // --- Test Window ---
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is a test window.");
    ImGui::End();

    ImGuiIntegration::render();
}

void Gui::handle_event(SDL_Event& event) {
    ImGuiIntegration::process_event(event);
}

void Gui::cleanup() {
    ImGuiIntegration::cleanup();
}
### FIELE END: src/Gui.cpp
## FILE START: src/ImGuiIntegration.cpp
// src/ImGuiIntegration.cpp
#include "ImGuiIntegration.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

void ImGuiIntegration::init(SDL_Window* window, SDL_GLContext context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiIntegration::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiIntegration::new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiIntegration::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiIntegration::process_event(SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}
### FIELE END: src/ImGuiIntegration.cpp
## FILE START: src/TextManager.cpp
// src/TextManager.cpp
#include "TextManager.h"
#include <algorithm>
#include <regex>
#include <sstream> // Required for std::stringstream

TextManager::TextManager(TextRenderer& textRenderer) : _textRenderer(textRenderer) {}

std::string TextManager::sanitize_text(const std::string& text) {
    std::string sanitized = text;

    // Remove file extension
    size_t dot_pos = sanitized.rfind('.');
    if (dot_pos != std::string::npos) {
        sanitized = sanitized.substr(0, dot_pos);
    }

    // Normalize separators
    std::replace(sanitized.begin(), sanitized.end(), '_', ' ');
    std::replace(sanitized.begin(), sanitized.end(), '-', ' ');

    // Keep only allowed characters
    std::string result;
    for (char c : sanitized) {
        // removed this manually because songs often have round brackets c == '(' || c == ')') {
        if (isalnum(c) || c == ' ' || c == '[' || c == ']') {
            result += c;
        }
    }

    return result;
}

std::vector<std::string> TextManager::split_text(const std::string& text, int window_width, float scale) {
    std::string sanitized = sanitize_text(text);
    std::vector<std::string> lines;
    if (sanitized.empty()) {
        return lines;
    }

    std::string current_line;
    std::stringstream ss(sanitized);
    std::string word;

    const float max_width = window_width * 0.5f;

    while (ss >> word) {
        std::string test_line = current_line.empty() ? word : current_line + " " + word;
        if (_textRenderer.getTextWidth(test_line, scale) > max_width) {
            if (!current_line.empty()) {
                lines.push_back(current_line);
                current_line = word;
            } else {
                // The word itself is longer than the max width, so we must split it.
                std::string temp_word = word;
                while (_textRenderer.getTextWidth(temp_word, scale) > max_width) {
                    size_t split_pos = 0;
                    // Find the last character that fits
                    for (size_t i = 1; i <= temp_word.length(); ++i) {
                         if (_textRenderer.getTextWidth(temp_word.substr(0, i), scale) > max_width) {
                            split_pos = i - 1;
                            break;
                        }
                    }
                    // If we can't even fit one character, something is wrong.
                    // To prevent infinite loops, we'll just push the first character.
                    if (split_pos == 0 && !temp_word.empty()) {
                        split_pos = 1;
                    }
                    lines.push_back(temp_word.substr(0, split_pos));
                    temp_word = temp_word.substr(split_pos);
                }
                current_line = temp_word;
            }
        } else {
            current_line = test_line;
        }
    }

    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}
### FIELE END: src/TextManager.cpp
## FILE START: src/TextRenderer.cpp
#include "TextRenderer.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D text;
uniform vec3 textColor;
uniform float alpha;
uniform vec3 borderColor;
uniform float borderThickness;
void main() {
    float distance = texture(text, TexCoords).r;
    float smoothWidth = fwidth(distance);
    float outline = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);
    float border = smoothstep(0.5 - borderThickness - smoothWidth, 0.5 - borderThickness + smoothWidth, distance);
    vec4 blendedColor = mix(vec4(borderColor, alpha), vec4(textColor, alpha), outline);
    color = vec4(blendedColor.rgb, alpha * (outline + (1.0 - outline) * border));
}
)glsl";

TextRenderer::TextRenderer() : _shaderProgram(0), _vao(0), _vbo(0), _ft(nullptr), _face(nullptr), _width(0), _height(0), _initialized(false) {}

TextRenderer::~TextRenderer() {
    cleanup();
}

void TextRenderer::cleanup() {
    if (_shaderProgram) glDeleteProgram(_shaderProgram);
    for (auto const& [key, val] : _characters) {
        glDeleteTextures(1, &val.textureID);
    }
    _characters.clear();
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_face) FT_Done_Face(_face);
    if (_ft) FT_Done_FreeType(_ft);
}

bool TextRenderer::init(const std::string& fontPath, int fontSize) {
    if (FT_Init_FreeType(&_ft)) return false;
    if (FT_New_Face(_ft, fontPath.c_str(), 0, &_face)) return false;
    FT_Set_Pixel_Sizes(_face, 0, fontSize);

    if (!initShaders()) return false;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(_face, c, FT_LOAD_RENDER)) continue;
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _face->glyph->bitmap.width, _face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _characters[c] = {texture, glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows), glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top), static_cast<unsigned int>(_face->glyph->advance.x)};
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _initialized = true;
    return true;
}

void TextRenderer::setProjection(int width, int height) {
    _width = width;
    _height = height;
    glUseProgram(_shaderProgram);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(_width), 0.0f, static_cast<float>(_height));
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(0);
}

void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, bool show_border, const glm::vec3& border_color, float border_thickness) {
    // NOTE: The contrast adjustment logic was causing heap corruption and has been disabled.
    float final_border_thickness = show_border ? border_thickness : 0.0f;
    renderTextPass(text, x, y, scale, color, alpha, border_color, final_border_thickness);
}

glm::vec4 TextRenderer::getTextBounds(const std::string& text, float x, float y, float scale) {
    float min_x = x, max_x = x;
    float min_y = y, max_y = y;

    for (const char& c : text) {
        Character ch = _characters[c];
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        min_x = std::min(min_x, xpos);
        max_x = std::max(max_x, xpos + w);
        min_y = std::min(min_y, ypos);
        max_y = std::max(max_y, ypos + h);
        
        x += (ch.advance >> 6) * scale;
    }
    return glm::vec4(min_x, min_y, max_x - min_x, max_y - min_y);
}

void TextRenderer::renderTextPass(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, const glm::vec3& border_color, float border_thickness) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(_shaderProgram);
    glUniform3f(glGetUniformLocation(_shaderProgram, "textColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(_shaderProgram, "alpha"), alpha);
    glUniform3f(glGetUniformLocation(_shaderProgram, "borderColor"), border_color.x, border_color.y, border_color.z);
    glUniform1f(glGetUniformLocation(_shaderProgram, "borderThickness"), border_thickness);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);

    for (const char& c : text) {
        Character ch = _characters[c];
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f }, { xpos,     ypos,       0.0f, 1.0f }, { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f }, { xpos + w, ypos,       1.0f, 1.0f }, { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

float TextRenderer::getTextWidth(const std::string& text, float scale) {
    float width = 0.0f;
    for (const char& c : text) {
        width += (_characters[c].advance >> 6) * scale;
    }
    return width;
}

bool TextRenderer::initShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        return false;
    }

    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}
### FIELE END: src/TextRenderer.cpp
## FILE START: src/VideoExporter.cpp
#include "VideoExporter.h"
#include "utils/common.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <cstring>


namespace fs = std::filesystem;

// Helper function to escape a string for use as a shell argument
static std::string escape_shell_argument(const std::string& arg) {
    std::string escaped_arg = "'";
    for (char c : arg) {
        if (c == '\'') { // Corrected: single quote character literal using hex escape
            escaped_arg += "'\\''"; // Corrected: shell-escaped single quote using hex escape
        } else {
            escaped_arg += c;
        }
    }
    escaped_arg += "'";
    return escaped_arg;
}

// Helper function to replace placeholders in a string
static std::string replace_placeholders(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

VideoExporter::VideoExporter(const Config& config) : _config(config), _ffmpeg_pipe(nullptr) {}

VideoExporter::~VideoExporter() {
    if (_ffmpeg_pipe) {
        end_export();
    }
}

bool VideoExporter::start_export(int width, int height) {
    _width = width;
    _height = height;

    if (_config.ffmpeg_command.empty()) {
        std::cerr << "Error: ffmpeg_command is not set in the configuration." << std::endl;
        return false;
    }

    std::string sanitized_filename = "output";
    if (!_config.audio_file_paths.empty()) {
        sanitized_filename = sanitize_for_filename(_config.audio_file_paths[0]);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string timestamp = ss.str();

    std::string output_path = _config.video_directory + "/" + sanitized_filename + "_" + timestamp + ".mp4";
    std::string escaped_output_path = escape_shell_argument(output_path);

    std::string command = _config.ffmpeg_command;
    command = replace_placeholders(command, "{WIDTH}", std::to_string(_width));
    command = replace_placeholders(command, "{HEIGHT}", std::to_string(_height));
    command = replace_placeholders(command, "{FPS}", std::to_string(_config.video_framerate));
    if (!_config.audio_file_paths.empty()) {
        std::string escaped_audio_path = escape_shell_argument(_config.audio_file_paths[0]);
        command = replace_placeholders(command, "{AUDIO_FILE_PATH}", escaped_audio_path);
    } else {
        command = replace_placeholders(command, "-i {AUDIO_FILE_PATH}", "");
    }
    command = replace_placeholders(command, "{OUTPUT_PATH}", escaped_output_path);

    std::cout << "Starting ffmpeg with command: " << command << std::endl;

    _ffmpeg_pipe = popen(command.c_str(), "w");
    if (!_ffmpeg_pipe) {
        std::cerr << "Error: Could not open ffmpeg pipe." << std::endl;
        return false;
    }

    return true;
}

void VideoExporter::end_export() {
    if (_ffmpeg_pipe) {
        pclose(_ffmpeg_pipe);
        _ffmpeg_pipe = nullptr;
        std::cout << "Stopped ffmpeg process." << std::endl;
    }
}

void VideoExporter::write_frame(const unsigned char* pixels) {
    if (_ffmpeg_pipe) {
        fwrite(pixels, 1, _width * _height * 3, _ffmpeg_pipe);
        fflush(_ffmpeg_pipe);
    }
}### FIELE END: src/VideoExporter.cpp
## FILE START: src/audio_input.cpp
// src/audio_input.cpp
#include "audio_input.h"
#include "utils/Logger.h"
#include <vector>

AudioInput::AudioInput(Config& config) : _config(config), _music(nullptr) {
    _audio_data.pM = nullptr;
}

AudioInput::~AudioInput() {
    cleanup();
}

bool AudioInput::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_SetPostMix(audio_callback, &_audio_data);
    return true;
}

void AudioInput::load_and_play_music(const std::string& music_file) {
    if (_music) {
        Mix_FreeMusic(_music);
        _music = nullptr;
    }
    _music = Mix_LoadMUS(music_file.c_str());
    if (!_music) {
        Logger::error("Failed to load music: " + music_file + " - " + std::string(Mix_GetError()));
        return;
    }
    Mix_PlayMusic(_music, 1);
}

void AudioInput::cleanup() {
    if (_music) {
        Mix_FreeMusic(_music);
        _music = nullptr;
    }
    Mix_CloseAudio();
}

void AudioInput::set_projectm_handle(projectm_handle pM) {
    _audio_data.pM = pM;
}

double AudioInput::get_audio_duration() const {
    if (_music) {
        return Mix_MusicDuration(_music);
    }
    return 0.0;
}

void AudioInput::audio_callback(void* userdata, Uint8* stream, int len) {
    AudioData* audioData = static_cast<AudioData*>(userdata);
    if (!audioData || !audioData->pM) {
        return;
    }

    int samples = len / (2 * sizeof(int16_t));
    std::vector<float> float_samples(len / sizeof(int16_t));
    int16_t* pcm_stream = reinterpret_cast<int16_t*>(stream);

    for (int i = 0; i < len / 2; ++i) {
        float_samples[i] = static_cast<float>(pcm_stream[i]) / 32768.0f;
    }
    projectm_pcm_add_float(audioData->pM, float_samples.data(), samples, PROJECTM_STEREO);
}
### FIELE END: src/audio_input.cpp
## FILE START: src/backends/audio_backend.cpp
// src/backends/audio_backend.cpp
#include "backends/audio_backend.h"

// Implementation of audio backend (e.g., PulseAudio, PipeWire)
### FIELE END: src/backends/audio_backend.cpp
## FILE START: src/backends/display_backend.cpp
// src/backends/display_backend.cpp
#include "backends/display_backend.h"

// Implementation of display backend (e.g., X11, Wayland)
### FIELE END: src/backends/display_backend.cpp
## FILE START: src/core.cpp
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
}### FIELE END: src/core.cpp
## FILE START: src/event_handler.cpp
// src/event_handler.cpp
#include "event_handler.h"
#include <iostream>

EventHandler::EventHandler(Config& config, PresetManager& presetManager, AnimationManager& animationManager, TextRenderer& textRenderer, TextManager& textManager)
    : _config(config), _presetManager(presetManager), _animationManager(animationManager), _textRenderer(textRenderer), _textManager(textManager) {}

EventHandler::~EventHandler() {}

void EventHandler::handle_event(const SDL_Event& event, bool& g_quit, int& current_audio_index, double& time_since_last_shuffle, std::string& currentPreset, projectm_handle pM, std::vector<std::string>& titleLines) {
    if (event.type == SDL_QUIT) {
        g_quit = true;
    }
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                Mix_HaltMusic();
                break;
            case SDLK_LEFT:
                 if (current_audio_index > 0) {
                    current_audio_index -= 2; // Will be incremented back to the previous index
                    Mix_HaltMusic();
                }
                break;
            case SDLK_SPACE:
                if (Mix_PausedMusic()) {
                    Mix_ResumeMusic();
                } else {
                    Mix_PauseMusic();
                }
                break;
            case SDLK_q:
                g_quit = true;
                break;
            default:
                if (event.key.keysym.sym == _config.next_preset_key) {
                    currentPreset = _presetManager.get_next_preset();
                    if (!currentPreset.empty()) {
                        projectm_load_preset_file(pM, currentPreset.c_str(), true);
                    }
                    time_since_last_shuffle = 0.0;
                } else if (event.key.keysym.sym == _config.prev_preset_key) {
                    currentPreset = _presetManager.get_prev_preset();
                    if (!currentPreset.empty()) {
                        projectm_load_preset_file(pM, currentPreset.c_str(), true);
                    }
                    time_since_last_shuffle = 0.0;
                } else if (event.key.keysym.sym == _config.mark_broken_preset_key) {
                    _presetManager.mark_current_preset_as_broken();
                    currentPreset = _presetManager.get_next_preset();
                    if (!currentPreset.empty()) {
                        projectm_load_preset_file(pM, currentPreset.c_str(), true);
                    }
                    time_since_last_shuffle = 0.0;
                } else if (event.key.keysym.sym == _config.favorite_preset_key) {
                    _presetManager.toggle_favorite_current_preset();
                }
                break;
        }
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        _config.width = event.window.data1;
        _config.height = event.window.data2;
        glViewport(0, 0, _config.width, _config.height);
        projectm_set_window_size(pM, _config.width, _config.height);
        _textRenderer.setProjection(_config.width, _config.height);
        titleLines = _textManager.split_text(_config.songTitle, _config.width, 1.0f);
        _animationManager.reset(titleLines);
    }
}### FIELE END: src/event_handler.cpp
## FILE START: src/main.cpp
// src/main.cpp
#include "core.h"
#include "Config.h"
#include "ConfigLoader.h"
#include "CliParser.h"
#include "utils/Logger.h"
#include <csignal>
#include <iostream>

// Global quit flag for signal handler
volatile sig_atomic_t g_quit_flag = 0;

void signalHandler(int signum) {
    Logger::info("Caught signal " + std::to_string(signum) + ", shutting down...");
    g_quit_flag = true;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    Config config;
    if (!ConfigLoader::load(config, argv[0])) {
        Logger::error("Failed to load configuration.");
        return 1;
    }
    if (!CliParser::parse(config, argc, argv)) {
        return 0; // Exit gracefully if help was shown
    }

    Logger::set_verbose_logging(config.verbose_logging);
    if (config.verbose_logging) {
        Logger::info("Verbose logging enabled.");
        Logger::info("Configuration loaded.");
    }

    if (config.show_version) {
        Logger::info("Aurora Visualizer version " + std::string(APP_VERSION));
        return 0;
    }

    Core visualizerCore(config);
    if (!visualizerCore.init()) {
        Logger::error("Failed to initialize visualizer core.");
        return 1;
    }

    visualizerCore.run();

    return 0;
}### FIELE END: src/main.cpp
## FILE START: src/preset_manager.cpp
// src/preset_manager.cpp
#include "preset_manager.h"
#include "utils/Logger.h"
#include <fstream>
#include <random>
#include <filesystem>
#include <cstdlib> // For getenv
#include <algorithm>

namespace fs = std::filesystem;

const int MAX_HISTORY_SIZE = 50; // Maximum number of presets to keep in history

PresetManager::PresetManager(const Config& config) : _config(config) {}

void PresetManager::load_presets() {
    _all_presets.clear();
    _favorite_presets.clear();
    _history.clear();
    _current_preset_index = -1;
    _history_index = -1;

    // Load all presets from the directory
    try {
        for (const auto & entry : fs::recursive_directory_iterator(_config.presetsDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".milk") {
                _all_presets.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error reading presets directory: " << e.what() << std::endl;
    }

    // Load favorites
    load_favorites();

    // Select an initial random preset if available
    if (!_all_presets.empty()) {
        std::string initial_preset = get_random_preset(_all_presets);
        _history.push_back(initial_preset);
        _history_index = 0;
    }
}

std::string PresetManager::get_next_preset() {
    if (_all_presets.empty()) {
        return "";
    }

    // If we've gone back in history, clear the "future" history before adding a new preset
    if (_history_index != -1 && _history_index < (int)_history.size() - 1) {
        _history.erase(_history.begin() + _history_index + 1, _history.end());
    }

    std::string preset = get_random_preset(_all_presets);
    _history.push_back(preset);
    _history_index++;

    // Cap history size
    if (_history.size() > MAX_HISTORY_SIZE) {
        _history.erase(_history.begin());
        _history_index--; // Adjust index as the first element was removed
    }
    return preset;
}

std::string PresetManager::get_prev_preset() {
    if (_history_index > 0) { // Can go back if not at the very first element
        _history_index--;
        return _history[_history_index];
    }
    return ""; // Cannot go back further
}

std::string PresetManager::get_current_preset() const {
    if (_history_index >= 0 && _history_index < (int)_history.size()) {
        return _history[_history_index];
    }
    return "";
}


void PresetManager::mark_current_preset_as_broken() {
    std::string current_preset = get_current_preset();
    if (current_preset.empty()) {
        return;
    }

    try {
        fs::path source_path(current_preset);
        
        std::string raw_broken_dir = _config.broken_preset_directory;
        fs::path broken_dir_path;
        if (raw_broken_dir.rfind("~", 0) == 0) {
            const char* home_dir = getenv("HOME");
            if (home_dir) {
                std::string path_without_tilde = raw_broken_dir.substr(1);
                if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                    path_without_tilde = path_without_tilde.substr(1);
                }
                broken_dir_path = fs::path(home_dir) / path_without_tilde;
            } else {
                Logger::warn("HOME environment variable not set. Cannot resolve broken preset directory: " + raw_broken_dir);
                broken_dir_path = raw_broken_dir;
            }
        } else {
            broken_dir_path = raw_broken_dir;
        }

        fs::create_directories(broken_dir_path);
        fs::path dest_path = broken_dir_path / source_path.filename();

        fs::rename(source_path, dest_path);

        std::cout << "Moved broken preset to: " << dest_path << std::endl;

        // Remove from all lists
        _all_presets.erase(std::remove(_all_presets.begin(), _all_presets.end(), current_preset), _all_presets.end());
        _favorite_presets.erase(std::remove(_favorite_presets.begin(), _favorite_presets.end(), current_preset), _favorite_presets.end());
        _history.erase(std::remove(_history.begin(), _history.end(), current_preset), _history.end());
        _history_index--;


    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error moving preset file: " << e.what() << std::endl;
    }
}

void PresetManager::toggle_favorite_current_preset() {
    std::string current_preset = get_current_preset();
    if (current_preset.empty()) {
        return;
    }

    auto it = std::find(_favorite_presets.begin(), _favorite_presets.end(), current_preset);
    if (it == _favorite_presets.end()) {
        // Add to favorites
        _favorite_presets.push_back(current_preset);
        std::cout << "Added to favorites: " << current_preset << std::endl;
    } else {
        // Remove from favorites
        _favorite_presets.erase(it);
        std::cout << "Removed from favorites: " << current_preset << std::endl;
    }
    save_favorites();
}


void PresetManager::load_favorites() {
    _favorite_presets.clear();
    std::string raw_path = _config.favoritesFile;
    fs::path resolved_path;

    if (raw_path.rfind("~", 0) == 0) {
        const char* home_dir = getenv("HOME");
        if (home_dir) {
            std::string path_without_tilde = raw_path.substr(1);
            if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                path_without_tilde = path_without_tilde.substr(1);
            }
            resolved_path = fs::path(home_dir) / path_without_tilde;
        } else {
            Logger::warn("HOME environment variable not set. Cannot resolve path: " + raw_path);
            resolved_path = raw_path;
        }
    } else {
        resolved_path = raw_path;
    }

    if (!fs::exists(resolved_path)) {
        return; // No favorites file yet
    }

    std::ifstream favorites_file(resolved_path.string());
    if (!favorites_file.is_open()) {
        Logger::error("Could not open favorites file: " + resolved_path.string());
        return;
    }

    std::string line;
    while (std::getline(favorites_file, line)) {
        if (!line.empty()) {
            _favorite_presets.push_back(line);
        }
    }
}

void PresetManager::save_favorites() {
     std::string raw_path = _config.favoritesFile;
    fs::path resolved_path;

    if (raw_path.rfind("~", 0) == 0) {
        const char* home_dir = getenv("HOME");
        if (home_dir) {
            std::string path_without_tilde = raw_path.substr(1);
            if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                path_without_tilde = path_without_tilde.substr(1);
            }
            resolved_path = fs::path(home_dir) / path_without_tilde;
        } else {
            Logger::warn("HOME environment variable not set. Cannot resolve path: " + raw_path);
            resolved_path = raw_path;
        }
    } else {
        resolved_path = raw_path;
    }

    std::ofstream favorites_file(resolved_path.string());
    if (!favorites_file.is_open()) {
        Logger::error("Could not open favorites file for writing: " + resolved_path.string());
        return;
    }

    for (const auto& preset : _favorite_presets) {
        favorites_file << preset << std::endl;
    }
}

std::string PresetManager::get_random_preset(const std::vector<std::string>& preset_list) {
    if (preset_list.empty()) {
        return "";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, preset_list.size() - 1);

    _current_preset_index = distrib(gen);
    return preset_list[_current_preset_index];
}### FIELE END: src/preset_manager.cpp
## FILE START: src/renderer.cpp
#include "renderer.h"
#include <iostream>

Renderer::Renderer() : _window(nullptr), _context(nullptr), _fbo(0), _fbo_texture(0), _rbo(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(SDL_Window* window, SDL_GLContext* context, Config& config) {
    _window = window;
    _context = context;

    glViewport(0, 0, config.width, config.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after glClearColor: " << error << std::endl;
        return false;
    }

    return create_fbo(config.width, config.height);
}

void Renderer::render(projectm_handle pM) {
    render_to_fbo(pM);
}

void Renderer::cleanup() {
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
    }
    if (_fbo_texture) {
        glDeleteTextures(1, &_fbo_texture);
    }
    if (_rbo) {
        glDeleteRenderbuffers(1, &_rbo);
    }
}

bool Renderer::create_fbo(int width, int height) {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_fbo_texture);
    glBindTexture(GL_TEXTURE_2D, _fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbo_texture, 0);

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Renderer::render_to_fbo(projectm_handle pM) {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projectm_opengl_render_frame(pM);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
### FIELE END: src/renderer.cpp
## FILE START: src/utils/Logger.cpp
#include "utils/Logger.h"

LogLevel Logger::_log_level = LogLevel::INFO;
bool Logger::_verbose_logging = false;
std::mutex Logger::_mutex;
### FIELE END: src/utils/Logger.cpp
## FILE START: src/utils/common.cpp
// src/utils/common.cpp
#include "utils/common.h"
#include <algorithm>
#include <sstream>

std::string sanitize_filename(const std::string &filepath) {
    std::string filename = filepath.substr(filepath.find_last_of('/') + 1);
    size_t dot_pos = filename.rfind('.');
    if (dot_pos != std::string::npos) {
        filename = filename.substr(0, dot_pos);
    }
    std::replace(filename.begin(), filename.end(), '_', ' ');
    return filename;
}

std::string sanitize_for_filename(const std::string& input) {
    std::string output = input;
    // Remove path and extension
    size_t last_slash = output.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        output = output.substr(last_slash + 1);
    }
    size_t last_dot = output.find_last_of('.');
    if (last_dot != std::string::npos) {
        output = output.substr(0, last_dot);
    }
    // Replace invalid characters with underscores
    std::string invalid_chars = "<>:\x22\x2F\x5C|\x3F\x2A\x27\x28\x29"; // "<>":/\\|?*'()"
    for (char& c : output) {
        if (invalid_chars.find(c) != std::string::npos) {
            c = '_';
        }
    }
    return output;
}

std::vector<std::string> wrapText(const std::string &text,
                                  int lineLengthTarget) {
  std::string processedText = text;
  std::replace(processedText.begin(), processedText.end(), '_', ' ');

  std::stringstream ss(processedText);
  std::string word;
  std::string currentLine;
  std::vector<std::string> lines;

  while (ss >> word) {
    if (static_cast<int>(currentLine.length() + word.length() + 1) >
            lineLengthTarget &&
        !currentLine.empty()) {
      lines.push_back(currentLine);
      currentLine = "";
    }
    if (!currentLine.empty()) {
      currentLine += " ";
    }
    currentLine += word;
  }
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }

  return lines;
}
### FIELE END: src/utils/common.cpp
## FILE START: CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer CXX)

# --- CPM.cmake Dependency Manager ---
include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- System Dependencies ---
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavformat libavutil libswscale)

# --- CPM Dependencies ---
CPMAddPackage(
    NAME tomlplusplus
    GITHUB_REPOSITORY marzer/tomlplusplus
    VERSION 3.4.0 # Using a specific version for stability
)

# --- External Project: projectM ---
include(ExternalProject)
ExternalProject_Add(
    projectM_external
    GIT_REPOSITORY https://github.com/projectM-visualizer/projectm.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/projectm_src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/projectm_build"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/projectm_install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_SHARED_LIBS=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)
# Create the install directory for projectM ahead of time to prevent CMake configuration errors
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/lib")
add_library(projectM_lib SHARED IMPORTED)
set_property(TARGET projectM_lib PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/projectm_install/lib/libprojectM-4.so")
set_property(TARGET projectM_lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/projectm_install/include")
add_dependencies(projectM_lib projectM_external)

# --- Source Files ---
file(GLOB_RECURSE APP_SOURCE_FILES "src/*.cpp" "src/**/*.cpp")
set(IMGUI_SOURCE_FILES
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_widgets.cpp"
    "deps/imgui/imgui_tables.cpp"
    "deps/imgui/backends/imgui_impl_sdl2.cpp"
    "deps/imgui/backends/imgui_impl_opengl3.cpp"
)

# --- Executable Target ---
add_executable(AuroraVisualizer ${APP_SOURCE_FILES} ${IMGUI_SOURCE_FILES})

# --- Target Properties (Includes and Linking) ---
target_include_directories(AuroraVisualizer PRIVATE
    "include"
    "deps/imgui"
    "${CMAKE_BINARY_DIR}/projectm_install/include"
    "${FREETYPE_INCLUDE_DIRS}"
    "${CMAKE_BINARY_DIR}/_deps/tomlplusplus-src"
)

target_link_libraries(AuroraVisualizer PRIVATE
    # Project Libraries
    projectM_lib
    tomlplusplus::tomlplusplus

    # System Libraries
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    ${FREETYPE_LIBRARIES}
    OpenGL::GL
    GLEW::GLEW
    glm::glm
    PkgConfig::FFMPEG
)
### FIELE END: CMakeLists.txt
## FILE START: cmake/
### FIELE END: cmake/
## FILE START: cmake/CPM.cmake
# CPM.cmake - CMake's missing package manager
# ===========================================
# See https://github.com/cpm-cmake/CPM.cmake for usage and update instructions.
#
# MIT License
# -----------
#[[
  Copyright (c) 2019-2023 Lars Melchior and contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]]

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Initialize logging prefix
if(NOT CPM_INDENT)
  set(CPM_INDENT
      "CPM:"
      CACHE INTERNAL ""
  )
endif()

if(NOT COMMAND cpm_message)
  function(cpm_message)
    message(${ARGV})
  endfunction()
endif()

if(DEFINED EXTRACTED_CPM_VERSION)
  set(CURRENT_CPM_VERSION "${EXTRACTED_CPM_VERSION}${CPM_DEVELOPMENT}")
else()
  set(CURRENT_CPM_VERSION 0.42.0)
endif()

get_filename_component(CPM_CURRENT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" REALPATH)
if(CPM_DIRECTORY)
  if(NOT CPM_DIRECTORY STREQUAL CPM_CURRENT_DIRECTORY)
    if(CPM_VERSION VERSION_LESS CURRENT_CPM_VERSION)
      message(
        AUTHOR_WARNING
          "${CPM_INDENT} \
A dependency is using a more recent CPM version (${CURRENT_CPM_VERSION}) than the current project (${CPM_VERSION}). \
It is recommended to upgrade CPM to the most recent version. \
See https://github.com/cpm-cmake/CPM.cmake for more information."
      )
    endif()
    if(${CMAKE_VERSION} VERSION_LESS "3.17.0")
      include(FetchContent)
    endif()
    return()
  endif()

  get_property(
    CPM_INITIALIZED GLOBAL ""
    PROPERTY CPM_INITIALIZED
    SET
  )
  if(CPM_INITIALIZED)
    return()
  endif()
endif()

if(CURRENT_CPM_VERSION MATCHES "development-version")
  message(
    WARNING "${CPM_INDENT} Your project is using an unstable development version of CPM.cmake. \
Please update to a recent release if possible. \
See https://github.com/cpm-cmake/CPM.cmake for details."
  )
endif()

set_property(GLOBAL PROPERTY CPM_INITIALIZED true)

macro(cpm_set_policies)
  # the policy allows us to change options without caching
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

  # the policy allows us to change set(CACHE) without caching
  if(POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)
  endif()

  # The policy uses the download time for timestamp, instead of the timestamp in the archive. This
  # allows for proper rebuilds when a projects url changes
  if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
  endif()

  # treat relative git repository paths as being relative to the parent project's remote
  if(POLICY CMP0150)
    cmake_policy(SET CMP0150 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0150 NEW)
  endif()
endmacro()
cpm_set_policies()

option(CPM_USE_LOCAL_PACKAGES "Always try to use `find_package` to get dependencies"
       $ENV{CPM_USE_LOCAL_PACKAGES}
)
option(CPM_LOCAL_PACKAGES_ONLY "Only use `find_package` to get dependencies"
       $ENV{CPM_LOCAL_PACKAGES_ONLY}
)
option(CPM_DOWNLOAD_ALL "Always download dependencies from source" $ENV{CPM_DOWNLOAD_ALL})
option(CPM_DONT_UPDATE_MODULE_PATH "Don't update the module path to allow using find_package"
       $ENV{CPM_DONT_UPDATE_MODULE_PATH}
)
option(CPM_DONT_CREATE_PACKAGE_LOCK "Don't create a package lock file in the binary path"
       $ENV{CPM_DONT_CREATE_PACKAGE_LOCK}
)
option(CPM_INCLUDE_ALL_IN_PACKAGE_LOCK
       "Add all packages added through CPM.cmake to the package lock"
       $ENV{CPM_INCLUDE_ALL_IN_PACKAGE_LOCK}
)
option(CPM_USE_NAMED_CACHE_DIRECTORIES
       "Use additional directory of package name in cache on the most nested level."
       $ENV{CPM_USE_NAMED_CACHE_DIRECTORIES}
)

set(CPM_VERSION
    ${CURRENT_CPM_VERSION}
    CACHE INTERNAL ""
)
set(CPM_DIRECTORY
    ${CPM_CURRENT_DIRECTORY}
    CACHE INTERNAL ""
)
set(CPM_FILE
    ${CMAKE_CURRENT_LIST_FILE}
    CACHE INTERNAL ""
)
set(CPM_PACKAGES
    ""
    CACHE INTERNAL ""
)
set(CPM_DRY_RUN
    OFF
    CACHE INTERNAL "Don't download or configure dependencies (for testing)"
)

if(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_SOURCE_CACHE_DEFAULT $ENV{CPM_SOURCE_CACHE})
else()
  set(CPM_SOURCE_CACHE_DEFAULT OFF)
endif()

set(CPM_SOURCE_CACHE
    ${CPM_SOURCE_CACHE_DEFAULT}
    CACHE PATH "Directory to download CPM dependencies"
)

if(NOT CPM_DONT_UPDATE_MODULE_PATH AND NOT DEFINED CMAKE_FIND_PACKAGE_REDIRECTS_DIR)
  set(CPM_MODULE_PATH
      "${CMAKE_BINARY_DIR}/CPM_modules"
      CACHE INTERNAL ""
  )
  # remove old modules
  file(REMOVE_RECURSE ${CPM_MODULE_PATH})
  file(MAKE_DIRECTORY ${CPM_MODULE_PATH})
  # locally added CPM modules should override global packages
  set(CMAKE_MODULE_PATH "${CPM_MODULE_PATH};${CMAKE_MODULE_PATH}")
endif()

if(NOT CPM_DONT_CREATE_PACKAGE_LOCK)
  set(CPM_PACKAGE_LOCK_FILE
      "${CMAKE_BINARY_DIR}/cpm-package-lock.cmake"
      CACHE INTERNAL ""
  )
  file(WRITE ${CPM_PACKAGE_LOCK_FILE}
       "# CPM Package Lock\n# This file should be committed to version control\n\n"
  )
endif()

include(FetchContent)

# Try to infer package name from git repository uri (path or url)
function(cpm_package_name_from_git_uri URI RESULT)
  if("${URI}" MATCHES "([^/:]+)/?.git/?$")
    set(${RESULT}
        ${CMAKE_MATCH_1}
        PARENT_SCOPE
    )
  else()
    unset(${RESULT} PARENT_SCOPE)
  endif()
endfunction()

# Find the shortest hash that can be used eg, if origin_hash is
# cccb77ae9609d2768ed80dd42cec54f77b1f1455 the following files will be checked, until one is found
# that is either empty (allowing us to assign origin_hash), or whose contents matches ${origin_hash}
#
# * .../cccb.hash
# * .../cccb77ae.hash
# * .../cccb77ae9609.hash
# * .../cccb77ae9609d276.hash
# * etc
#
# We will be able to use a shorter path with very high probability, but in the (rare) event that the
# first couple characters collide, we will check longer and longer substrings.
function(cpm_get_shortest_hash source_cache_dir origin_hash short_hash_output_var)
  # for compatibility with caches populated by a previous version of CPM, check if a directory using
  # the full hash already exists
  if(EXISTS "${source_cache_dir}/${origin_hash}")
    set(${short_hash_output_var}
        "${origin_hash}"
        PARENT_SCOPE
    )
    return()
  endif()

  foreach(len RANGE 4 40 4)
    string(SUBSTRING "${origin_hash}" 0 ${len} short_hash)
    set(hash_lock ${source_cache_dir}/${short_hash}.lock)
    set(hash_fp ${source_cache_dir}/${short_hash}.hash)
    # Take a lock, so we don't have a race condition with another instance of cmake. We will release
    # this lock when we can, however, if there is an error, we want to ensure it gets released on
    # it's own on exit from the function.
    file(LOCK ${hash_lock} GUARD FUNCTION)

    # Load the contents of .../${short_hash}.hash
    file(TOUCH ${hash_fp})
    file(READ ${hash_fp} hash_fp_contents)

    if(hash_fp_contents STREQUAL "")
      # Write the origin hash
      file(WRITE ${hash_fp} ${origin_hash})
      file(LOCK ${hash_lock} RELEASE)
      break()
    elseif(hash_fp_contents STREQUAL origin_hash)
      file(LOCK ${hash_lock} RELEASE)
      break()
    else()
      file(LOCK ${hash_lock} RELEASE)
    endif()
  endforeach()
  set(${short_hash_output_var}
      "${short_hash}"
      PARENT_SCOPE
  )
endfunction()

# Try to infer package name and version from a url
function(cpm_package_name_and_ver_from_url url outName outVer)
  if(url MATCHES "[/\\?]([a-zA-Z0-9_\\.-]+)\\.(tar|tar\\.gz|tar\\.bz2|zip|ZIP)(\\?|/|$)")
    # We matched an archive
    set(filename "${CMAKE_MATCH_1}")

    if(filename MATCHES "([a-zA-Z0-9_\\.-]+)[_-]v?(([0-9]+\\.)*[0-9]+[a-zA-Z0-9]*)")
      # We matched <name>-<version> (ie foo-1.2.3)
      set(${outName}
          "${CMAKE_MATCH_1}"
          PARENT_SCOPE
      )
      set(${outVer}
          "${CMAKE_MATCH_2}"
          PARENT_SCOPE
      )
    elseif(filename MATCHES "(([0-9]+\\.)+[0-9]+[a-zA-Z0-9]*)")
      # We couldn't find a name, but we found a version
      #
      # In many cases (which we don't handle here) the url would look something like
      # `irrelevant/ACTUAL_PACKAGE_NAME/irrelevant/1.2.3.zip`. In such a case we can't possibly
      # distinguish the package name from the irrelevant bits. Moreover if we try to match the
      # package name from the filename, we'd get bogus at best.
      unset(${outName} PARENT_SCOPE)
      set(${outVer}
          "${CMAKE_MATCH_1}"
          PARENT_SCOPE
      )
    else()
      # Boldly assume that the file name is the package name.
      #
      # Yes, something like `irrelevant/ACTUAL_NAME/irrelevant/download.zip` will ruin our day, but
      # such cases should be quite rare. No popular service does this... we think.
      set(${outName}
          "${filename}"
          PARENT_SCOPE
      )
      unset(${outVer} PARENT_SCOPE)
    endif()
  else()
    # No ideas yet what to do with non-archives
    unset(${outName} PARENT_SCOPE)
    unset(${outVer} PARENT_SCOPE)
  endif()
endfunction()

function(cpm_find_package NAME VERSION)
  string(REPLACE " " ";" EXTRA_ARGS "${ARGN}")
  find_package(${NAME} ${VERSION} ${EXTRA_ARGS} QUIET)
  if(${CPM_ARGS_NAME}_FOUND)
    if(DEFINED ${CPM_ARGS_NAME}_VERSION)
      set(VERSION ${${CPM_ARGS_NAME}_VERSION})
    endif()
    cpm_message(STATUS "${CPM_INDENT} Using local package ${CPM_ARGS_NAME}@${VERSION}")
    CPMRegisterPackage(${CPM_ARGS_NAME} "${VERSION}")
    set(CPM_PACKAGE_FOUND
        YES
        PARENT_SCOPE
    )
  else()
    set(CPM_PACKAGE_FOUND
        NO
        PARENT_SCOPE
    )
  endif()
endfunction()

# Create a custom FindXXX.cmake module for a CPM package This prevents `find_package(NAME)` from
# finding the system library
function(cpm_create_module_file Name)
  if(NOT CPM_DONT_UPDATE_MODULE_PATH)
    if(DEFINED CMAKE_FIND_PACKAGE_REDIRECTS_DIR)
      # Redirect find_package calls to the CPM package. This is what FetchContent does when you set
      # OVERRIDE_FIND_PACKAGE. The CMAKE_FIND_PACKAGE_REDIRECTS_DIR works for find_package in CONFIG
      # mode, unlike the Find${Name}.cmake fallback. CMAKE_FIND_PACKAGE_REDIRECTS_DIR is not defined
      # in script mode, or in CMake < 3.24.
      # https://cmake.org/cmake/help/latest/module/FetchContent.html#fetchcontent-find-package-integration-examples
      string(TOLOWER ${Name} NameLower)
      file(WRITE ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/${NameLower}-config.cmake
           "include(\"\${CMAKE_CURRENT_LIST_DIR}/${NameLower}-extra.cmake\" OPTIONAL)\n"
           "include(\"\${CMAKE_CURRENT_LIST_DIR}/${Name}Extra.cmake\" OPTIONAL)\n"
      )
      file(WRITE ${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/${NameLower}-config-version.cmake
           "set(PACKAGE_VERSION_COMPATIBLE TRUE)\n" "set(PACKAGE_VERSION_EXACT TRUE)\n"
      )
    else()
      file(WRITE ${CPM_MODULE_PATH}/Find${Name}.cmake
           "include(\"${CPM_FILE}\")\n${ARGN}\nset(${Name}_FOUND TRUE)"
      )
    endif()
  endif()
endfunction()

# Find a package locally or fallback to CPMAddPackage
function(CPMFindPackage)
  set(oneValueArgs NAME VERSION GIT_TAG FIND_PACKAGE_ARGUMENTS)

  cmake_parse_arguments(CPM_ARGS "" "${oneValueArgs}" "" ${ARGN})

  if(NOT DEFINED CPM_ARGS_VERSION)
    if(DEFINED CPM_ARGS_GIT_TAG)
      cpm_get_version_from_git_tag("${CPM_ARGS_GIT_TAG}" CPM_ARGS_VERSION)
    endif()
  endif()

  set(downloadPackage ${CPM_DOWNLOAD_ALL})
  if(DEFINED CPM_DOWNLOAD_${CPM_ARGS_NAME})
    set(downloadPackage ${CPM_DOWNLOAD_${CPM_ARGS_NAME}})
  elseif(DEFINED ENV{CPM_DOWNLOAD_${CPM_ARGS_NAME}})
    set(downloadPackage $ENV{CPM_DOWNLOAD_${CPM_ARGS_NAME}})
  endif()
  if(downloadPackage)
    CPMAddPackage(${ARGN})
    cpm_export_variables(${CPM_ARGS_NAME})
    return()
  endif()

  cpm_find_package(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" ${CPM_ARGS_FIND_PACKAGE_ARGUMENTS})

  if(NOT CPM_PACKAGE_FOUND)
    CPMAddPackage(${ARGN})
    cpm_export_variables(${CPM_ARGS_NAME})
  endif()

endfunction()

# checks if a package has been added before
function(cpm_check_if_package_already_added CPM_ARGS_NAME CPM_ARGS_VERSION)
  if("${CPM_ARGS_NAME}" IN_LIST CPM_PACKAGES)
    CPMGetPackageVersion(${CPM_ARGS_NAME} CPM_PACKAGE_VERSION)
    if("${CPM_PACKAGE_VERSION}" VERSION_LESS "${CPM_ARGS_VERSION}")
      message(
        WARNING
          "${CPM_INDENT} Requires a newer version of ${CPM_ARGS_NAME} (${CPM_ARGS_VERSION}) than currently included (${CPM_PACKAGE_VERSION})."
      )
    endif()
    cpm_get_fetch_properties(${CPM_ARGS_NAME})
    set(${CPM_ARGS_NAME}_ADDED NO)
    set(CPM_PACKAGE_ALREADY_ADDED
        YES
        PARENT_SCOPE
    )
    cpm_export_variables(${CPM_ARGS_NAME})
  else()
    set(CPM_PACKAGE_ALREADY_ADDED
        NO
        PARENT_SCOPE
    )
  endif()
endfunction()

# Parse the argument of CPMAddPackage in case a single one was provided and convert it to a list of
# arguments which can then be parsed idiomatically. For example gh:foo/bar@1.2.3 will be converted
# to: GITHUB_REPOSITORY;foo/bar;VERSION;1.2.3
function(cpm_parse_add_package_single_arg arg outArgs)
  # Look for a scheme
  if("${arg}" MATCHES "^([a-zA-Z]+):(.+)$")
    string(TOLOWER "${CMAKE_MATCH_1}" scheme)
    set(uri "${CMAKE_MATCH_2}")

    # Check for CPM-specific schemes
    if(scheme STREQUAL "gh")
      set(out "GITHUB_REPOSITORY;${uri}")
      set(packageType "git")
    elseif(scheme STREQUAL "gl")
      set(out "GITLAB_REPOSITORY;${uri}")
      set(packageType "git")
    elseif(scheme STREQUAL "bb")
      set(out "BITBUCKET_REPOSITORY;${uri}")
      set(packageType "git")
      # A CPM-specific scheme was not found. Looks like this is a generic URL so try to determine
      # type
    elseif(arg MATCHES ".git/?(@|#|$)")
      set(out "GIT_REPOSITORY;${arg}")
      set(packageType "git")
    else()
      # Fall back to a URL
      set(out "URL;${arg}")
      set(packageType "archive")

      # We could also check for SVN since FetchContent supports it, but SVN is so rare these days.
      # We just won't bother with the additional complexity it will induce in this function. SVN is
      # done by multi-arg
    endif()
  else()
    if(arg MATCHES ".git/?(@|#|$)")
      set(out "GIT_REPOSITORY;${arg}")
      set(packageType "git")
    else()
      # Give up
      message(FATAL_ERROR "${CPM_INDENT} Can't determine package type of '${arg}'")
    endif()
  endif()

  # For all packages we interpret @... as version. Only replace the last occurrence. Thus URIs
  # containing '@' can be used
  string(REGEX REPLACE "@([^@]+)$" ";VERSION;\\1" out "${out}")

  # Parse the rest according to package type
  if(packageType STREQUAL "git")
    # For git repos we interpret #... as a tag or branch or commit hash
    string(REGEX REPLACE "#([^#]+)$" ";GIT_TAG;\\1" out "${out}")
  elseif(packageType STREQUAL "archive")
    # For archives we interpret #... as a URL hash.
    string(REGEX REPLACE "#([^#]+)$" ";URL_HASH;\\1" out "${out}")
    # We don't try to parse the version if it's not provided explicitly. cpm_get_version_from_url
    # should do this at a later point
  else()
    # We should never get here. This is an assertion and hitting it means there's a problem with the
    # code above. A packageType was set, but not handled by this if-else.
    message(FATAL_ERROR "${CPM_INDENT} Unsupported package type '${packageType}' of '${arg}'")
  endif()

  set(${outArgs}
      ${out}
      PARENT_SCOPE
  )
endfunction()

# Check that the working directory for a git repo is clean
function(cpm_check_git_working_dir_is_clean repoPath gitTag isClean)

  find_package(Git REQUIRED)

  if(NOT GIT_EXECUTABLE)
    # No git executable, assume directory is clean
    set(${isClean}
        TRUE
        PARENT_SCOPE
    )
    return()
  endif()

  # check for uncommitted changes
  execute_process(
    COMMAND ${GIT_EXECUTABLE} status --porcelain
    RESULT_VARIABLE resultGitStatus
    OUTPUT_VARIABLE repoStatus
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    WORKING_DIRECTORY ${repoPath}
  )
  if(resultGitStatus)
    # not supposed to happen, assume clean anyway
    message(WARNING "${CPM_INDENT} Calling git status on folder ${repoPath} failed")
    set(${isClean}
        TRUE
        PARENT_SCOPE
    )
    return()
  endif()

  if(NOT "${repoStatus}" STREQUAL "")
    set(${isClean}
        FALSE
        PARENT_SCOPE
    )
    return()
  endif()

  # check for committed changes
  execute_process(
    COMMAND ${GIT_EXECUTABLE} diff -s --exit-code ${gitTag}
    RESULT_VARIABLE resultGitDiff
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_QUIET
    WORKING_DIRECTORY ${repoPath}
  )

  if(${resultGitDiff} EQUAL 0)
    set(${isClean}
        TRUE
        PARENT_SCOPE
    )
  else()
    set(${isClean}
        FALSE
        PARENT_SCOPE
    )
  endif()

endfunction()

# Add PATCH_COMMAND to CPM_ARGS_UNPARSED_ARGUMENTS. This method consumes a list of files in ARGN
# then generates a `PATCH_COMMAND` appropriate for `ExternalProject_Add()`. This command is appended
# to the parent scope's `CPM_ARGS_UNPARSED_ARGUMENTS`.
function(cpm_add_patches)
  # Return if no patch files are supplied.
  if(NOT ARGN)
    return()
  endif()

  # Find the patch program.
  find_program(PATCH_EXECUTABLE patch)
  if(CMAKE_HOST_WIN32 AND NOT PATCH_EXECUTABLE)
    # The Windows git executable is distributed with patch.exe. Find the path to the executable, if
    # it exists, then search `../usr/bin` and `../../usr/bin` for patch.exe.
    find_package(Git QUIET)
    if(GIT_EXECUTABLE)
      get_filename_component(extra_search_path ${GIT_EXECUTABLE} DIRECTORY)
      get_filename_component(extra_search_path_1up ${extra_search_path} DIRECTORY)
      get_filename_component(extra_search_path_2up ${extra_search_path_1up} DIRECTORY)
      find_program(
        PATCH_EXECUTABLE patch HINTS "${extra_search_path_1up}/usr/bin"
                                     "${extra_search_path_2up}/usr/bin"
      )
    endif()
  endif()
  if(NOT PATCH_EXECUTABLE)
    message(FATAL_ERROR "Couldn't find `patch` executable to use with PATCHES keyword.")
  endif()

  # Create a temporary
  set(temp_list ${CPM_ARGS_UNPARSED_ARGUMENTS})

  # Ensure each file exists (or error out) and add it to the list.
  set(first_item True)
  foreach(PATCH_FILE ${ARGN})
    # Make sure the patch file exists, if we can't find it, try again in the current directory.
    if(NOT EXISTS "${PATCH_FILE}")
      if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${PATCH_FILE}")
        message(FATAL_ERROR "Couldn't find patch file: '${PATCH_FILE}'")
      endif()
      set(PATCH_FILE "${CMAKE_CURRENT_LIST_DIR}/${PATCH_FILE}")
    endif()

    # Convert to absolute path for use with patch file command.
    get_filename_component(PATCH_FILE "${PATCH_FILE}" ABSOLUTE)

    # The first patch entry must be preceded by "PATCH_COMMAND" while the following items are
    # preceded by "&&".
    if(first_item)
      set(first_item False)
      list(APPEND temp_list "PATCH_COMMAND")
    else()
      list(APPEND temp_list "&&")
    endif()
    # Add the patch command to the list
    list(APPEND temp_list "${PATCH_EXECUTABLE}" "-p1" "<" "${PATCH_FILE}")
  endforeach()

  # Move temp out into parent scope.
  set(CPM_ARGS_UNPARSED_ARGUMENTS
      ${temp_list}
      PARENT_SCOPE
  )

endfunction()

# method to overwrite internal FetchContent properties, to allow using CPM.cmake to overload
# FetchContent calls. As these are internal cmake properties, this method should be used carefully
# and may need modification in future CMake versions. Source:
# https://github.com/Kitware/CMake/blob/dc3d0b5a0a7d26d43d6cfeb511e224533b5d188f/Modules/FetchContent.cmake#L1152
function(cpm_override_fetchcontent contentName)
  cmake_parse_arguments(PARSE_ARGV 1 arg "" "SOURCE_DIR;BINARY_DIR" "")
  if(NOT "${arg_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(FATAL_ERROR "${CPM_INDENT} Unsupported arguments: ${arg_UNPARSED_ARGUMENTS}")
  endif()

  string(TOLOWER ${contentName} contentNameLower)
  set(prefix "_FetchContent_${contentNameLower}")

  set(propertyName "${prefix}_sourceDir")
  define_property(
    GLOBAL
    PROPERTY ${propertyName}
    BRIEF_DOCS "Internal implementation detail of FetchContent_Populate()"
    FULL_DOCS "Details used by FetchContent_Populate() for ${contentName}"
  )
  set_property(GLOBAL PROPERTY ${propertyName} "${arg_SOURCE_DIR}")

  set(propertyName "${prefix}_binaryDir")
  define_property(
    GLOBAL
    PROPERTY ${propertyName}
    BRIEF_DOCS "Internal implementation detail of FetchContent_Populate()"
    FULL_DOCS "Details used by FetchContent_Populate() for ${contentName}"
  )
  set_property(GLOBAL PROPERTY ${propertyName} "${arg_BINARY_DIR}")

  set(propertyName "${prefix}_populated")
  define_property(
    GLOBAL
    PROPERTY ${propertyName}
    BRIEF_DOCS "Internal implementation detail of FetchContent_Populate()"
    FULL_DOCS "Details used by FetchContent_Populate() for ${contentName}"
  )
  set_property(GLOBAL PROPERTY ${propertyName} TRUE)
endfunction()

# Download and add a package from source
function(CPMAddPackage)
  cpm_set_policies()

  set(oneValueArgs
      NAME
      FORCE
      VERSION
      GIT_TAG
      DOWNLOAD_ONLY
      GITHUB_REPOSITORY
      GITLAB_REPOSITORY
      BITBUCKET_REPOSITORY
      GIT_REPOSITORY
      SOURCE_DIR
      FIND_PACKAGE_ARGUMENTS
      NO_CACHE
      SYSTEM
      GIT_SHALLOW
      EXCLUDE_FROM_ALL
      SOURCE_SUBDIR
      CUSTOM_CACHE_KEY
  )

  set(multiValueArgs URL OPTIONS DOWNLOAD_COMMAND PATCHES)

  list(LENGTH ARGN argnLength)

  # Parse single shorthand argument
  if(argnLength EQUAL 1)
    cpm_parse_add_package_single_arg("${ARGN}" ARGN)

    # The shorthand syntax implies EXCLUDE_FROM_ALL and SYSTEM
    set(ARGN "${ARGN};EXCLUDE_FROM_ALL;YES;SYSTEM;YES;")

    # Parse URI shorthand argument
  elseif(argnLength GREATER 1 AND "${ARGV0}" STREQUAL "URI")
    list(REMOVE_AT ARGN 0 1) # remove "URI gh:<...>@version#tag"
    cpm_parse_add_package_single_arg("${ARGV1}" ARGV0)

    set(ARGN "${ARGV0};EXCLUDE_FROM_ALL;YES;SYSTEM;YES;${ARGN}")
  endif()

  cmake_parse_arguments(CPM_ARGS "" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

  # Set default values for arguments
  if(NOT DEFINED CPM_ARGS_VERSION)
    if(DEFINED CPM_ARGS_GIT_TAG)
      cpm_get_version_from_git_tag("${CPM_ARGS_GIT_TAG}" CPM_ARGS_VERSION)
    endif()
  endif()

  if(CPM_ARGS_DOWNLOAD_ONLY)
    set(DOWNLOAD_ONLY ${CPM_ARGS_DOWNLOAD_ONLY})
  else()
    set(DOWNLOAD_ONLY NO)
  endif()

  if(DEFINED CPM_ARGS_GITHUB_REPOSITORY)
    set(CPM_ARGS_GIT_REPOSITORY "https://github.com/${CPM_ARGS_GITHUB_REPOSITORY}.git")
  elseif(DEFINED CPM_ARGS_GITLAB_REPOSITORY)
    set(CPM_ARGS_GIT_REPOSITORY "https://gitlab.com/${CPM_ARGS_GITLAB_REPOSITORY}.git")
  elseif(DEFINED CPM_ARGS_BITBUCKET_REPOSITORY)
    set(CPM_ARGS_GIT_REPOSITORY "https://bitbucket.org/${CPM_ARGS_BITBUCKET_REPOSITORY}.git")
  endif()

  if(DEFINED CPM_ARGS_GIT_REPOSITORY)
    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_REPOSITORY ${CPM_ARGS_GIT_REPOSITORY})
    if(NOT DEFINED CPM_ARGS_GIT_TAG)
      set(CPM_ARGS_GIT_TAG v${CPM_ARGS_VERSION})
    endif()

    # If a name wasn't provided, try to infer it from the git repo
    if(NOT DEFINED CPM_ARGS_NAME)
      cpm_package_name_from_git_uri(${CPM_ARGS_GIT_REPOSITORY} CPM_ARGS_NAME)
    endif()
  endif()

  set(CPM_SKIP_FETCH FALSE)

  if(DEFINED CPM_ARGS_GIT_TAG)
    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_TAG ${CPM_ARGS_GIT_TAG})
    # If GIT_SHALLOW is explicitly specified, honor the value.
    if(DEFINED CPM_ARGS_GIT_SHALLOW)
      list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_SHALLOW ${CPM_ARGS_GIT_SHALLOW})
    endif()
  endif()

  if(DEFINED CPM_ARGS_URL)
    # If a name or version aren't provided, try to infer them from the URL
    list(GET CPM_ARGS_URL 0 firstUrl)
    cpm_package_name_and_ver_from_url(${firstUrl} nameFromUrl verFromUrl)
    # If we fail to obtain name and version from the first URL, we could try other URLs if any.
    # However multiple URLs are expected to be quite rare, so for now we won't bother.

    # If the caller provided their own name and version, they trump the inferred ones.
    if(NOT DEFINED CPM_ARGS_NAME)
      set(CPM_ARGS_NAME ${nameFromUrl})
    endif()
    if(NOT DEFINED CPM_ARGS_VERSION)
      set(CPM_ARGS_VERSION ${verFromUrl})
    endif()

    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS URL "${CPM_ARGS_URL}")
  endif()

  # Check for required arguments

  if(NOT DEFINED CPM_ARGS_NAME)
    message(
      FATAL_ERROR
        "${CPM_INDENT} 'NAME' was not provided and couldn't be automatically inferred for package added with arguments: '${ARGN}'"
    )
  endif()

  # Check if package has been added before
  cpm_check_if_package_already_added(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}")
  if(CPM_PACKAGE_ALREADY_ADDED)
    cpm_export_variables(${CPM_ARGS_NAME})
    return()
  endif()

  # Check for manual overrides
  if(NOT CPM_ARGS_FORCE AND NOT "${CPM_${CPM_ARGS_NAME}_SOURCE}" STREQUAL "")
    set(PACKAGE_SOURCE ${CPM_${CPM_ARGS_NAME}_SOURCE})
    set(CPM_${CPM_ARGS_NAME}_SOURCE "")
    CPMAddPackage(
      NAME "${CPM_ARGS_NAME}"
      SOURCE_DIR "${PACKAGE_SOURCE}"
      EXCLUDE_FROM_ALL "${CPM_ARGS_EXCLUDE_FROM_ALL}"
      SYSTEM "${CPM_ARGS_SYSTEM}"
      PATCHES "${CPM_ARGS_PATCHES}"
      OPTIONS "${CPM_ARGS_OPTIONS}"
      SOURCE_SUBDIR "${CPM_ARGS_SOURCE_SUBDIR}"
      DOWNLOAD_ONLY "${DOWNLOAD_ONLY}"
      FORCE True
    )
    cpm_export_variables(${CPM_ARGS_NAME})
    return()
  endif()

  # Check for available declaration
  if(NOT CPM_ARGS_FORCE AND NOT "${CPM_DECLARATION_${CPM_ARGS_NAME}}" STREQUAL "")
    set(declaration ${CPM_DECLARATION_${CPM_ARGS_NAME}})
    set(CPM_DECLARATION_${CPM_ARGS_NAME} "")
    CPMAddPackage(${declaration})
    cpm_export_variables(${CPM_ARGS_NAME})
    # checking again to ensure version and option compatibility
    cpm_check_if_package_already_added(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}")
    return()
  endif()

  if(NOT CPM_ARGS_FORCE)
    if(CPM_USE_LOCAL_PACKAGES OR CPM_LOCAL_PACKAGES_ONLY)
      cpm_find_package(${CPM_ARGS_NAME} "${CPM_ARGS_VERSION}" ${CPM_ARGS_FIND_PACKAGE_ARGUMENTS})

      if(CPM_PACKAGE_FOUND)
        cpm_export_variables(${CPM_ARGS_NAME})
        return()
      endif()

      if(CPM_LOCAL_PACKAGES_ONLY)
        message(
          SEND_ERROR
            "${CPM_INDENT} ${CPM_ARGS_NAME} not found via find_package(${CPM_ARGS_NAME} ${CPM_ARGS_VERSION})"
        )
      endif()
    endif()
  endif()

  CPMRegisterPackage("${CPM_ARGS_NAME}" "${CPM_ARGS_VERSION}")

  if(DEFINED CPM_ARGS_GIT_TAG)
    set(PACKAGE_INFO "${CPM_ARGS_GIT_TAG}")
  elseif(DEFINED CPM_ARGS_SOURCE_DIR)
    set(PACKAGE_INFO "${CPM_ARGS_SOURCE_DIR}")
  else()
    set(PACKAGE_INFO "${CPM_ARGS_VERSION}")
  endif()

  if(DEFINED FETCHCONTENT_BASE_DIR)
    # respect user's FETCHCONTENT_BASE_DIR if set
    set(CPM_FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR})
  else()
    set(CPM_FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)
  endif()

  cpm_add_patches(${CPM_ARGS_PATCHES})

  if(DEFINED CPM_ARGS_DOWNLOAD_COMMAND)
    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS DOWNLOAD_COMMAND ${CPM_ARGS_DOWNLOAD_COMMAND})
  elseif(DEFINED CPM_ARGS_SOURCE_DIR)
    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS SOURCE_DIR ${CPM_ARGS_SOURCE_DIR})
    if(NOT IS_ABSOLUTE ${CPM_ARGS_SOURCE_DIR})
      # Expand `CPM_ARGS_SOURCE_DIR` relative path. This is important because EXISTS doesn't work
      # for relative paths.
      get_filename_component(
        source_directory ${CPM_ARGS_SOURCE_DIR} REALPATH BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}
      )
    else()
      set(source_directory ${CPM_ARGS_SOURCE_DIR})
    endif()
    if(NOT EXISTS ${source_directory})
      string(TOLOWER ${CPM_ARGS_NAME} lower_case_name)
      # remove timestamps so CMake will re-download the dependency
      file(REMOVE_RECURSE "${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-subbuild")
    endif()
  elseif(CPM_SOURCE_CACHE AND NOT CPM_ARGS_NO_CACHE)
    string(TOLOWER ${CPM_ARGS_NAME} lower_case_name)
    set(origin_parameters ${CPM_ARGS_UNPARSED_ARGUMENTS})
    list(SORT origin_parameters)
    if(CPM_ARGS_CUSTOM_CACHE_KEY)
      # Application set a custom unique directory name
      set(download_directory ${CPM_SOURCE_CACHE}/${lower_case_name}/${CPM_ARGS_CUSTOM_CACHE_KEY})
    elseif(CPM_USE_NAMED_CACHE_DIRECTORIES)
      string(SHA1 origin_hash "${origin_parameters};NEW_CACHE_STRUCTURE_TAG")
      cpm_get_shortest_hash(
        "${CPM_SOURCE_CACHE}/${lower_case_name}" # source cache directory
        "${origin_hash}" # Input hash
        origin_hash # Computed hash
      )
      set(download_directory ${CPM_SOURCE_CACHE}/${lower_case_name}/${origin_hash}/${CPM_ARGS_NAME})
    else()
      string(SHA1 origin_hash "${origin_parameters}")
      cpm_get_shortest_hash(
        "${CPM_SOURCE_CACHE}/${lower_case_name}" # source cache directory
        "${origin_hash}" # Input hash
        origin_hash # Computed hash
      )
      set(download_directory ${CPM_SOURCE_CACHE}/${lower_case_name}/${origin_hash})
    endif()
    # Expand `download_directory` relative path. This is important because EXISTS doesn't work for
    # relative paths.
    get_filename_component(download_directory ${download_directory} ABSOLUTE)
    list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS SOURCE_DIR ${download_directory})

    if(CPM_SOURCE_CACHE)
      file(LOCK ${download_directory}/../cmake.lock)
    endif()

    if(EXISTS ${download_directory})
      if(CPM_SOURCE_CACHE)
        file(LOCK ${download_directory}/../cmake.lock RELEASE)
      endif()

      cpm_store_fetch_properties(
        ${CPM_ARGS_NAME} "${download_directory}"
        "${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-build"
      )
      cpm_get_fetch_properties("${CPM_ARGS_NAME}")

      if(DEFINED CPM_ARGS_GIT_TAG AND NOT (PATCH_COMMAND IN_LIST CPM_ARGS_UNPARSED_ARGUMENTS))
        # warn if cache has been changed since checkout
        cpm_check_git_working_dir_is_clean(${download_directory} ${CPM_ARGS_GIT_TAG} IS_CLEAN)
        if(NOT ${IS_CLEAN})
          message(
            WARNING "${CPM_INDENT} Cache for ${CPM_ARGS_NAME} (${download_directory}) is dirty"
          )
        endif()
      endif()

      cpm_add_subdirectory(
        "${CPM_ARGS_NAME}"
        "${DOWNLOAD_ONLY}"
        "${${CPM_ARGS_NAME}_SOURCE_DIR}/${CPM_ARGS_SOURCE_SUBDIR}"
        "${${CPM_ARGS_NAME}_BINARY_DIR}"
        "${CPM_ARGS_EXCLUDE_FROM_ALL}"
        "${CPM_ARGS_SYSTEM}"
        "${CPM_ARGS_OPTIONS}"
      )
      set(PACKAGE_INFO "${PACKAGE_INFO} at ${download_directory}")

      # As the source dir is already cached/populated, we override the call to FetchContent.
      set(CPM_SKIP_FETCH TRUE)
      cpm_override_fetchcontent(
        "${lower_case_name}" SOURCE_DIR "${${CPM_ARGS_NAME}_SOURCE_DIR}/${CPM_ARGS_SOURCE_SUBDIR}"
        BINARY_DIR "${${CPM_ARGS_NAME}_BINARY_DIR}"
      )

    else()
      # Enable shallow clone when GIT_TAG is not a commit hash. Our guess may not be accurate, but
      # it should guarantee no commit hash get mis-detected.
      if(NOT DEFINED CPM_ARGS_GIT_SHALLOW)
        cpm_is_git_tag_commit_hash("${CPM_ARGS_GIT_TAG}" IS_HASH)
        if(NOT ${IS_HASH})
          list(APPEND CPM_ARGS_UNPARSED_ARGUMENTS GIT_SHALLOW TRUE)
        endif()
      endif()

      # remove timestamps so CMake will re-download the dependency
      file(REMOVE_RECURSE ${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-subbuild)
      set(PACKAGE_INFO "${PACKAGE_INFO} to ${download_directory}")
    endif()
  endif()

  if(NOT "${DOWNLOAD_ONLY}")
    cpm_create_module_file(${CPM_ARGS_NAME} "CPMAddPackage(\"${ARGN}\")")
  endif()

  if(CPM_PACKAGE_LOCK_ENABLED)
    if((CPM_ARGS_VERSION AND NOT CPM_ARGS_SOURCE_DIR) OR CPM_INCLUDE_ALL_IN_PACKAGE_LOCK)
      cpm_add_to_package_lock(${CPM_ARGS_NAME} "${ARGN}")
    elseif(CPM_ARGS_SOURCE_DIR)
      cpm_add_comment_to_package_lock(${CPM_ARGS_NAME} "local directory")
    else()
      cpm_add_comment_to_package_lock(${CPM_ARGS_NAME} "${ARGN}")
    endif()
  endif()

  cpm_message(
    STATUS "${CPM_INDENT} Adding package ${CPM_ARGS_NAME}@${CPM_ARGS_VERSION} (${PACKAGE_INFO})"
  )

  if(NOT CPM_SKIP_FETCH)
    # CMake 3.28 added EXCLUDE, SYSTEM (3.25), and SOURCE_SUBDIR (3.18) to FetchContent_Declare.
    # Calling FetchContent_MakeAvailable will then internally forward these options to
    # add_subdirectory. Up until these changes, we had to call FetchContent_Populate and
    # add_subdirectory separately, which is no longer necessary and has been deprecated as of 3.30.
    # A Bug in CMake prevents us to use the non-deprecated functions until 3.30.3.
    set(fetchContentDeclareExtraArgs "")
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.30.3")
      if(${CPM_ARGS_EXCLUDE_FROM_ALL})
        list(APPEND fetchContentDeclareExtraArgs EXCLUDE_FROM_ALL)
      endif()
      if(${CPM_ARGS_SYSTEM})
        list(APPEND fetchContentDeclareExtraArgs SYSTEM)
      endif()
      if(DEFINED CPM_ARGS_SOURCE_SUBDIR)
        list(APPEND fetchContentDeclareExtraArgs SOURCE_SUBDIR ${CPM_ARGS_SOURCE_SUBDIR})
      endif()
      # For CMake version <3.28 OPTIONS are parsed in cpm_add_subdirectory
      if(CPM_ARGS_OPTIONS AND NOT DOWNLOAD_ONLY)
        foreach(OPTION ${CPM_ARGS_OPTIONS})
          cpm_parse_option("${OPTION}")
          set(${OPTION_KEY} "${OPTION_VALUE}")
        endforeach()
      endif()
    endif()
    cpm_declare_fetch(
      "${CPM_ARGS_NAME}" ${fetchContentDeclareExtraArgs} "${CPM_ARGS_UNPARSED_ARGUMENTS}"
    )

    cpm_fetch_package("${CPM_ARGS_NAME}" ${DOWNLOAD_ONLY} populated ${CPM_ARGS_UNPARSED_ARGUMENTS})
    if(CPM_SOURCE_CACHE AND download_directory)
      file(LOCK ${download_directory}/../cmake.lock RELEASE)
    endif()
    if(${populated} AND ${CMAKE_VERSION} VERSION_LESS "3.30.3")
      cpm_add_subdirectory(
        "${CPM_ARGS_NAME}"
        "${DOWNLOAD_ONLY}"
        "${${CPM_ARGS_NAME}_SOURCE_DIR}/${CPM_ARGS_SOURCE_SUBDIR}"
        "${${CPM_ARGS_NAME}_BINARY_DIR}"
        "${CPM_ARGS_EXCLUDE_FROM_ALL}"
        "${CPM_ARGS_SYSTEM}"
        "${CPM_ARGS_OPTIONS}"
      )
    endif()
    cpm_get_fetch_properties("${CPM_ARGS_NAME}")
  endif()

  set(${CPM_ARGS_NAME}_ADDED YES)
  cpm_export_variables("${CPM_ARGS_NAME}")
endfunction()

# Fetch a previously declared package
macro(CPMGetPackage Name)
  if(DEFINED "CPM_DECLARATION_${Name}")
    CPMAddPackage(NAME ${Name})
  else()
    message(SEND_ERROR "${CPM_INDENT} Cannot retrieve package ${Name}: no declaration available")
  endif()
endmacro()

# export variables available to the caller to the parent scope expects ${CPM_ARGS_NAME} to be set
macro(cpm_export_variables name)
  set(${name}_SOURCE_DIR
      "${${name}_SOURCE_DIR}"
      PARENT_SCOPE
  )
  set(${name}_BINARY_DIR
      "${${name}_BINARY_DIR}"
      PARENT_SCOPE
  )
  set(${name}_ADDED
      "${${name}_ADDED}"
      PARENT_SCOPE
  )
  set(CPM_LAST_PACKAGE_NAME
      "${name}"
      PARENT_SCOPE
  )
endmacro()

# declares a package, so that any call to CPMAddPackage for the package name will use these
# arguments instead. Previous declarations will not be overridden.
macro(CPMDeclarePackage Name)
  if(NOT DEFINED "CPM_DECLARATION_${Name}")
    set("CPM_DECLARATION_${Name}" "${ARGN}")
  endif()
endmacro()

function(cpm_add_to_package_lock Name)
  if(NOT CPM_DONT_CREATE_PACKAGE_LOCK)
    cpm_prettify_package_arguments(PRETTY_ARGN false ${ARGN})
    file(APPEND ${CPM_PACKAGE_LOCK_FILE} "# ${Name}\nCPMDeclarePackage(${Name}\n${PRETTY_ARGN})\n")
  endif()
endfunction()

function(cpm_add_comment_to_package_lock Name)
  if(NOT CPM_DONT_CREATE_PACKAGE_LOCK)
    cpm_prettify_package_arguments(PRETTY_ARGN true ${ARGN})
    file(APPEND ${CPM_PACKAGE_LOCK_FILE}
         "# ${Name} (unversioned)\n# CPMDeclarePackage(${Name}\n${PRETTY_ARGN}#)\n"
    )
  endif()
endfunction()

# includes the package lock file if it exists and creates a target `cpm-update-package-lock` to
# update it
macro(CPMUsePackageLock file)
  if(NOT CPM_DONT_CREATE_PACKAGE_LOCK)
    get_filename_component(CPM_ABSOLUTE_PACKAGE_LOCK_PATH ${file} ABSOLUTE)
    if(EXISTS ${CPM_ABSOLUTE_PACKAGE_LOCK_PATH})
      include(${CPM_ABSOLUTE_PACKAGE_LOCK_PATH})
    endif()
    if(NOT TARGET cpm-update-package-lock)
      add_custom_target(
        cpm-update-package-lock COMMAND ${CMAKE_COMMAND} -E copy ${CPM_PACKAGE_LOCK_FILE}
                                        ${CPM_ABSOLUTE_PACKAGE_LOCK_PATH}
      )
    endif()
    set(CPM_PACKAGE_LOCK_ENABLED true)
  endif()
endmacro()

# registers a package that has been added to CPM
function(CPMRegisterPackage PACKAGE VERSION)
  list(APPEND CPM_PACKAGES ${PACKAGE})
  set(CPM_PACKAGES
      ${CPM_PACKAGES}
      CACHE INTERNAL ""
  )
  set("CPM_PACKAGE_${PACKAGE}_VERSION"
      ${VERSION}
      CACHE INTERNAL ""
  )
endfunction()

# retrieve the current version of the package to ${OUTPUT}
function(CPMGetPackageVersion PACKAGE OUTPUT)
  set(${OUTPUT}
      "${CPM_PACKAGE_${PACKAGE}_VERSION}"
      PARENT_SCOPE
  )
endfunction()

# declares a package in FetchContent_Declare
function(cpm_declare_fetch PACKAGE)
  if(${CPM_DRY_RUN})
    cpm_message(STATUS "${CPM_INDENT} Package not declared (dry run)")
    return()
  endif()

  FetchContent_Declare(${PACKAGE} ${ARGN})
endfunction()

# returns properties for a package previously defined by cpm_declare_fetch
function(cpm_get_fetch_properties PACKAGE)
  if(${CPM_DRY_RUN})
    return()
  endif()

  set(${PACKAGE}_SOURCE_DIR
      "${CPM_PACKAGE_${PACKAGE}_SOURCE_DIR}"
      PARENT_SCOPE
  )
  set(${PACKAGE}_BINARY_DIR
      "${CPM_PACKAGE_${PACKAGE}_BINARY_DIR}"
      PARENT_SCOPE
  )
endfunction()

function(cpm_store_fetch_properties PACKAGE source_dir binary_dir)
  if(${CPM_DRY_RUN})
    return()
  endif()

  set(CPM_PACKAGE_${PACKAGE}_SOURCE_DIR
      "${source_dir}"
      CACHE INTERNAL ""
  )
  set(CPM_PACKAGE_${PACKAGE}_BINARY_DIR
      "${binary_dir}"
      CACHE INTERNAL ""
  )
endfunction()

# adds a package as a subdirectory if viable, according to provided options
function(
  cpm_add_subdirectory
  PACKAGE
  DOWNLOAD_ONLY
  SOURCE_DIR
  BINARY_DIR
  EXCLUDE
  SYSTEM
  OPTIONS
)

  if(NOT DOWNLOAD_ONLY AND EXISTS ${SOURCE_DIR}/CMakeLists.txt)
    set(addSubdirectoryExtraArgs "")
    if(EXCLUDE)
      list(APPEND addSubdirectoryExtraArgs EXCLUDE_FROM_ALL)
    endif()
    if("${SYSTEM}" AND "${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.25")
      # https://cmake.org/cmake/help/latest/prop_dir/SYSTEM.html#prop_dir:SYSTEM
      list(APPEND addSubdirectoryExtraArgs SYSTEM)
    endif()
    if(OPTIONS)
      foreach(OPTION ${OPTIONS})
        cpm_parse_option("${OPTION}")
        set(${OPTION_KEY} "${OPTION_VALUE}")
      endforeach()
    endif()
    set(CPM_OLD_INDENT "${CPM_INDENT}")
    set(CPM_INDENT "${CPM_INDENT} ${PACKAGE}:")
    add_subdirectory(${SOURCE_DIR} ${BINARY_DIR} ${addSubdirectoryExtraArgs})
    set(CPM_INDENT "${CPM_OLD_INDENT}")
  endif()
endfunction()

# downloads a previously declared package via FetchContent and exports the variables
# `${PACKAGE}_SOURCE_DIR` and `${PACKAGE}_BINARY_DIR` to the parent scope
function(cpm_fetch_package PACKAGE DOWNLOAD_ONLY populated)
  set(${populated}
      FALSE
      PARENT_SCOPE
  )
  if(${CPM_DRY_RUN})
    cpm_message(STATUS "${CPM_INDENT} Package ${PACKAGE} not fetched (dry run)")
    return()
  endif()

  FetchContent_GetProperties(${PACKAGE})

  string(TOLOWER "${PACKAGE}" lower_case_name)

  if(NOT ${lower_case_name}_POPULATED)
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.30.3")
      if(DOWNLOAD_ONLY)
        # MakeAvailable will call add_subdirectory internally which is not what we want when
        # DOWNLOAD_ONLY is set. Populate will only download the dependency without adding it to the
        # build
        FetchContent_Populate(
          ${PACKAGE}
          SOURCE_DIR "${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-src"
          BINARY_DIR "${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-build"
          SUBBUILD_DIR "${CPM_FETCHCONTENT_BASE_DIR}/${lower_case_name}-subbuild"
          ${ARGN}
        )
      else()
        FetchContent_MakeAvailable(${PACKAGE})
      endif()
    else()
      FetchContent_Populate(${PACKAGE})
    endif()
    set(${populated}
        TRUE
        PARENT_SCOPE
    )
  endif()

  cpm_store_fetch_properties(
    ${CPM_ARGS_NAME} ${${lower_case_name}_SOURCE_DIR} ${${lower_case_name}_BINARY_DIR}
  )

  set(${PACKAGE}_SOURCE_DIR
      ${${lower_case_name}_SOURCE_DIR}
      PARENT_SCOPE
  )
  set(${PACKAGE}_BINARY_DIR
      ${${lower_case_name}_BINARY_DIR}
      PARENT_SCOPE
  )
endfunction()

# splits a package option
function(cpm_parse_option OPTION)
  string(REGEX MATCH "^[^ ]+" OPTION_KEY "${OPTION}")
  string(LENGTH "${OPTION}" OPTION_LENGTH)
  string(LENGTH "${OPTION_KEY}" OPTION_KEY_LENGTH)
  if(OPTION_KEY_LENGTH STREQUAL OPTION_LENGTH)
    # no value for key provided, assume user wants to set option to "ON"
    set(OPTION_VALUE "ON")
  else()
    math(EXPR OPTION_KEY_LENGTH "${OPTION_KEY_LENGTH}+1")
    string(SUBSTRING "${OPTION}" "${OPTION_KEY_LENGTH}" "-1" OPTION_VALUE)
  endif()
  set(OPTION_KEY
      "${OPTION_KEY}"
      PARENT_SCOPE
  )
  set(OPTION_VALUE
      "${OPTION_VALUE}"
      PARENT_SCOPE
  )
endfunction()

# guesses the package version from a git tag
function(cpm_get_version_from_git_tag GIT_TAG RESULT)
  string(LENGTH ${GIT_TAG} length)
  if(length EQUAL 40)
    # GIT_TAG is probably a git hash
    set(${RESULT}
        0
        PARENT_SCOPE
    )
  else()
    string(REGEX MATCH "v?([0123456789.]*).*" _ ${GIT_TAG})
    set(${RESULT}
        ${CMAKE_MATCH_1}
        PARENT_SCOPE
    )
  endif()
endfunction()

# guesses if the git tag is a commit hash or an actual tag or a branch name.
function(cpm_is_git_tag_commit_hash GIT_TAG RESULT)
  string(LENGTH "${GIT_TAG}" length)
  # full hash has 40 characters, and short hash has at least 7 characters.
  if(length LESS 7 OR length GREATER 40)
    set(${RESULT}
        0
        PARENT_SCOPE
    )
  else()
    if(${GIT_TAG} MATCHES "^[a-fA-F0-9]+$")
      set(${RESULT}
          1
          PARENT_SCOPE
      )
    else()
      set(${RESULT}
          0
          PARENT_SCOPE
      )
    endif()
  endif()
endfunction()

function(cpm_prettify_package_arguments OUT_VAR IS_IN_COMMENT)
  set(oneValueArgs
      NAME
      FORCE
      VERSION
      GIT_TAG
      DOWNLOAD_ONLY
      GITHUB_REPOSITORY
      GITLAB_REPOSITORY
      BITBUCKET_REPOSITORY
      GIT_REPOSITORY
      SOURCE_DIR
      FIND_PACKAGE_ARGUMENTS
      NO_CACHE
      SYSTEM
      GIT_SHALLOW
      EXCLUDE_FROM_ALL
      SOURCE_SUBDIR
  )
  set(multiValueArgs URL OPTIONS DOWNLOAD_COMMAND)
  cmake_parse_arguments(CPM_ARGS "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(oneArgName ${oneValueArgs})
    if(DEFINED CPM_ARGS_${oneArgName})
      if(${IS_IN_COMMENT})
        string(APPEND PRETTY_OUT_VAR "#")
      endif()
      if(${oneArgName} STREQUAL "SOURCE_DIR")
        string(REPLACE ${CMAKE_SOURCE_DIR} "\${CMAKE_SOURCE_DIR}" CPM_ARGS_${oneArgName}
                       ${CPM_ARGS_${oneArgName}}
        )
      endif()
      string(APPEND PRETTY_OUT_VAR "  ${oneArgName} ${CPM_ARGS_${oneArgName}}\n")
    endif()
  endforeach()
  foreach(multiArgName ${multiValueArgs})
    if(DEFINED CPM_ARGS_${multiArgName})
      if(${IS_IN_COMMENT})
        string(APPEND PRETTY_OUT_VAR "#")
      endif()
      string(APPEND PRETTY_OUT_VAR "  ${multiArgName}\n")
      foreach(singleOption ${CPM_ARGS_${multiArgName}})
        if(${IS_IN_COMMENT})
          string(APPEND PRETTY_OUT_VAR "#")
        endif()
        string(APPEND PRETTY_OUT_VAR "    \"${singleOption}\"\n")
      endforeach()
    endif()
  endforeach()

  if(NOT "${CPM_ARGS_UNPARSED_ARGUMENTS}" STREQUAL "")
    if(${IS_IN_COMMENT})
      string(APPEND PRETTY_OUT_VAR "#")
    endif()
    string(APPEND PRETTY_OUT_VAR " ")
    foreach(CPM_ARGS_UNPARSED_ARGUMENT ${CPM_ARGS_UNPARSED_ARGUMENTS})
      string(APPEND PRETTY_OUT_VAR " ${CPM_ARGS_UNPARSED_ARGUMENT}")
    endforeach()
    string(APPEND PRETTY_OUT_VAR "\n")
  endif()

  set(${OUT_VAR}
      ${PRETTY_OUT_VAR}
      PARENT_SCOPE
  )

endfunction()
### FIELE END: cmake/CPM.cmake
