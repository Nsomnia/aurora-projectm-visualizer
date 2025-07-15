#include "QtGui.h"
#include "core.h"
#include "QtOpenGLWidget.h" // Include the new header
#include "Config.h"
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

QWidget* QtGui::getVisualizerWidget() const {
    return visualizerWidget;
}


QtGui::QtGui(Config& config, Core& core) : _config(config), _core(core) {}

QtGui::~QtGui() {}

void QtGui::init() {
    _window = std::make_unique<QMainWindow>();
    _window->setWindowTitle("Aurora Visualizer");
    _window->resize(_config.width, _config.height); // Set window size from config
    _window->setDockNestingEnabled(true); // Allow tabbing

    // Create the custom visualizer widget
    visualizerWidget = new QtOpenGLWidget(_core, _window.get());
    _window->setCentralWidget(visualizerWidget);

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

    // View Menu (for toggling docks)
    viewMenu = _window->menuBar()->addMenu(tr("&View"));

    // Help Menu
    helpMenu = _window->menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), _window.get());
    connect(aboutAction, &QAction::triggered, this, &QtGui::about);
    helpMenu->addAction(aboutAction);
}

void QtGui::createDockWidgets() {
    // --- Playlist Dock ---
    playlistDock = new QDockWidget(tr("Playlist"), _window.get());
    playlistDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* playlistContainerWidget = new QWidget();
    QVBoxLayout* playlistLayout = new QVBoxLayout(playlistContainerWidget);

    playlistWidget = new QListWidget();
    playlistWidget->setDragDropMode(QAbstractItemView::InternalMove);
    playlistWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (const std::string& filePath : _config.audio_file_paths) {
        playlistWidget->addItem(QString::fromStdString(filePath));
    }
    playlistLayout->addWidget(playlistWidget);

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

    playlistDock->setWidget(playlistContainerWidget);
    _window->addDockWidget(Qt::LeftDockWidgetArea, playlistDock);

    // --- Controls Dock ---
    controlsDock = new QDockWidget(tr("Controls"), _window.get());
    controlsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* controlsContainerWidget = new QWidget();
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsContainerWidget);
    controlsLayout->setSpacing(10);

    // Current Preset Label
    currentPresetLabel = new QLabel(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
    currentPresetLabel->setWordWrap(true);
    controlsLayout->addWidget(currentPresetLabel);

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
    controlsDock->setWidget(controlsContainerWidget);
    _window->addDockWidget(Qt::LeftDockWidgetArea, controlsDock);

    // --- Dock Layout ---
    _window->tabifyDockWidget(playlistDock, controlsDock);
    playlistDock->raise(); // Make playlist the default visible tab

    // --- View Menu Actions ---
    viewMenu->addAction(playlistDock->toggleViewAction());
    viewMenu->addAction(controlsDock->toggleViewAction());


    // --- Connect Signals ---
    connect(addFileButton, &QPushButton::clicked, this, &QtGui::add_audio_file);
    connect(removeFileButton, &QPushButton::clicked, this, &QtGui::remove_selected_audio_file);
    connect(playlistWidget->model(), &QAbstractItemModel::rowsMoved, this, &QtGui::playlist_reordered);
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
        _window.get(),
        tr("Add Audio Files"),
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        tr("Audio Files (*.mp3 *.wav *.flac *.ogg);;All Files (*)")
    );

    if (!fileNames.isEmpty()) {
        for (const QString& fileName : fileNames) {
            _config.audio_file_paths.push_back(fileName.toStdString());
            playlistWidget->addItem(fileName);
        }
        _core.set_audio_file_paths(_config.audio_file_paths); // Notify Core
    }
}

void QtGui::remove_selected_audio_file() {
    QList<QListWidgetItem*> selectedItems = playlistWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    // Remove from QListWidget and _config.audio_file_paths
    // Iterate backwards to safely remove items while modifying the list
    for (int i = selectedItems.size() - 1; i >= 0; --i) {
        QListWidgetItem* item = selectedItems.at(i);
        int row = playlistWidget->row(item);
        if (row != -1) {
            _config.audio_file_paths.erase(_config.audio_file_paths.begin() + row);
            delete playlistWidget->takeItem(row); // takeItem removes and returns, then delete
        }
    }
    _core.set_audio_file_paths(_config.audio_file_paths); // Notify Core
}

void QtGui::playlist_reordered() {
    // Reconstruct _config.audio_file_paths based on the new order in playlistWidget
    _config.audio_file_paths.clear();
    for (int i = 0; i < playlistWidget->count(); ++i) {
        _config.audio_file_paths.push_back(playlistWidget->item(i)->text().toStdString());
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
    currentPresetLabel->setText(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
}

void QtGui::prev_preset() {
    _core.prev_preset();
    currentPresetLabel->setText(tr("Preset: ") + get_preset_short_name(_core.get_current_preset_name()));
}

void QtGui::about() {
    QMessageBox::about(_window.get(), tr("About Aurora Visualizer"),
        tr("<h2>Aurora Visualizer</h2>"
           "<p>Version 0.1.0-dev</p>"
           "<p>A cross-platform audio visualizer using projectM.</p>"));
}
