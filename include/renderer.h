#pragma once

#include "Config.h"
#include <QOpenGLFunctions_3_3_Core>
#include <projectM-4/projectM.h>
#include <vector>

class Renderer : protected QOpenGLFunctions_3_3_Core {
public:
    Renderer();
    ~Renderer();

    bool init(Config& config);
    void render(projectm_handle pM);
    void resize(int w, int h);
    std::vector<unsigned char> get_framebuffer_pixels(int width, int height);
    void cleanup();

private:
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;
    GLuint _quad_vao;
    GLuint _quad_vbo;
    GLuint _shader_program;

    bool create_fbo(int width, int height);
    void render_to_fbo(projectm_handle pM);
    bool create_shaders();
    std::string read_shader_file(const std::string& path);
};
