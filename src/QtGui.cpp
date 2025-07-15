#include "QtGui.h"
#include "core.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>

QtGui::QtGui(Config& config, Core& core) : _config(config), _core(core) {
    _thread = std::thread(&QtGui::run, this);
}

QtGui::~QtGui() {
    if (_thread.joinable()) {
        _thread.join();
    }
}

void QtGui::run() {
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);

    _window = std::make_unique<QMainWindow>();
    _window->setWindowTitle("Aurora Visualizer Controls");
    _window->resize(400, 300);

    QPushButton* quitButton = new QPushButton("Quit", _window.get());
    quitButton->setGeometry(10, 10, 80, 30);
    QObject::connect(quitButton, &QPushButton::clicked, &app, &QApplication::quit);

    _window->show();
    app.exec();
}
