// src/TextManager.cpp
#include "TextManager.h"
#include <algorithm>
#include <regex>
#include <sstream> // Required for std::stringstream

TextManager::TextManager(TextRenderer& textRenderer) : _textRenderer(textRenderer) {}

std::string TextManager::sanitize_text(const std::string& text) {
    std::string sanitized = text;

    // Remove file extension
    size_t dot_pos = sanitized.rfind('.');
    if (dot_pos != std::string::npos) {
        sanitized = sanitized.substr(0, dot_pos);
    }

    // Normalize separators
    std::replace(sanitized.begin(), sanitized.end(), '_', ' ');
    std::replace(sanitized.begin(), sanitized.end(), '-', ' ');

    // Keep only allowed characters
    std::string result;
    for (char c : sanitized) {
        // removed this manually because songs often have round brackets c == '(' || c == ')') {
        if (isalnum(c) || c == ' ' || c == '[' || c == ']') {
            result += c;
        }
    }

    return result;
}

std::vector<std::string> TextManager::split_text(const std::string& text, int window_width, float scale) {
    std::string sanitized = sanitize_text(text);
    std::vector<std::string> lines;
    if (sanitized.empty()) {
        return lines;
    }

    std::string current_line;
    std::stringstream ss(sanitized);
    std::string word;

    const float max_width = window_width * 0.5f;

    while (ss >> word) {
        std::string test_line = current_line.empty() ? word : current_line + " " + word;
        if (_textRenderer.getTextWidth(test_line, scale) > max_width) {
            if (!current_line.empty()) {
                lines.push_back(current_line);
                current_line = word;
            } else {
                // The word itself is longer than the max width, so we must split it.
                std::string temp_word = word;
                while (_textRenderer.getTextWidth(temp_word, scale) > max_width) {
                    size_t split_pos = 0;
                    // Find the last character that fits
                    for (size_t i = 1; i <= temp_word.length(); ++i) {
                         if (_textRenderer.getTextWidth(temp_word.substr(0, i), scale) > max_width) {
                            split_pos = i - 1;
                            break;
                        }
                    }
                    // If we can't even fit one character, something is wrong.
                    // To prevent infinite loops, we'll just push the first character.
                    if (split_pos == 0 && !temp_word.empty()) {
                        split_pos = 1;
                    }
                    lines.push_back(temp_word.substr(0, split_pos));
                    temp_word = temp_word.substr(split_pos);
                }
                current_line = temp_word;
            }
        } else {
            current_line = test_line;
        }
    }

    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}
