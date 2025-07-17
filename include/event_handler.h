#pragma once

#include "Config.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"
#include <projectM-4/projectM.h>
#include <QObject>
#include <projectM-4/projectM.h>
#include <QKeyEvent>

class EventHandler : public QObject {
    Q_OBJECT

public:
    EventHandler(Config& config, PresetManager& presetManager, AnimationManager& animationManager, TextRenderer& textRenderer, TextManager& textManager, projectm_handle pM);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Config& _config;
    PresetManager& _preset_manager;
    AnimationManager& _animation_manager;
    TextRenderer& _text_renderer;
    TextManager& _text_manager;
    projectm_handle _pM;
};
