#include "renderer.h"
#include "utils/Logger.h"
#include "utils/gl_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer() : _config(nullptr), _fbo(0), _fbo_texture(0), _rbo(0), _quad_vao(0), _quad_vbo(0), _shader_program(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(Config& config) {
    _config = &config;
    initializeOpenGLFunctions();
    glViewport(0, 0, config.width, config.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!_config->disable_fbo) {
        if (!create_shaders()) {
            Logger::error("Failed to create shaders.");
            return false;
        }
        if (!create_fbo(config.width, config.height)) {
            Logger::error("Failed to create FBO.");
            return false;
        }
    }
    return true;
}

void Renderer::render(projectm_handle pM) {
    if (!_config->disable_fbo) {
        // FBO path
        render_to_fbo(pM);

        // Now render the FBO texture to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, _config->width, _config->height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(_shader_program);
        glBindVertexArray(_quad_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _fbo_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    } else {
        // Non-FBO path
        glViewport(0, 0, _config->width, _config->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Safely call the legacy rendering function
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        projectm_opengl_render_frame(pM);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glPopAttrib();
    }
    CHECK_GL_ERROR();
}

void Renderer::resize(int w, int h) {
    glViewport(0, 0, w, h);
    if (!_config->disable_fbo) {
        // Recreate FBO with new dimensions
        cleanup();
        init(*_config);
    }
}

void Renderer::cleanup() {
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = 0;
    }
    if (_fbo_texture) {
        glDeleteTextures(1, &_fbo_texture);
        _fbo_texture = 0;
    }
    if (_rbo) {
        glDeleteRenderbuffers(1, &_rbo);
        _rbo = 0;
    }
    if (_shader_program) {
        glDeleteProgram(_shader_program);
        _shader_program = 0;
    }
    if (_quad_vao) {
        glDeleteVertexArrays(1, &_quad_vao);
        _quad_vao = 0;
    }
    if (_quad_vbo) {
        glDeleteBuffers(1, &_quad_vbo);
        _quad_vbo = 0;
    }
}

std::vector<unsigned char> Renderer::get_framebuffer_pixels(int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);
    if (!_config->disable_fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    if (!_config->disable_fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    return pixels;
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
    glViewport(0, 0, _config->width, _config->height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Safely call the legacy rendering function
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    projectm_opengl_render_frame(pM);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Renderer::create_shaders() {
    std::string vert_shader_str = read_shader_file("src/fbo.vert");
    std::string frag_shader_str = read_shader_file("src/fbo.frag");

    if (vert_shader_str.empty() || frag_shader_str.empty()) {
        return false;
    }

    const char* vert_shader_src = vert_shader_str.c_str();
    const char* frag_shader_src = frag_shader_str.c_str();

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_src, NULL);
    glCompileShader(vert_shader);
    // ... (error checking)

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_src, NULL);
    glCompileShader(frag_shader);
    // ... (error checking)

    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, vert_shader);
    glAttachShader(_shader_program, frag_shader);
    glLinkProgram(_shader_program);
    // ... (error checking)

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    // Create quad VAO
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

std::string Renderer::read_shader_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::error("Failed to open shader file: " + path);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
