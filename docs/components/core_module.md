# Core Component Improvements

This document outlines potential improvements for the `Core` component.

## `src/Core.cpp` / `include/visualizer/core.h`

*   **Long Constructor Initialization List:**
    *   **Issue:** The constructor has a very long initialization list.
    *   **Suggestion:** For simple members, consider using in-class member initialization in `core.h` (e.g., `SDL_Window* _window = nullptr;`). This can make the constructor more concise.
*   **Large `run` Function:**
    *   **Issue:** The `run` function contains the main application loop and is quite extensive.
    *   **Suggestion:**
        *   **Event Handling:** Extract the `while (SDL_PollEvent(&event))` loop into a private method, e.g., `Core::process_events()`.
        *   **Rendering Logic:** Further encapsulate the rendering steps. For example, a `Core::render_frame(double music_len, double current_time, const std::vector<std::string>& titleLines)` method could orchestrate the `_renderer.render`, `_text_renderer.renderText`, and `_gui->render` calls.
        *   **Video Recording:** Move the `glReadPixels` and frame flipping logic into `VideoExporter` (e.g., `_video_exporter.capture_and_write_frame(width, height)`). This centralizes video export concerns.
*   **GUI Integration (Commented Out):**
    *   **Issue:** The `_gui` unique pointer and related calls are commented out.
    *   **Suggestion:** Decide if the GUI is a temporary disabled feature or permanently removed. If permanent, remove all related code. If temporary, ensure clear comments and a plan for re-enabling/integrating it.
*   **`sanitize_filename` Duplication:**
    *   **Issue:** `_config.songTitle = sanitize_filename(current_audio_file);` calls a `sanitize_filename` function that is also present in `utils/common.cpp`.
    *   **Suggestion:** Ensure `Core` uses the canonical `sanitize_filename` from `visualizer/utils/common.h`.
*   **Video Recording Cutoff:**
    *   **Issue:** Recorded videos cut off a short period before the end of audio playback.
    *   **Cause:** Likely a synchronization issue between SDL_mixer's audio playback status and FFmpeg's video encoding, or insufficient buffering for FFmpeg.
    *   **Simple Fixes (to try first):**
        1.  **Increase `extra_frames_after_music_ends`:** In `Core::run()`, increase the value assigned to `extra_frames_after_music_ends` (currently `_config.fps`). Try `2 * _config.fps` or `3 * _config.fps`.
        2.  **Add `-fflags +genpts` to FFmpeg command:** Modify `Config::ffmpeg_command` to include this flag.
        3.  **Increase `thread_queue_size` for raw video input:** Add `-thread_queue_size <size>` (e.g., `512` or `1024`) right before the `-i -` input in the FFmpeg command. This provides more buffering for the video stream.
