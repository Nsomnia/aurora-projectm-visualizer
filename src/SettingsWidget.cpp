#include "SettingsWidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>

SettingsWidget::SettingsWidget(Config& config, QWidget* parent)
    : QDockWidget("Settings", parent),
      _config(config)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* mainWidget = new QWidget(this);
    QFormLayout* layout = new QFormLayout(mainWidget);

    QSpinBox* widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(1, 8192);
    widthSpinBox->setValue(_config.width);
    connect(widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
        _config.width = value;
    });
    layout->addRow("Width", widthSpinBox);

    QSpinBox* heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(1, 8192);
    heightSpinBox->setValue(_config.height);
    connect(heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
        _config.height = value;
    });
    layout->addRow("Height", heightSpinBox);

    QSpinBox* fpsSpinBox = new QSpinBox(this);
    fpsSpinBox->setRange(1, 240);
    fpsSpinBox->setValue(_config.fps);
    connect(fpsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
        _config.fps = value;
    });
    layout->addRow("FPS", fpsSpinBox);

    setWidget(mainWidget);
}

SettingsWidget::~SettingsWidget()
{
}
