// src/AnimationManager.cpp
#include "AnimationManager.h"
#include "TextRenderer.h"
#include <glm/gtc/random.hpp>
#include <algorithm> // For std::min/max
#include <iostream>
#include <cmath>

AnimationManager::AnimationManager(Config &config, TextRenderer &textRenderer)
    : _config(config), _textRenderer(textRenderer), _artistPosition(0.0f),
      _artistVelocity(0.0f), _alpha(1.0f), _breathingScale(1.0f),
      _currentState(AnimationState::BOUNCING) {}

void AnimationManager::reset(const std::vector<std::string>& title_lines) {
    initializePositions(title_lines);
    _titleBlockPosition = _initialTitleBlockPosition;
    _artistPosition = _initialArtistPosition;

    _titleBlockVelocity = glm::linearRand(glm::vec2(-_config.bounce_speed, -_config.bounce_speed), glm::vec2(_config.bounce_speed, _config.bounce_speed));
    _artistVelocity = glm::linearRand(glm::vec2(-_config.bounce_speed, -_config.bounce_speed), glm::vec2(_config.bounce_speed, _config.bounce_speed));

    _alpha = 1.0f;
    _breathingScale = 1.0f;
    _currentState = AnimationState::BOUNCING;
}

void AnimationManager::initializePositions(const std::vector<std::string>& title_lines) {
    _titleLineOffsets.clear();
    float total_height = title_lines.size() * _config.songInfoFontSize;
    float max_width = 0;
    for (const auto& line : title_lines) {
        max_width = std::max(max_width, _textRenderer.getTextWidth(line, 1.0f));
    }

    _initialTitleBlockPosition = {
        (_config.width - max_width) / 2.0f,
        (_config.height / 2.0f) + (total_height / 2.0f)
    };

    float current_y = 0;
    for (const auto& line : title_lines) {
        float titleWidth = _textRenderer.getTextWidth(line, 1.0f);
        _titleLineOffsets.push_back({
            (max_width - titleWidth) / 2.0f,
            current_y
        });
        current_y -= _config.songInfoFontSize;
    }

    float artistWidth = _textRenderer.getTextWidth(_config.artistName, 1.0f);
    _initialArtistPosition = {
        (_config.width - artistWidth) / 2.0f,
        _initialTitleBlockPosition.y - total_height - _config.songInfoFontSize
    };
}

void AnimationManager::update(double music_len, double current_time, const std::vector<std::string>& title_lines) {
    if (music_len <= 0) return;

    float deltaTime = 1.0f / _config.fps;
    float time_until_end = music_len - current_time;

    // Breathing effect
    if (_config.text_breathing_effect) {
        float sine_wave = sin(current_time * _config.breathing_effect_speed);
        _breathingScale = 1.0f + sine_wave * _config.breathing_effect_amount;
    }

    // State transitions
    switch (_currentState) {
        case AnimationState::BOUNCING:
            if (current_time >= _config.bounce_duration) {
                _currentState = AnimationState::FADING_OUT;
            }
            updateBouncing(deltaTime, title_lines);
            break;
        case AnimationState::FADING_OUT:
            {
                float fade_progress = (current_time - _config.bounce_duration) / _config.transitionTime;
                _alpha = 1.0f - fade_progress;
                if (_alpha <= 0.0f) {
                    _currentState = AnimationState::FADING_IN;
                }
                updateBouncing(deltaTime, title_lines);
            }
            break;
        case AnimationState::FADING_IN:
            {
                float fade_progress = (current_time - (_config.bounce_duration + _config.transitionTime)) / _config.transitionTime;
                _alpha = fade_progress;
                if (_alpha >= 1.0f) {
                    _currentState = AnimationState::FINISHED;
                }
                updateBouncing(deltaTime, title_lines);
            }
            break;
        case AnimationState::FINISHED:
            // Animation is done, do nothing.
            break;
    }
     _alpha = std::max(0.0f, std::min(1.0f, _alpha));
}


void AnimationManager::updateBouncing(float deltaTime, const std::vector<std::string>& title_lines) {
    // Single update for the entire text block
    _titleBlockPosition += _titleBlockVelocity * deltaTime;

    float block_width = 0;
    for(const auto& line : title_lines) {
        block_width = std::max(block_width, _textRenderer.getTextWidth(line, 1.0f));
    }
    float block_height = title_lines.size() * _config.songInfoFontSize;

    if (_titleBlockPosition.x < 0 || _titleBlockPosition.x + block_width > _config.width) {
        _titleBlockVelocity.x *= -1;
        _titleBlockVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _titleBlockPosition.x = std::max(0.0f, std::min(_titleBlockPosition.x, _config.width - block_width));
    }
    float half_block_height = block_height / 2.0f;
    if (_titleBlockPosition.y - half_block_height < 0 || _titleBlockPosition.y + half_block_height > _config.height) {
        _titleBlockVelocity.y *= -1;
        _titleBlockVelocity += glm::linearRand(glm::vec2(-_config.bounce_randomness, -_config.bounce_randomness), glm::vec2(_config.bounce_randomness, _config.bounce_randomness));
        _titleBlockPosition.y = std::max(half_block_height, std::min(_titleBlockPosition.y, (float)_config.height - half_block_height));
    }

    // Artist position is now relative to the title block
    float artistWidth = _textRenderer.getTextWidth(_config.artistName, 1.0f);
    _artistPosition = {
        _titleBlockPosition.x + (block_width - artistWidth) / 2.0f,
        _titleBlockPosition.y - block_height - _config.songInfoFontSize
    };
}

void AnimationManager::updateReturning(float deltaTime) {
    glm::vec2 direction = glm::normalize(_initialTitleBlockPosition - _titleBlockPosition);
    _titleBlockPosition += direction * _config.bounce_speed * deltaTime;

    glm::vec2 artistDirection = glm::normalize(_initialArtistPosition - _artistPosition);
    _artistPosition += artistDirection * _config.bounce_speed * deltaTime;
}

std::vector<glm::vec2> AnimationManager::getTitlePositions(const std::vector<std::string>& title_lines) const {
    std::vector<glm::vec2> positions;
    for (size_t i = 0; i < title_lines.size(); ++i) {
        positions.push_back(_titleBlockPosition + _titleLineOffsets[i]);
    }
    return positions;
}

glm::vec2 AnimationManager::getArtistPosition() const {
  return _artistPosition;
}

float AnimationManager::getAlpha() const { return _alpha; }

float AnimationManager::getBreathingScale() const { return _breathingScale; }

void AnimationManager::render(const std::vector<std::string>& title_lines) {
    if (!_textRenderer.is_initialized()) {
        return;
    }

    std::vector<glm::vec2> title_positions = getTitlePositions(title_lines);
    for (size_t i = 0; i < title_lines.size(); ++i) {
        _textRenderer.renderText(title_lines[i], title_positions[i].x, title_positions[i].y,
                                 getBreathingScale(), _config.songInfoColor, getAlpha(),
                                 _config.show_border, _config.songInfoBorderColor, _config.songInfoBorderThickness);
    }

    glm::vec2 artist_position = getArtistPosition();
    _textRenderer.renderText(_config.artistName, artist_position.x, artist_position.y,
                             getBreathingScale(), _config.urlColor, getAlpha(),
                             _config.show_border, _config.urlBorderColor, _config.urlBorderThickness);
}