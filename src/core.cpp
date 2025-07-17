// src/core.cpp
#include "core.h"
#include "utils/Logger.h"
#include <QOpenGLWidget>

Core::Core(Config& config)
    : _config(config),
      _pM(nullptr),
      _renderer(),
      _audio_input(config),
      _preset_manager(config),
      _text_renderer(),
      _text_manager(_text_renderer),
      _animation_manager(config, _text_renderer),
      _video_exporter(config),
      _gl_widget(nullptr),
      g_quit(false),
      _current_audio_index(0),
      _playback_state(PlaybackState::Stopped)
{}

Core::~Core() {
    cleanup();
}

bool Core::init(QOpenGLWidget* openGLWidget) {
    _gl_widget = openGLWidget;
    _gl_widget->makeCurrent();

    if (!_renderer.init(_config)) {
        Logger::error("Failed to initialize renderer");
        return false;
    }

    if (!_text_renderer.init(_config.font_path, _config.songInfoFontSize)) {
        Logger::error("Failed to load font: " + _config.font_path);
    }
    _text_renderer.setProjection(_config.width, _config.height);

    _pM = projectm_create();
    projectm_set_window_size(_pM, _config.width, _config.height);
    projectm_set_mesh_size(_pM, 64, 48);
    projectm_set_soft_cut_duration(_pM, _config.presetBlendTime);

    if (!_config.use_default_projectm_visualizer) {
        _preset_manager.load_presets();
        next_preset();
    }

    _audio_input.set_projectm_handle(_pM);

    if (!_audio_input.init()) {
        Logger::error("Failed to initialize audio input");
        return false;
    }

    _gl_widget->doneCurrent();
    return true;
}

void Core::render() {
    Logger::info("Core::render() called");
    _renderer.render(_pM);

    // Text rendering would go here, but needs to be adapted for Qt's paint cycle
}

void Core::cleanup() {
    if (_pM) {
        projectm_destroy(_pM);
        _pM = nullptr;
    }
    _audio_input.cleanup();
}

void Core::resize(int w, int h) {
    _renderer.resize(w, h);
    projectm_set_window_size(_pM, w, h);
}

void Core::set_audio_file_paths(const std::vector<std::string>& paths) {
    _config.audio_file_paths = paths;
    _current_audio_index = 0;
}

void Core::play_audio() {
    if (_playback_state == PlaybackState::Paused) {
        _audio_input.resume_music();
    } else {
        load_and_play_current_audio();
    }
    _playback_state = PlaybackState::Playing;
}

void Core::pause_audio() {
    _audio_input.pause_music();
    _playback_state = PlaybackState::Paused;
}

void Core::stop_audio() {
    _audio_input.stop_music();
    _playback_state = PlaybackState::Stopped;
}

void Core::next_audio() {
    if (_config.audio_file_paths.empty()) return;
    _current_audio_index = (_current_audio_index + 1) % _config.audio_file_paths.size();
    load_and_play_current_audio();
}

void Core::prev_audio() {
    if (_config.audio_file_paths.empty()) return;
    _current_audio_index = (_current_audio_index - 1 + _config.audio_file_paths.size()) % _config.audio_file_paths.size();
    load_and_play_current_audio();
}

void Core::load_and_play_current_audio() {
    if (_config.audio_file_paths.empty() || _current_audio_index >= _config.audio_file_paths.size()) {
        return;
    }
    const std::string& path = _config.audio_file_paths[_current_audio_index];
    _audio_input.load_and_play_music(path);
}

void Core::next_preset() {
    if (_config.use_default_projectm_visualizer) return;
    std::string preset = _preset_manager.get_next_preset();
    if (!preset.empty()) {
        projectm_load_preset_file(_pM, preset.c_str(), true);
    }
}

void Core::prev_preset() {
    if (_config.use_default_projectm_visualizer) return;
    std::string preset = _preset_manager.get_prev_preset();
    if (!preset.empty()) {
        projectm_load_preset_file(_pM, preset.c_str(), true);
    }
}

std::string Core::get_current_preset_name() const {
    // This function needs access to the current preset path from PresetManager
    return _preset_manager.get_current_preset();
}