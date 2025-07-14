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
