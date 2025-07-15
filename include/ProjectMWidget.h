#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "core.h"
#include "renderer.h" // Assuming renderer.h is the header for your Renderer class

class ProjectMWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit ProjectMWidget(Core& core, QWidget* parent = nullptr);
    ~ProjectMWidget() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    Core& _core;
    Renderer _renderer;
    bool _is_initialized;
};
