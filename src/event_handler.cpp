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
}