# AnimationManager Component Improvements

This document outlines potential improvements for the `AnimationManager` component.

## `src/AnimationManager.cpp` / `include/visualizer/AnimationManager.h`

*   **Code Duplication in `updateBouncing` and `updateReturning`:**
    *   **Issue:** The logic for updating and clamping positions/velocities is duplicated for `_titleBlockPosition` and `_artistPosition`.
    *   **Suggestion:** Create a private helper function or a small struct/class (e.g., `AnimatableTextElement`) that encapsulates `glm::vec2 position`, `glm::vec2 velocity`, and methods to `update_position(deltaTime)` and `handle_bounce(min_x, max_x, min_y, max_y, randomness)`. This would significantly reduce code duplication.
*   **`initializePositions` Readability:**
    *   **Issue:** The calculation of `max_width` and `total_height` for the title block is embedded within `initializePositions`.
    *   **Suggestion:** Extract the logic for calculating the overall dimensions of the title block into a separate private helper function (e.g., `calculate_title_block_dimensions`).
*   **Randomness Usage:**
    *   **Issue:** `glm::linearRand` is used for randomness.
    *   **Suggestion:** Ensure `glm` is consistently used for all vector/matrix operations. No major refactoring needed here, but good to note for consistency.
