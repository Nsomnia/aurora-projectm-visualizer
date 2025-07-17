# General Observations & Improvements

This document outlines general observations and potential improvements applicable across the Aurora Visualizer project.

## Consistency in Header Guards:
*   **Issue:** Some headers use `#pragma once` (`AnimationManager.h`, `audio_input.h`, `CliParser.h`, `ConfigLoader.h`, `core.h`, `event_handler.h`, `Gui.h`, `ImGuiIntegration.h`, `preset_manager.h`, `renderer.h`, `TextManager.h`, `TextRenderer.h`, `VideoExporter.h`) while others use `#ifndef/#define/#endif` (`Config.h`, `audio_backend.h`, `display_backend.h`, `common.h`).
*   **Suggestion:** Standardize to `#pragma once` for all headers in a modern C++ project. It's simpler and less error-prone.

## Redundant `visualizer/` Prefix in Includes:
*   **Issue:** All internal includes use the `visualizer/` prefix (e.g., `#include "visualizer/Config.h"`). This is redundant if `include/` is the primary include directory for the project.
*   **Suggestion:** Remove the `visualizer/` subdirectory from `include/` and adjust all includes to directly reference the header file (e.g., `#include "Config.h"`). This simplifies include paths and reduces verbosity.

## Error Handling and Logging:
*   **Issue:** `std::cerr` is used inconsistently for warnings and errors across various files.
*   **Suggestion:** Implement a simple, custom logging system (e.g., a `Logger` class or utility functions) that can be configured for different verbosity levels (e.g., `DEBUG`, `INFO`, `WARN`, `ERROR`) based on `_config.verbose_logging`. This provides a centralized and more flexible logging mechanism.

## Magic Numbers/Strings:
*   **Issue:** Many literal values (e.g., `44100`, `2`, `4096` in `AudioInput::init`, `128` in `TextRenderer`) and strings (e.g., `".milk"`, `"favorites.txt"`) are hardcoded.
*   **Suggestion:** Define these as `const` variables, `enum` values, or configuration parameters where appropriate. This improves readability, maintainability, and makes it easier to modify these values.

## Global Variables:
*   **Issue:** `volatile sig_atomic_t g_quit_flag` is a global variable used for signal handling in `main.cpp` and `core.cpp`.
*   **Suggestion:** While common for signal handlers, consider encapsulating this within the `Core` class or a dedicated `SignalHandler` class (perhaps a singleton) to reduce global state and improve encapsulation.

## `std::string` vs `char[]` for `ffmpeg_command`:
*   **Issue:** `Config::ffmpeg_command` is a fixed-size `char[]`, leading to manual `strncpy` and potential truncation warnings in `ConfigLoader.cpp` and `VideoExporter.cpp`.
*   **Suggestion:** Change `ffmpeg_command` in `Config.h` to `std::string`. This is more idiomatic C++, safer, and handles dynamic string lengths automatically.
