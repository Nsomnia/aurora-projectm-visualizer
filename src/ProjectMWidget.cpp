#include "ProjectMWidget.h"
#include <GL/glew.h>

ProjectMWidget::ProjectMWidget(Config& config, projectm_handle pM, TextRenderer& textRenderer, TextManager& textManager, AnimationManager& animationManager, QWidget *parent)
    : QOpenGLWidget(parent),
      _config(config),
      _pM(pM),
      _text_renderer(textRenderer),
      _text_manager(textManager),
      _animation_manager(animationManager)
{
}

ProjectMWidget::~ProjectMWidget()
{
}

void ProjectMWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glewInit();
}

void ProjectMWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projectm_set_window_size(_pM, w, h);
    _text_renderer.setProjection(w, h);
}

void ProjectMWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectm_opengl_render_frame(_pM);

    // This is where the text rendering would go
}
