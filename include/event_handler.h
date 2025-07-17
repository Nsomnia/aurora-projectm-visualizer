#pragma once

#include "Config.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include "core.h"
#include <QKeyEvent>
#include <projectM-4/projectM.h>
#include <string>
#include <vector>

class EventHandler {
public:
    EventHandler(Config& config, Core& core, PresetManager& presetManager);
    ~EventHandler();

    void handle_key_press(QKeyEvent* event);

private:
    Config& _config;
    Core& _core;
    PresetManager& _presetManager;
};