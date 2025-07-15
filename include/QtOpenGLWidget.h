#pragma once

#include <QWidget>
#include "core.h"

class QtOpenGLWidget : public QWidget {
    Q_OBJECT

public:
    explicit QtOpenGLWidget(Core& core, QWidget* parent = nullptr);
    ~QtOpenGLWidget() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    Core& _core;
};