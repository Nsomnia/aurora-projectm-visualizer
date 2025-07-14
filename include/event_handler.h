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
