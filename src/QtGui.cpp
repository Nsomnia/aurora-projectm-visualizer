#include "QtGui.h"
#include "core.h"
#include "QtOpenGLWidget.h"
#include "Config.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QDockWidget>
#include <QListWidget>
#include <QFileDialog>
#include <QIcon>
#include "utils/Logger.h"

// Helper function to extract preset name
QString get_preset_short_name(const std::string& full_path) {
    if (full_path.empty()) {
        return "None";
    }
    // In C++17, you can use std::filesystem for this
    // For now, a simple string manipulation will do
    size_t last_slash = full_path.find_last_of("/\\");
    std::string filename = (last_slash == std::string::npos) ? full_path : full_path.substr(last_slash + 1);
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        filename = filename.substr(0, dot_pos);
    }
    return QString::fromStdString(filename);
}

QtGui::QtGui(Config& config, Core& core)
    : QMainWindow(nullptr), _config(config), _core(core) {
    setWindowTitle("Aurora Visualizer");
    resize(_config.width, _config.height);

    // Main container widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Right panel for visualizer
    _visualizer_widget = new QtOpenGLWidget(_core, centralWidget);

    mainLayout->addWidget(_visualizer_widget, 1); // The '1' makes it take up remaining space

    createMenus();
    createDockWidgets();
}

QtGui::~QtGui() {}

void QtGui::createMenus() {
    _file_menu = menuBar()->addMenu(tr("&File"));
    QAction* addAudioAction = new QAction(tr("&Add Audio File..."), this);
    connect(addAudioAction, &QAction::triggered, this, &QtGui::add_audio_file);
    _file_menu->addAction(addAudioAction);

    _file_menu->addSeparator();

    _quit_action = new QAction(tr("&Quit"), this);
    _quit_action->setShortcuts(QKeySequence::Quit);
    connect(_quit_action, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    _file_menu->addAction(_quit_action);

    _view_menu = menuBar()->addMenu(tr("&View"));

    _help_menu = menuBar()->addMenu(tr("&Help"));
    _about_action = new QAction(tr("&About"), this);
    connect(_about_action, &QAction::triggered, this, &QtGui::about);
    _help_menu->addAction(_about_action);
}

void QtGui::createDockWidgets()
{
    // --- Playlist Dock ---
    _playlist_dock = new QDockWidget(tr("Playlist"), this);
    _playlist_widget = new QListWidget();
    _playlist_dock->setWidget(_playlist_widget);
    addDockWidget(Qt::LeftDockWidgetArea, _playlist_dock);
    _view_menu->addAction(_playlist_dock->toggleViewAction());

    // --- Controls Dock ---
    _controls_dock = new QDockWidget(tr("Controls"), this);
    QWidget* controlsWidget = new QWidget();
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsWidget);

    // Preset Controls
    _current_preset_label = new QLabel(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
    _current_preset_label->setWordWrap(true);
    controlsLayout->addWidget(_current_preset_label);

    QHBoxLayout* presetButtonsLayout = new QHBoxLayout();
    QPushButton* prevPresetButton = new QPushButton(tr("Previous"));
    QPushButton* nextPresetButton = new QPushButton(tr("Next"));
    presetButtonsLayout->addWidget(prevPresetButton);
    presetButtonsLayout->addWidget(nextPresetButton);
    controlsLayout->addLayout(presetButtonsLayout);

    // Audio Controls
    QHBoxLayout* audioButtonsLayout = new QHBoxLayout();
    QPushButton* prevAudioButton = new QPushButton(tr("Prev Audio"));
    QPushButton* playButton = new QPushButton(tr("Play"));
    QPushButton* pauseButton = new QPushButton(tr("Pause"));
    QPushButton* stopButton = new QPushButton(tr("Stop"));
    QPushButton* nextAudioButton = new QPushButton(tr("Next Audio"));
    audioButtonsLayout->addWidget(prevAudioButton);
    audioButtonsLayout->addWidget(playButton);
    audioButtonsLayout->addWidget(pauseButton);
    audioButtonsLayout->addWidget(stopButton);
    audioButtonsLayout->addWidget(nextAudioButton);
    controlsLayout->addLayout(audioButtonsLayout);

    controlsLayout->addStretch();
    _controls_dock->setWidget(controlsWidget);
    addDockWidget(Qt::LeftDockWidgetArea, _controls_dock);
    _view_menu->addAction(_controls_dock->toggleViewAction());

    // --- Connect Signals ---
    connect(nextPresetButton, &QPushButton::clicked, this, &QtGui::next_preset);
    connect(prevPresetButton, &QPushButton::clicked, this, &QtGui::prev_preset);
    connect(playButton, &QPushButton::clicked, this, &QtGui::play_audio);
    connect(pauseButton, &QPushButton::clicked, this, &QtGui::pause_audio);
    connect(stopButton, &QPushButton::clicked, this, &QtGui::stop_audio);
    connect(nextAudioButton, &QPushButton::clicked, this, &QtGui::next_audio);
    connect(prevAudioButton, &QPushButton::clicked, this, &QtGui::prev_audio);
}

void QtGui::next_preset() {
    _core.next_preset();
    _current_preset_label->setText(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
}

void QtGui::prev_preset() {
    _core.prev_preset();
    _current_preset_label->setText(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
}

void QtGui::about() {
    QMessageBox::about(this, tr("About Aurora Visualizer"),
        tr("<h2>Aurora Visualizer</h2>"
           "<p>Version 0.2.0</p>"
           "<p>A cross-platform audio visualizer using projectM.</p>"));
}

void QtGui::add_audio_file()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Select one or more audio files to open",
        QDir::homePath(),
        "Audio Files (*.wav *.mp3 *.ogg *.flac)"
    );

    if (!files.isEmpty()) {
        _playlist_widget->addItems(files);
        update_core_playlist();
    }
}

void QtGui::remove_selected_audio_file()
{
    qDeleteAll(_playlist_widget->selectedItems());
    update_core_playlist();
}

void QtGui::playlist_reordered()
{
    update_core_playlist();
}

void QtGui::update_core_playlist()
{
    std::vector<std::string> paths;
    for (int i = 0; i < _playlist_widget->count(); ++i) {
        paths.push_back(_playlist_widget->item(i)->text().toStdString());
    }
    _core.set_audio_file_paths(paths);
}

void QtGui::play_audio()
{
    Logger::info("Play button clicked");
    _core.play_audio();
}

void QtGui::pause_audio()
{
    Logger::info("Pause button clicked");
    _core.pause_audio();
}

void QtGui::stop_audio()
{
    Logger::info("Stop button clicked");
    _core.stop_audio();
}

void QtGui::next_audio()
{
    Logger::info("Next audio button clicked");
    _core.next_audio();
}

void QtGui::prev_audio()
{
    Logger::info("Previous audio button clicked");
    _core.prev_audio();
}
