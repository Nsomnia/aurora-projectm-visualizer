#include "renderer.h"
#include "utils/Logger.h"
#include <stdexcept>
#include <vector>
#include <QOpenGLWidget>

namespace {
const char* vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}
)";

const char* fragment_shader_source = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
void main() {
    FragColor = texture(screenTexture, TexCoords);
}
)";
}

Renderer::Renderer(Config& config)
    : _config(config)
    , _gl_widget(nullptr)
    , _fbo(0)
    , _fbo_texture(0)
    , _rbo(0)
    , _quad_vao(0)
    , _quad_vbo(0)
    , _shader_program(0)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::init(QOpenGLWidget* openGLWidget)
{
    _gl_widget = openGLWidget;
    _gl_widget->makeCurrent();

    if (glewInit() != GLEW_OK) {
        Logger::error("GLEW initialization failed.");
        return false;
    }

    glViewport(0, 0, _config.width, _config.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        Logger::error("OpenGL Error after glClearColor: " + std::to_string(error));
        return false;
    }

    if (!create_fbo(_config.width, _config.height)) {
        return false;
    }

    if (!create_shader()) {
        return false;
    }

    float quad_vertices[] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    return true;
}

void Renderer::cleanup()
{
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
    }
    if (_fbo_texture) {
        glDeleteTextures(1, &_fbo_texture);
    }
    if (_rbo) {
        glDeleteRenderbuffers(1, &_rbo);
    }
    if (_shader_program) {
        glDeleteProgram(_shader_program);
    }
    if (_quad_vao) {
        glDeleteVertexArrays(1, &_quad_vao);
    }
    if (_quad_vbo) {
        glDeleteBuffers(1, &_quad_vbo);
    }
}

void Renderer::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::present()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_shader_program);
    glBindVertexArray(_quad_vao);
    glBindTexture(GL_TEXTURE_2D, _fbo_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Renderer::create_fbo(int width, int height)
{
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

bool Renderer::create_shader()
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        Logger::error("Vertex shader compilation failed: " + std::string(info_log));
        return false;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        Logger::error("Fragment shader compilation failed: " + std::string(info_log));
        return false;
    }

    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, vertex_shader);
    glAttachShader(_shader_program, fragment_shader);
    glLinkProgram(_shader_program);

    glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shader_program, 512, NULL, info_log);
        Logger::error("Shader program linking failed: " + std::string(info_log));
        return false;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
}