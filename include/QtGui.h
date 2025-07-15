#pragma once

#include "Config.h"
#include <QObject>
#include <memory>
#include <thread>

class Core;
class QMainWindow;

class QtGui : public QObject {
    Q_OBJECT

public:
    QtGui(Config& config, Core& core);
    ~QtGui();

    void run();

private:
    Config& _config;
    Core& _core;
    std::unique_ptr<QMainWindow> _window;
    std::thread _thread;
};
