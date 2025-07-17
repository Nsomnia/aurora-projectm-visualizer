#pragma once

#include "Config.h"
#include "renderer.h"
#include "audio_input.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include "VideoExporter.h"

#include <projectM-4/projectM.h>
#include <memory>

class QOpenGLWidget;

class Core {
public:
    Core(Config& config);
    ~Core();

    bool init(QOpenGLWidget* openGLWidget);
    void render();
    void cleanup();
    void resize(int w, int h);
    void set_audio_file_paths(const std::vector<std::string>& paths);
    void play_audio();
    void pause_audio();
    void stop_audio();
    void next_audio();
    void prev_audio();
    void next_preset();
    void prev_preset();
    std::string get_current_preset_name() const;

    projectm_handle projectM() const { return _pM; }

private:
    Config& _config;
    projectm_handle _pM;
    Renderer _renderer;
    AudioInput _audio_input;
    PresetManager _preset_manager;
    TextRenderer _text_renderer;
    TextManager _text_manager;
    AnimationManager _animation_manager;
    VideoExporter _video_exporter;
    QOpenGLWidget* _gl_widget;

    bool g_quit;
    int _current_audio_index;
    enum class PlaybackState { Stopped, Playing, Paused } _playback_state;

    // Private helper to load and play a specific audio file
    void load_and_play_current_audio();
};
