#pragma once

#include "Config.h"
#include <QObject>
#include <memory>

class Core;
class QMainWindow;

class QtGui : public QObject {
    Q_OBJECT

public:
    QtGui(Config& config, Core& core);
    ~QtGui();

    void init();

private:
    Config& _config;
    Core& _core;
    std::unique_ptr<QMainWindow> _window;
};
