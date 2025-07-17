#pragma once

#include "core.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class QtOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    explicit QtOpenGLWidget(Core& core, QWidget* parent = nullptr);
    ~QtOpenGLWidget() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    Core& _core;
};
