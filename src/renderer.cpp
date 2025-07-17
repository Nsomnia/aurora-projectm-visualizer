#include "renderer.h"
#include "utils/Logger.h"
#include "utils/gl_utils.h"
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>

Renderer::Renderer() : _fbo(0), _fbo_texture(0), _rbo(0), _quad_vao(0), _quad_vbo(0), _shader_program(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(Config& config) {
    initializeOpenGLFunctions();

    if (!create_shaders()) {
        Logger::error("Failed to create shaders.");
        return false;
    }

    glViewport(0, 0, config.width, config.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Create quad for drawing FBO
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &_quad_vao);
    glGenBuffers(1, &_quad_vbo);
    glBindVertexArray(_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    return create_fbo(config.width, config.height);
}

void Renderer::render(projectm_handle pM) {
    Logger::info("Renderer::render() called");
    // 1. Render projectM to our FBO
    render_to_fbo(pM);
    CHECK_GL_ERROR();

    // 2. Render our FBO to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_GL_ERROR();

    glUseProgram(_shader_program);
    glBindVertexArray(_quad_vao);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, _fbo_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();
}

void Renderer::resize(int w, int h) {
    glViewport(0, 0, w, h);

    // Delete old FBO resources
    if (_fbo) glDeleteFramebuffers(1, &_fbo);
    if (_fbo_texture) glDeleteTextures(1, &_fbo_texture);
    if (_rbo) glDeleteRenderbuffers(1, &_rbo);

    // Recreate FBO with new size
    create_fbo(w, h);
}

std::vector<unsigned char> Renderer::get_framebuffer_pixels(int width, int height) {
    std::vector<unsigned char> pixels(width * height * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pixels;
}

void Renderer::cleanup() {
    if (_fbo) glDeleteFramebuffers(1, &_fbo);
    if (_fbo_texture) glDeleteTextures(1, &_fbo_texture);
    if (_rbo) glDeleteRenderbuffers(1, &_rbo);
    if (_shader_program) glDeleteProgram(_shader_program);
    if (_quad_vao) glDeleteVertexArrays(1, &_quad_vao);
    if (_quad_vbo) glDeleteBuffers(1, &_quad_vbo);
}

bool Renderer::create_fbo(int width, int height) {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_fbo_texture);
    glBindTexture(GL_TEXTURE_2D, _fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbo_texture, 0);

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::error("Framebuffer is not complete!");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Renderer::render_to_fbo(projectm_handle pM) {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectm_opengl_render_frame(pM);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::string Renderer::read_shader_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::error("Could not open shader file: " + path);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Renderer::create_shaders() {
    std::string vert_source = read_shader_file("src/fbo.vert");
    std::string frag_source = read_shader_file("src/fbo.frag");

    if (vert_source.empty() || frag_source.empty()) {
        return false;
    }

    const char* vert_source_c = vert_source.c_str();
    const char* frag_source_c = frag_source.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_source_c, NULL);
    glCompileShader(vertex_shader);
    // ... error checking ...

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_source_c, NULL);
    glCompileShader(fragment_shader);
    // ... error checking ...

    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, vertex_shader);
    glAttachShader(_shader_program, fragment_shader);
    glLinkProgram(_shader_program);
    // ... error checking ...

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
}