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
