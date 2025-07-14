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

    ImGuiIntegration::render();
}

void Gui::handle_event(SDL_Event& event) {
    ImGuiIntegration::process_event(event);
}

void Gui::cleanup() {
    ImGuiIntegration::cleanup();
}
