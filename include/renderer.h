#pragma once

#include "Config.h"
#include "TextRenderer.h"
#include "AnimationManager.h"
#include <SDL.h>
#include <projectM-4/projectM.h>
#include <GL/glew.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(SDL_Window* window, SDL_GLContext* context, Config& config);
    void render(projectm_handle pM);
    void cleanup();

    bool create_fbo(int width, int height);
    GLuint get_fbo_texture() const { return _fbo_texture; }

private:
    void render_to_fbo(projectm_handle pM);

    SDL_Window* _window;
    SDL_GLContext _context;
    GLuint _fbo;
    GLuint _fbo_texture;
    GLuint _rbo;
};