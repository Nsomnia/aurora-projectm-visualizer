#pragma once

#include <QDockWidget>
#include <QListWidget>
#include "Config.h"

class PlaylistWidget : public QDockWidget
{
    Q_OBJECT

public:
    PlaylistWidget(Config& config, QWidget* parent = nullptr);
    ~PlaylistWidget();

private:
    Config& _config;
    QListWidget* _listWidget;
};
