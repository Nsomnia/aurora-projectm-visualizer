## Incomplete Task Queue (from Gemini 2.5 Pro)

**Current Status (End of Session):**

*   **`CMakeLists.txt`**: Updated to remove SDL dependencies and include Qt components. A `create_source_backup` target was added.
*   **`include/Config.h`**: Updated to remove `SDL.h` and replace `SDL_Keycode` with `QKeysequence`.
*   **`include/core.h`**: Updated to remove `SDL.h`, forward-declare `QOpenGLWidget`, and modify `init()` and `render()` signatures for Qt integration.
*   **`src/main.cpp`**: Updated to use `QApplication` and `QMainWindow` for the main application loop and window management.
*   **`include/QtGui.h`**: Updated to inherit from `QMainWindow` and adjust constructor/member variables.
*   **`src/QtGui.cpp`**: Updated to reflect changes in `QtGui.h` and integrate with Qt widgets.
*   **Incorrect files (`Gui.h`, `Gui.cpp`, `ImGuiIntegration.h`, `ImGuiIntegration.cpp`)**: Moved to `backups/pre-qt-refactor_jul15-afternoon/`.

**Remaining Tasks for Next Session:**

1.  **`include/QtOpenGLWidget.h` and `src/QtOpenGLWidget.cpp`**:
    *   Modify `QtOpenGLWidget.h` to inherit from `QOpenGLWidget`.
    *   Update `QtOpenGLWidget.cpp` to implement `initializeGL()`, `resizeGL()`, and `paintGL()` methods, integrating `_core.init()` and `_core.render()` calls within the Qt OpenGL context.
    *   Remove any remaining SDL-specific code from these files.
2.  **`src/core.cpp`**:
    *   Update `Core::init()` to remove SDL window/context creation and instead use the `QOpenGLWidget` passed to it.
    *   Update `Core::run()` (or the new `Core::render()` method) to remove the SDL event loop and direct rendering, relying on Qt's `paintGL` for rendering.
    *   Remove any remaining SDL-specific code (e.g., `SDL_Delay`, `SDL_PollEvent`, `SDL_GL_SwapWindow`).
3.  **`include/audio_input.h` and `src/audio_input.cpp`**:
    *   Remove `SDL_mixer` dependencies. The audio input will need to be re-evaluated for a Qt-native or cross-platform audio solution that doesn't rely on SDL.
4.  **`src/CliParser.cpp`**:
    *   Remove `SDL_GetKeyFromName` calls and replace with Qt-equivalent key parsing if necessary, or remove key parsing if it's now handled by Qt's event system.
5.  **`include/event_handler.h` and `src/event_handler.cpp`**:
    *   These files are likely obsolete as Qt's event system will handle user input. Confirm and remove them.
6.  **Rebuild and Test**:
    *   Run `cmake -B build` to reconfigure the project.
    *   Run `cmake --build build` to compile.
    *   Run `./build/AuroraVisualizer` to test the Qt application.
    *   Address any remaining compilation or linker errors.
    *   Verify the `BadWindow` error is resolved.
