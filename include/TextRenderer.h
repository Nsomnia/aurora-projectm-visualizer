#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <QOpenGLFunctions_3_3_Core>

struct Character {
    unsigned int textureID;
    glm::ivec2   size;
    glm::ivec2   bearing;
    unsigned int advance;
};

class TextRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    TextRenderer();
    ~TextRenderer();

    bool init(const std::string& fontPath, int fontSize);
    void setProjection(int width, int height);
    void cleanup();
    void renderText(const std::string& text, float x, float y, float scale, 
                    const glm::vec3& color, float alpha = 1.0f, 
                    bool show_border = true, const glm::vec3& border_color = glm::vec3(0.0f), 
                    float border_thickness = 0.1f);
    float getTextWidth(const std::string& text, float scale);
    glm::vec4 getTextBounds(const std::string& text, float x, float y, float scale);
    bool is_initialized() const { return _initialized; }

private:
    bool initShaders();
    void renderTextPass(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, const glm::vec3& border_color, float border_thickness);

    GLuint _shaderProgram;
    GLuint _vao, _vbo;
    FT_Library _ft;
    FT_Face _face;
    std::map<char, Character> _characters;
    int _width, _height;
    bool _initialized;
};
