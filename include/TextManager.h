#pragma once

#include "TextRenderer.h"
#include <string>
#include <vector>

class TextManager {
public:
    TextManager(TextRenderer& textRenderer);

    std::vector<std::string> split_text(const std::string& text, int window_width, float scale);

private:
    std::string sanitize_text(const std::string& text);

    TextRenderer& _textRenderer;
};
