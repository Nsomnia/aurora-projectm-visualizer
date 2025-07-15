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
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>

QtGui::QtGui(Config& config, Core& core) : _config(config), _core(core) {}

QtGui::~QtGui() {}

void QtGui::init() {
    _window = std::make_unique<QMainWindow>();
    _window->setWindowTitle("Aurora Visualizer");
    _window->resize(_config.width, _config.height); // Set window size from config
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

    // Playlist Dock
    playlistDock = new QDockWidget(tr("Playlist"), _window.get());
    playlistDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    playlistWidget = new QListWidget(); // Initialize member variable
    playlistWidget->setDragDropMode(QAbstractItemView::InternalMove);
    playlistWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    playlistWidget->setDragEnabled(true);
    playlistWidget->setAcceptDrops(true);
    playlistWidget->setDropIndicatorShown(true);
    for (const std::string& filePath : _config.audio_file_paths) {
        playlistWidget->addItem(QString::fromStdString(filePath));
    }
    QVBoxLayout* playlistLayout = new QVBoxLayout();
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

    QWidget* playlistContainerWidget = new QWidget();
    playlistContainerWidget->setLayout(playlistLayout);
    playlistDock->setWidget(playlistContainerWidget);
    _window->addDockWidget(Qt::LeftDockWidgetArea, playlistDock);

    // Visualizer Dock
    visualizerDock = new QDockWidget(tr("Visualizer"), _window.get());
    visualizerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* visualizerWidget = new QWidget();
    QVBoxLayout* visualizerLayout = new QVBoxLayout(visualizerWidget);
    visualizerLayout->setSpacing(10);

    currentPresetLabel = new QLabel(tr("Current Preset: ") + QString::fromStdString(_core.get_current_preset_name()));
    visualizerLayout->addWidget(currentPresetLabel);

    // Favorites Only Shuffle Checkbox
    QCheckBox* favoritesOnlyShuffleCheckBox = new QCheckBox(tr("Shuffle Favorites Only"));
    favoritesOnlyShuffleCheckBox->setChecked(_config.favorites_only_shuffle);
    connect(favoritesOnlyShuffleCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        _config.favorites_only_shuffle = checked;
    });
    visualizerLayout->addWidget(favoritesOnlyShuffleCheckBox);

    // Next/Previous Preset Buttons
    QHBoxLayout* presetButtonsLayout = new QHBoxLayout();
    QPushButton* prevPresetButton = new QPushButton(tr("Previous Preset"));
    QPushButton* nextPresetButton = new QPushButton(tr("Next Preset"));
    presetButtonsLayout->addWidget(prevPresetButton);
    presetButtonsLayout->addWidget(nextPresetButton);
    visualizerLayout->addLayout(presetButtonsLayout);

    visualizerLayout->addStretch();
    visualizerDock->setWidget(visualizerWidget);
    _window->addDockWidget(Qt::RightDockWidgetArea, visualizerDock);

    // Connect buttons
    connect(addFileButton, &QPushButton::clicked, this, &QtGui::add_audio_file);
    connect(removeFileButton, &QPushButton::clicked, this, &QtGui::remove_selected_audio_file);

    // Connect for reordering
    connect(playlistWidget->model(), &QAbstractItemModel::rowsMoved, this, &QtGui::playlist_reordered);

    // Connect playback buttons
    connect(playButton, &QPushButton::clicked, this, &QtGui::play_audio);
    connect(pauseButton, &QPushButton::clicked, this, &QtGui::pause_audio);
    connect(stopButton, &QPushButton::clicked, this, &QtGui::stop_audio);
    connect(nextButton, &QPushButton::clicked, this, &QtGui::next_audio);
    connect(prevButton, &QPushButton::clicked, this, &QtGui::prev_audio);

    // Connect preset buttons
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
    currentPresetLabel->setText(tr("Current Preset: ") + QString::fromStdString(_core.get_current_preset_name()));
}

void QtGui::prev_preset() {
    _core.prev_preset();
    currentPresetLabel->setText(tr("Current Preset: ") + QString::fromStdString(_core.get_current_preset_name()));
}

void QtGui::about() {
    QMessageBox::about(_window.get(), tr("About Aurora Visualizer"),
        tr("<h2>Aurora Visualizer</h2>"
           "<p>Version 0.1.0-dev</p>"
           "<p>A cross-platform audio visualizer using projectM.</p>"));
}
