# Gui Component Improvements

This document outlines potential improvements for the `Gui` component.

## `src/Gui.cpp` / `include/visualizer/Gui.h`

*   **Circular Dependency with `Core`:**
    *   **Issue:** `Gui` takes a `Core&` reference, and `Core` holds a `std::unique_ptr<Gui>`. While `Core` forward-declares `Gui`, this can sometimes indicate tight coupling.
    *   **Suggestion:** If `Gui` only needs access to the `Renderer` (via `_core.get_renderer()`), consider passing `Renderer&` directly to `Gui`'s constructor instead of `Core&`. This reduces the dependency scope.
*   **Raw `ImTextureID` Cast:**
    *   **Issue:** The cast `(ImTextureID)(intptr_t)_core.get_renderer().get_fbo_texture()` is a raw C-style cast.
    *   **Suggestion:** While common with ImGui, consider if a more type-safe wrapper or a `static_cast` with `reinterpret_cast` (if absolutely necessary) could be used, or if ImGui provides a more idiomatic way to handle texture IDs.
