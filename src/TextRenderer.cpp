#include "TextRenderer.h"
#include "utils/Logger.h"
#include "utils/gl_utils.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D text;
uniform vec3 textColor;
uniform float alpha;
uniform vec3 borderColor;
uniform float borderThickness;
void main() {
    float distance = texture(text, TexCoords).r;
    float smoothWidth = fwidth(distance);
    float outline = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);
    float border = smoothstep(0.5 - borderThickness - smoothWidth, 0.5 - borderThickness + smoothWidth, distance);
    vec4 blendedColor = mix(vec4(borderColor, alpha), vec4(textColor, alpha), outline);
    color = vec4(blendedColor.rgb, alpha * (outline + (1.0 - outline) * border));
}
)glsl";

TextRenderer::TextRenderer() : _shaderProgram(0), _vao(0), _vbo(0), _ft(nullptr), _face(nullptr), _width(0), _height(0), _initialized(false) {}

TextRenderer::~TextRenderer() {
    cleanup();
}

void TextRenderer::cleanup() {
    if (_shaderProgram) glDeleteProgram(_shaderProgram);
    for (auto const& [key, val] : _characters) {
        glDeleteTextures(1, &val.textureID);
    }
    _characters.clear();
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_face) FT_Done_Face(_face);
    if (_ft) FT_Done_FreeType(_ft);
}

bool TextRenderer::init(const std::string& fontPath, int fontSize) {
    initializeOpenGLFunctions();
    if (FT_Init_FreeType(&_ft)) return false;
    if (FT_New_Face(_ft, fontPath.c_str(), 0, &_face)) return false;
    FT_Set_Pixel_Sizes(_face, 0, fontSize);

    if (!initShaders()) return false;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(_face, c, FT_LOAD_RENDER)) continue;
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _face->glyph->bitmap.width, _face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, _face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _characters[c] = {texture, glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows), glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top), static_cast<unsigned int>(_face->glyph->advance.x)};
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _initialized = true;
    return true;
}

void TextRenderer::setProjection(int width, int height) {
    _width = width;
    _height = height;
    glUseProgram(_shaderProgram);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(_width), 0.0f, static_cast<float>(_height));
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUseProgram(0);
}

void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, bool show_border, const glm::vec3& border_color, float border_thickness) {
    Logger::info("TextRenderer::renderText() called for text: " + text);
    // NOTE: The contrast adjustment logic was causing heap corruption and has been disabled.
    float final_border_thickness = show_border ? border_thickness : 0.0f;
    renderTextPass(text, x, y, scale, color, alpha, border_color, final_border_thickness);
}

glm::vec4 TextRenderer::getTextBounds(const std::string& text, float x, float y, float scale) {
    float min_x = x, max_x = x;
    float min_y = y, max_y = y;

    for (const char& c : text) {
        Character ch = _characters[c];
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        min_x = std::min(min_x, xpos);
        max_x = std::max(max_x, xpos + w);
        min_y = std::min(min_y, ypos);
        max_y = std::max(max_y, ypos + h);
        
        x += (ch.advance >> 6) * scale;
    }
    return glm::vec4(min_x, min_y, max_x - min_x, max_y - min_y);
}

void TextRenderer::renderTextPass(const std::string& text, float x, float y, float scale, const glm::vec3& color, float alpha, const glm::vec3& border_color, float border_thickness) {
    CHECK_GL_ERROR();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(_shaderProgram);
    CHECK_GL_ERROR();
    glUniform3f(glGetUniformLocation(_shaderProgram, "textColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(_shaderProgram, "alpha"), alpha);
    glUniform3f(glGetUniformLocation(_shaderProgram, "borderColor"), border_color.x, border_color.y, border_color.z);
    glUniform1f(glGetUniformLocation(_shaderProgram, "borderThickness"), border_thickness);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);
    CHECK_GL_ERROR();

    for (const char& c : text) {
        Character ch = _characters[c];
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f }, { xpos,     ypos,       0.0f, 1.0f }, { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f }, { xpos + w, ypos,       1.0f, 1.0f }, { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }
    CHECK_GL_ERROR();

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glDisable(GL_BLEND);
    CHECK_GL_ERROR();
}

float TextRenderer::getTextWidth(const std::string& text, float scale) {
    float width = 0.0f;
    for (const char& c : text) {
        width += (_characters[c].advance >> 6) * scale;
    }
    return width;
}

bool TextRenderer::initShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        return false;
    }

    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}
