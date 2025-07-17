#pragma once

#include <QMainWindow>

class ProjectMWidget;
class PlaylistWidget;
struct Config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Config& config, ProjectMWidget* projectMWidget, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void showAboutDialog();

private:
    ProjectMWidget* _projectMWidget;
    PlaylistWidget* _playlistWidget;
    SettingsWidget* _settingsWidget;
};
