# PresetManager Component Improvements

This document outlines potential improvements for the `PresetManager` component.

## `src/PresetManager.cpp` / `include/visualizer/preset_manager.h`

*   **Duplicated Path Resolution:**
    *   **Issue:** The logic for resolving `~` (home directory) in paths is duplicated here and in `ConfigLoader.cpp`.
    *   **Suggestion:** Extract this into a common utility function in `visualizer/utils/common.h` (e.g., `resolve_home_path`).
*   **Repetitive List Removal:**
    *   **Issue:** The logic for removing a preset from `_all_presets`, `_favorite_presets`, and `_history` in `mark_current_preset_as_broken()` is repetitive.
    *   **Suggestion:** Create a private helper function (e.g., `remove_preset_from_all_lists(const std::string& preset_path)`) to encapsulate this common operation.
*   **Magic Number `MAX_HISTORY_SIZE`:**
    *   **Issue:** `const int MAX_HISTORY_SIZE = 50;` is a magic number.
    *   **Suggestion:** Define this as a `static const int` member in the header or a `constexpr` if possible, or make it configurable via `Config`.
