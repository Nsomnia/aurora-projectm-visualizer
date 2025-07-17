// src/event_handler.cpp
#include "event_handler.h"
#include "core.h"

EventHandler::EventHandler(Config& config, Core& core, PresetManager& presetManager)
    : _config(config), _core(core), _presetManager(presetManager) {}

EventHandler::~EventHandler() {}

void EventHandler::handle_key_press(QKeyEvent* event) {
    QKeySequence key_sequence(event->key());

    if (key_sequence.matches(_config.next_preset_key)) {
        _core.next_preset();
    } else if (key_sequence.matches(_config.prev_preset_key)) {
        _core.prev_preset();
    } else if (key_sequence.matches(_config.mark_broken_preset_key)) {
        _presetManager.mark_current_preset_as_broken();
        _core.next_preset();
    } else if (key_sequence.matches(_config.favorite_preset_key)) {
        _presetManager.toggle_favorite_current_preset();
    } else {
        switch (event->key()) {
            case Qt::Key_Right:
                _core.next_audio();
                break;
            case Qt::Key_Left:
                _core.prev_audio();
                break;
            case Qt::Key_Space:
                // This is a placeholder. A proper implementation would check
                // the current playback state in Core.
                _core.pause_audio(); // Or play_audio()
                break;
            case Qt::Key_Q:
                // Quitting is handled by the main window closing.
                break;
        }
    }
}
