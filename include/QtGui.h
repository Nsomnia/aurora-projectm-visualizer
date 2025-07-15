#pragma once

#include "Config.h"
#include <QObject>
#include <memory>

// Forward declarations for Qt classes
class Core;
class QMainWindow;
class QDockWidget;
class QMenu;
class QAction;

class QtGui : public QObject {
    Q_OBJECT

public:
    QtGui(Config& config, Core& core);
    ~QtGui();

    void init();

private slots:
    void about();

private:
    void createMenus();
    void createDockWidgets();

    Config& _config;
    Core& _core;
    std::unique_ptr<QMainWindow> _window;

    // Menu Bar
    QMenu* fileMenu;
    QMenu* helpMenu;
    QAction* quitAction;
    QAction* aboutAction;

    // Dock Widgets
    QDockWidget* controlsDock;
};