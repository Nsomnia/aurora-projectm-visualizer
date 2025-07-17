#pragma once

#include "Config.h"
#include <projectM-4/projectM.h>
#include <GL/glew.h>
#include <vector>

class QOpenGLWidget;

class Renderer {
public:
    Renderer(Config& config);
    ~Renderer();

    bool init(QOpenGLWidget* openGLWidget);
    void render(projectm_handle pM);
    std::vector<unsigned char> get_framebuffer_pixels(int width, int height);
    void cleanup();

private:
    Config& _config;
    QOpenGLWidget* _gl_widget;
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;

    bool create_fbo(int width, int height);
    void render_to_fbo(projectm_handle pM);
};