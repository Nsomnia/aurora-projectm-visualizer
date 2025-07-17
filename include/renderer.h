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
    void cleanup();

    void begin();
    void end();
    void present();
    GLuint fboTexture() const { return _fbo_texture; }


private:
    Config& _config;
    QOpenGLWidget* _gl_widget;
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;
    GLuint _quad_vao;
    GLuint _quad_vbo;
    GLuint _shader_program;

    bool create_fbo(int width, int height);
    bool create_shader();
};