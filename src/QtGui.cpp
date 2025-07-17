#include "QtGui.h"
#include "core.h"
#include "QtOpenGLWidget.h"
#include "Config.h"
#include <QApplication>
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
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <filesystem>

// Helper function to extract preset name
QString get_preset_short_name(const std::string& full_path) {
    if (full_path.empty()) {
        return "None";
    }
    std::filesystem::path p(full_path);
    return QString::fromStdString(p.stem().string());
}

QtGui::QtGui(Config& config, Core& core, QWidget* parent)
    : QMainWindow(parent), _config(config), _core(core) {
    setWindowTitle("Aurora Visualizer");
    resize(_config.width, _config.height);
    setDockNestingEnabled(true);

    _visualizer_widget = new QtOpenGLWidget(_core, this);
    setCentralWidget(_visualizer_widget);

    createMenus();
    createDockWidgets();

    show();
}

QtGui::~QtGui() {}

void QtGui::createMenus() {
    // File Menu
    _file_menu = menuBar()->addMenu(tr("&File"));
    _quit_action = new QAction(tr("&Quit"), this);
    _quit_action->setShortcuts(QKeySequence::Quit);
    connect(_quit_action, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    _file_menu->addAction(_quit_action);

    // View Menu (for toggling docks)
    _view_menu = menuBar()->addMenu(tr("&View"));

    // Help Menu
    _help_menu = menuBar()->addMenu(tr("&Help"));
    _about_action = new QAction(tr("&About"), this);
    connect(_about_action, &QAction::triggered, this, &QtGui::about);
    _help_menu->addAction(_about_action);
}

void QtGui::createDockWidgets() {
    // --- Playlist Dock ---
    _playlist_dock = new QDockWidget(tr("Playlist"), this);
    _playlist_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* playlistContainerWidget = new QWidget();
    QVBoxLayout* playlistLayout = new QVBoxLayout(playlistContainerWidget);

    _playlist_widget = new QListWidget();
    _playlist_widget->setDragDropMode(QAbstractItemView::InternalMove);
    _playlist_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (const std::string& filePath : _config.audio_file_paths) {
        _playlist_widget->addItem(QString::fromStdString(filePath));
    }
    playlistLayout->addWidget(_playlist_widget);

    // Add/Remove Buttons
    QHBoxLayout* playlistButtonsLayout = new QHBoxLayout();
    QPushButton* addFileButton = new QPushButton(tr("Add File"));
    QPushButton* removeFileButton = new QPushButton(tr("Remove Selected"));
    playlistButtonsLayout->addWidget(addFileButton);
    playlistButtonsLayout->addWidget(removeFileButton);
    playlistLayout->addLayout(playlistButtonsLayout);

    // Playback Controls
    QHBoxLayout* playbackButtonsLayout = new QHBoxLayout();
    QPushButton* playButton = new QPushButton(tr("Play"));
    QPushButton* pauseButton = new QPushButton(tr("Pause"));
    QPushButton* stopButton = new QPushButton(tr("Stop"));
    QPushButton* nextButton = new QPushButton(tr("Next"));
    QPushButton* prevButton = new QPushButton(tr("Previous"));
    playbackButtonsLayout->addWidget(prevButton);
    playbackButtonsLayout->addWidget(playButton);
    playbackButtonsLayout->addWidget(pauseButton);
    playbackButtonsLayout->addWidget(stopButton);
    playbackButtonsLayout->addWidget(nextButton);
    playlistLayout->addLayout(playbackButtonsLayout);

    _playlist_dock->setWidget(playlistContainerWidget);
    addDockWidget(Qt::LeftDockWidgetArea, _playlist_dock);

    // --- Controls Dock ---
    _controls_dock = new QDockWidget(tr("Controls"), this);
    _controls_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* controlsContainerWidget = new QWidget();
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsContainerWidget);
    controlsLayout->setSpacing(10);

    // Current Preset Label
    _current_preset_label = new QLabel(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
    _current_preset_label->setWordWrap(true);
    controlsLayout->addWidget(_current_preset_label);

    // Next/Previous Preset Buttons
    QHBoxLayout* presetButtonsLayout = new QHBoxLayout();
    QPushButton* prevPresetButton = new QPushButton(tr("Previous"));
    QPushButton* nextPresetButton = new QPushButton(tr("Next"));
    presetButtonsLayout->addWidget(prevPresetButton);
    presetButtonsLayout->addWidget(nextPresetButton);
    controlsLayout->addLayout(presetButtonsLayout);

    // Preset Duration Slider
    QLabel* presetDurationLabel = new QLabel("Duration: " + QString::number(_config.presetDuration, 'f', 1) + "s");
    controlsLayout->addWidget(presetDurationLabel);
    QSlider* presetDurationSlider = new QSlider(Qt::Horizontal);
    presetDurationSlider->setRange(10, 600); // 1.0 to 60.0 seconds
    presetDurationSlider->setValue(static_cast<int>(_config.presetDuration * 10));
    connect(presetDurationSlider, &QSlider::valueChanged, this, [this, presetDurationLabel](int value) {
        _config.presetDuration = value / 10.0;
        presetDurationLabel->setText("Duration: " + QString::number(_config.presetDuration, 'f', 1) + "s");
    });
    controlsLayout->addWidget(presetDurationSlider);

    // Shuffle Options
    QCheckBox* shuffleCheckBox = new QCheckBox("Shuffle Presets");
    shuffleCheckBox->setChecked(_config.shuffleEnabled);
    connect(shuffleCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        _config.shuffleEnabled = checked;
    });
    controlsLayout->addWidget(shuffleCheckBox);

    QCheckBox* favoritesOnlyShuffleCheckBox = new QCheckBox(tr("Favorites Only"));
    favoritesOnlyShuffleCheckBox->setChecked(_config.favorites_only_shuffle);
    connect(favoritesOnlyShuffleCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        _config.favorites_only_shuffle = checked;
    });
    controlsLayout->addWidget(favoritesOnlyShuffleCheckBox);


    controlsLayout->addStretch(); // Pushes widgets to the top
    _controls_dock->setWidget(controlsContainerWidget);
    addDockWidget(Qt::LeftDockWidgetArea, _controls_dock);

    // --- Dock Layout ---
    tabifyDockWidget(_playlist_dock, _controls_dock);
    _playlist_dock->raise(); // Make playlist the default visible tab

    // --- View Menu Actions ---
    _view_menu->addAction(_playlist_dock->toggleViewAction());
    _view_menu->addAction(_controls_dock->toggleViewAction());


    // --- Connect Signals ---
    connect(addFileButton, &QPushButton::clicked, this, &QtGui::add_audio_file);
    connect(removeFileButton, &QPushButton::clicked, this, &QtGui::remove_selected_audio_file);
    connect(_playlist_widget->model(), &QAbstractItemModel::rowsMoved, this, &QtGui::playlist_reordered);
    connect(playButton, &QPushButton::clicked, this, &QtGui::play_audio);
    connect(pauseButton, &QPushButton::clicked, this, &QtGui::pause_audio);
    connect(stopButton, &QPushButton::clicked, this, &QtGui::stop_audio);
    connect(nextButton, &QPushButton::clicked, this, &QtGui::next_audio);
    connect(prevButton, &QPushButton::clicked, this, &QtGui::prev_audio);
    connect(nextPresetButton, &QPushButton::clicked, this, &QtGui::next_preset);
    connect(prevPresetButton, &QPushButton::clicked, this, &QtGui::prev_preset);
}

