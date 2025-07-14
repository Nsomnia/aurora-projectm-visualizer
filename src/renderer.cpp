#include "renderer.h"
#include <iostream>

Renderer::Renderer() : _window(nullptr), _context(nullptr), _fbo(0), _fbo_texture(0), _rbo(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(SDL_Window* window, SDL_GLContext* context, Config& config) {
    _window = window;
    _context = context;

    glViewport(0, 0, config.width, config.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after glClearColor: " << error << std::endl;
        return false;
    }

    return create_fbo(config.width, config.height);
}

void Renderer::render(projectm_handle pM) {
    render_to_fbo(pM);
}

void Renderer::cleanup() {
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
    }
    if (_fbo_texture) {
        glDeleteTextures(1, &_fbo_texture);
    }
    if (_rbo) {
        glDeleteRenderbuffers(1, &_rbo);
    }
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
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
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
