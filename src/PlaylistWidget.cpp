#include "PlaylistWidget.h"
#include <QVBoxLayout>

PlaylistWidget::PlaylistWidget(Config& config, QWidget* parent)
    : QDockWidget("Playlist", parent),
      _config(config)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    _listWidget = new QListWidget(this);
    for (const auto& path : _config.audio_file_paths) {
        _listWidget->addItem(QString::fromStdString(path));
    }

    setWidget(_listWidget);
}

PlaylistWidget::~PlaylistWidget()
{
}
