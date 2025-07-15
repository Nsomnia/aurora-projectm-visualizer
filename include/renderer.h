#pragma once

#include "Config.h"
#include <projectM-4/projectM.h>
#include <GL/glew.h>
#include <SDL.h>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(Config& config);
    void render(projectm_handle pM);
    std::vector<unsigned char> get_framebuffer_pixels(int width, int height);
    void cleanup();

private:
    SDL_GLContext _context;
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;

    bool create_fbo(int width, int height);
    void render_to_fbo(projectm_handle pM);
};