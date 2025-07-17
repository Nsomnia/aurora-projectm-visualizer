#pragma once

#include <QMainWindow>
#include <memory>

// Forward declarations
class Core;
class QDockWidget;
class QMenu;
class QAction;
class QListWidget;
class QLabel;
class QtOpenGLWidget;
class Config;

class QtGui : public QMainWindow {
    Q_OBJECT

public:
    QtGui(Config& config, Core& core);
    ~QtGui();

private slots:
    void about();
    void add_audio_file();
    void remove_selected_audio_file();
    void playlist_reordered();
    void play_audio();
    void pause_audio();
    void stop_audio();
    void next_audio();
    void prev_audio();
    void next_preset();
    void prev_preset();

private:
    void createMenus();
    void createDockWidgets();

    Config& _config;
    Core& _core;
    QtOpenGLWidget* _visualizer_widget;

    // Menu Bar
    QMenu* _file_menu;
    QMenu* _view_menu;
    QMenu* _help_menu;
    QAction* _quit_action;
    QAction* _about_action;

    // Dock Widgets
    QDockWidget* _playlist_dock;
    QDockWidget* _controls_dock;
    QListWidget* _playlist_widget;
    QLabel* _current_preset_label;
};