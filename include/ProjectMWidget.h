#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "Config.h"
#include <projectM-4/projectM.h>
#include "TextRenderer.h"
#include "TextManager.h"
#include "AnimationManager.h"

class ProjectMWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ProjectMWidget(Config& config, projectm_handle pM, TextRenderer& textRenderer, TextManager& textManager, AnimationManager& animationManager, QWidget *parent = nullptr);
    ~ProjectMWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Config& _config;
    projectm_handle _pM;
    TextRenderer& _text_renderer;
    TextManager& _text_manager;
    AnimationManager& _animation_manager;
};
