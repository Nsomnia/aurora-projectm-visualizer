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
};