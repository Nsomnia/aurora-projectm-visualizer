#include "ProjectMWidget.h"
#include <GL/glew.h>

ProjectMWidget::ProjectMWidget(Config& config, projectm_handle pM, TextRenderer& textRenderer, TextManager& textManager, AnimationManager& animationManager, QWidget *parent)
    : QOpenGLWidget(parent),
      _config(config),
      _pM(pM),
      _text_renderer(textRenderer),
      _text_manager(textManager),
      _animation_manager(animationManager),
      _renderer(config)
{
}

ProjectMWidget::~ProjectMWidget()
{
}

void ProjectMWidget::initializeGL()
{
    initializeOpenGLFunctions();
    _renderer.init(this);
}

void ProjectMWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projectm_set_window_size(_pM, w, h);
    _text_renderer.setProjection(w, h);
}

void ProjectMWidget::paintGL()
{
    _renderer.begin();
    projectm_opengl_render_frame(_pM);

    if (_config.show_title) {
        std::vector<std::string> title_lines = _text_manager.split_text(_config.title, _config.width, 1.0f);
        _animation_manager.render(title_lines);
    }

    _renderer.end();
    _renderer.present();
}
