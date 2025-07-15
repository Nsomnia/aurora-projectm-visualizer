#pragma once

#include "Config.h"
#include "renderer.h"
#include "event_handler.h"
#include "audio_input.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include "QtGui.h"
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
    void set_gui(std::unique_ptr<QtGui> gui);
    void set_audio_file_paths(const std::vector<std::string>& paths);
    void play_audio();
    void pause_audio();
    void stop_audio();
    void next_audio();
    void prev_audio();
    void next_preset();
    void prev_preset();
    std::string get_current_preset_name() const;
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
    std::unique_ptr<QtGui> _gui;

    bool g_quit;
    int _current_audio_index;
    enum class PlaybackState { Stopped, Playing, Paused } _playback_state;

    // Private helper to load and play a specific audio file
    void load_and_play_current_audio();
};
