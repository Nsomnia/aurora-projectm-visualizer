# TextRenderer Component Improvements

This document outlines potential improvements for the `TextRenderer` component.

## `src/TextRenderer.cpp` / `include/visualizer/TextRenderer.h`

*   **Embedded Shader Source:**
    *   **Issue:** Vertex and fragment shader source code is embedded as `const char*` strings.
    *   **Suggestion:** For larger or more complex shaders, consider loading them from external files. This improves readability, allows for easier editing with syntax highlighting, and separates shader logic from C++ code.
*   **`renderText` and `renderTextPass`:**
    *   **Issue:** `renderText` acts as a wrapper around `renderTextPass` to handle `show_border`.
    *   **Suggestion:** Simplify `renderText` to directly call `renderTextPass` with the calculated `final_border_thickness`. The `show_border` parameter in `renderText` can then directly control this calculation.
*   **`getTextBounds`:**
    *   **Issue:** This function calculates text bounds.
    *   **Suggestion:** This is a good utility function. No immediate refactoring needed.
*   **`_characters` Map vs. Array:**
    *   **Issue:** `std::map<char, Character> _characters;` is used to store font glyphs. For ASCII characters (0-127), a `std::array<Character, 128>` or `Character _characters[128]` might offer slightly better performance and simpler access if only ASCII is supported.
    *   **Suggestion:** If only ASCII characters are expected, consider switching to a fixed-size array for `_characters`. If Unicode support is planned, `std::map` or a more advanced font rendering library is appropriate.
