#include "event_handler.h"
#include "preset_manager.h"

EventHandler::EventHandler(Config& config, PresetManager& presetManager, AnimationManager& animationManager, TextRenderer& textRenderer, TextManager& textManager, projectm_handle pM)
    : _config(config),
      _preset_manager(presetManager),
      _animation_manager(animationManager),
      _text_renderer(textRenderer),
      _text_manager(textManager),
      _pM(pM)
{
}

bool EventHandler::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == _config.next_preset_key.key()) {
            _preset_manager.get_next_preset(_pM);
        } else if (keyEvent->key() == _config.prev_preset_key.key()) {
            _preset_manager.get_prev_preset(_pM);
        } else if (keyEvent->key() == _config.mark_broken_preset_key.key()) {
            _preset_manager.mark_current_preset_as_broken();
        } else if (keyEvent->key() == _config.favorite_preset_key.key()) {
            _preset_manager.toggle_current_preset_favorite();
        }
    }
    return QObject::eventFilter(obj, event);
}
