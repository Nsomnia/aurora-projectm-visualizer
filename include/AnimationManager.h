#pragma once

#include "Config.h"
#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>

enum class AnimationState {
    BOUNCING,
    FADING_OUT,
    FADING_IN,
    FINISHED
};

class AnimationManager {
public:
    AnimationManager(Config& config, TextRenderer& textRenderer);

    void reset(const std::vector<std::string>& title_lines);
    void update(double music_len, double current_time, const std::vector<std::string>& title_lines);
    void render(const std::vector<std::string>& title_lines);

    std::vector<glm::vec2> getTitlePositions(const std::vector<std::string>& title_lines) const;
    glm::vec2 getArtistPosition() const;
    float getAlpha() const;
    float getBreathingScale() const;

private:
    void initializePositions(const std::vector<std::string>& title_lines);
    void updateBouncing(float deltaTime, const std::vector<std::string>& title_lines);
    void updateReturning(float deltaTime);

    Config& _config;
    TextRenderer& _textRenderer;

    glm::vec2 _initialTitleBlockPosition;
    std::vector<glm::vec2> _titleLineOffsets;
    glm::vec2 _initialArtistPosition;

    glm::vec2 _titleBlockPosition;
    glm::vec2 _artistPosition;

    glm::vec2 _titleBlockVelocity;
    glm::vec2 _artistVelocity;

    float _alpha;
    float _breathingScale;
    AnimationState _currentState;
};