void QtGui::add_audio_file() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Add Audio Files"),
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        tr("Audio Files (*.mp3 *.wav *.flac *.ogg);;All Files (*)")
    );

    if (!fileNames.isEmpty()) {
        for (const QString& fileName : fileNames) {
            _config.audio_file_paths.push_back(fileName.toStdString());
            _playlist_widget->addItem(fileName);
        }
        _core.set_audio_file_paths(_config.audio_file_paths); // Notify Core
    }
}

void QtGui::remove_selected_audio_file() {
    QList<QListWidgetItem*> selectedItems = _playlist_widget->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    // Remove from QListWidget and _config.audio_file_paths
    // Iterate backwards to safely remove items while modifying the list
    for (int i = selectedItems.size() - 1; i >= 0; --i) {
        QListWidgetItem* item = selectedItems.at(i);
        int row = _playlist_widget->row(item);
        if (row != -1) {
            _config.audio_file_paths.erase(_config.audio_file_paths.begin() + row);
            delete _playlist_widget->takeItem(row); // takeItem removes and returns, then delete
        }
    }
    _core.set_audio_file_paths(_config.audio_file_paths); // Notify Core
}

void QtGui::playlist_reordered() {
    // Reconstruct _config.audio_file_paths based on the new order in playlistWidget
    _config.audio_file_paths.clear();
    for (int i = 0; i < _playlist_widget->count(); ++i) {
        _config.audio_file_paths.push_back(_playlist_widget->item(i)->text().toStdString());
    }
    _core.set_audio_file_paths(_config.audio_file_paths); // Notify Core
}

void QtGui::play_audio() {
    _core.play_audio();
}

void QtGui::pause_audio() {
    _core.pause_audio();
}

void QtGui::stop_audio() {
    _core.stop_audio();
}

void QtGui::next_audio() {
    _core.next_audio();
}

void QtGui::prev_audio() {
    _core.prev_audio();
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
           "<p>Version 0.1.0-dev</p>"
           "<p>A cross-platform audio visualizer using projectM.</p>"));
}
