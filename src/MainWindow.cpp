#include "MainWindow.h"
#include "ProjectMWidget.h"
#include "PlaylistWidget.h"
#include "SettingsWidget.h"
#include "AboutDialog.h"
#include <QMenuBar>

MainWindow::MainWindow(Config& config, ProjectMWidget* projectMWidget, QWidget* parent)
    : QMainWindow(parent),
      _projectMWidget(projectMWidget)
{
    setCentralWidget(_projectMWidget);

    _playlistWidget = new PlaylistWidget(config, this);
    addDockWidget(Qt::RightDockWidgetArea, _playlistWidget);

    _settingsWidget = new SettingsWidget(config, this);
    addDockWidget(Qt::LeftDockWidgetArea, _settingsWidget);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(exitAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    helpMenu->addAction(aboutAction);
}

void MainWindow::showAboutDialog()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

MainWindow::~MainWindow()
{
}
