#include "QtGui.h"
#include "core.h"
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>

QtGui::QtGui(Config& config, Core& core) : _config(config), _core(core) {}

QtGui::~QtGui() {}

void QtGui::init() {
    _window = std::make_unique<QMainWindow>();
    _window->setWindowTitle("Aurora Visualizer");
    _window->resize(1280, 720); // Give it a more standard window size
    _window->setCentralWidget(new QWidget()); // A dummy central widget

    createMenus();
    createDockWidgets();

    _window->show();
}

void QtGui::createMenus() {
    // File Menu
    fileMenu = _window->menuBar()->addMenu(tr("&File"));
    quitAction = new QAction(tr("&Quit"), _window.get());
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    fileMenu->addAction(quitAction);

    // Help Menu
    helpMenu = _window->menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), _window.get());
    connect(aboutAction, &QAction::triggered, this, &QtGui::about);
    helpMenu->addAction(aboutAction);
}

void QtGui::createDockWidgets() {
    controlsDock = new QDockWidget(tr("Controls"), _window.get());
    controlsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* controlsWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(controlsWidget);
    layout->setSpacing(10);

    // Preset Duration Slider
    QLabel* presetDurationLabel = new QLabel("Preset Duration: " + QString::number(_config.presetDuration, 'f', 1) + "s");
    layout->addWidget(presetDurationLabel);
    QSlider* presetDurationSlider = new QSlider(Qt::Horizontal);
    presetDurationSlider->setRange(10, 600); // 1.0 to 60.0 seconds
    presetDurationSlider->setValue(static_cast<int>(_config.presetDuration * 10));
    connect(presetDurationSlider, &QSlider::valueChanged, this, [this, presetDurationLabel](int value) {
        _config.presetDuration = value / 10.0;
        presetDurationLabel->setText("Preset Duration: " + QString::number(_config.presetDuration, 'f', 1) + "s");
    });
    layout->addWidget(presetDurationSlider);

    // Shuffle Enabled Checkbox
    QCheckBox* shuffleCheckBox = new QCheckBox("Shuffle Presets");
    shuffleCheckBox->setChecked(_config.shuffleEnabled);
    connect(shuffleCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        _config.shuffleEnabled = checked;
    });
    layout->addWidget(shuffleCheckBox);

    layout->addStretch(); // Pushes widgets to the top

    controlsDock->setWidget(controlsWidget);
    _window->addDockWidget(Qt::RightDockWidgetArea, controlsDock);
}

void QtGui::about() {
    QMessageBox::about(_window.get(), tr("About Aurora Visualizer"),
        tr("<h2>Aurora Visualizer</h2>"
           "<p>Version 0.1.0-dev</p>"
           "<p>A cross-platform audio visualizer using projectM.</p>"));
}
