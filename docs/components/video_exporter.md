# VideoExporter Component Improvements

This document outlines potential improvements for the `VideoExporter` component.

## `src/VideoExporter.cpp` / `include/visualizer/VideoExporter.h`

*   **Duplicated Utility Functions:**
    *   **Issue:** `sanitize_filename` is duplicated here and in `utils/common.cpp`. `replace_placeholders` is a generic string utility.
    *   **Suggestion:** Remove the local `sanitize_filename` and use the one from `visualizer/utils/common.h`. Move `replace_placeholders` to `visualizer/utils/common.h` and `src/utils/common.cpp`.
*   **Frame Flipping Logic:**
    *   **Issue:** The frame flipping logic (`glReadPixels` and `memcpy` for flipping) is currently in `Core::run`.
    *   **Suggestion:** Move the `glReadPixels` and frame flipping logic into a new public method in `VideoExporter` (e.g., `VideoExporter::capture_and_write_frame(int width, int height)`). This centralizes all video export-related operations within the `VideoExporter` class.
