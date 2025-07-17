#pragma once

#include <QDockWidget>
#include "Config.h"

class SettingsWidget : public QDockWidget
{
    Q_OBJECT

public:
    SettingsWidget(Config& config, QWidget* parent = nullptr);
    ~SettingsWidget();

private:
    Config& _config;
};
