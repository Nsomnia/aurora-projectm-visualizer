# Current Task Queue

This document consolidates all immediate and ongoing tasks for the Aurora Visualizer project.

## GUI Development Progress

### Phase 1: Foundation (Complete)
- [x] Created the main application window with a menu bar (`File`, `Help`).
- [x] Implemented a dockable panel system.
- [x] Created the first dock panel, "Controls".
- [x] Migrated the "Preset Duration" slider and "Shuffle Presets" checkbox into the "Controls" panel.
- [x] Added an "About" dialog.

### Phase 2: Playlist
- [x] Create a dockable "Playlist" panel.
- [x] Display the list of audio files from the config.
- [x] Allow adding/removing files from the playlist.
- [x] Allow reordering files in the playlist.
- [x] Implement playback controls (Play, Pause, Stop, Next, Previous).

### Phase 3: Visualizer Controls
- [x] Create a dockable "Visualizer" panel.
- [x] Display the current preset file.
- [ ] Implement a "Favorite" button to add the current preset to the favorites list.
- [ ] Display the list of favorite presets.
- [x] Add option to shuffle only favorite presets.

### Phase 4: Settings & Configuration
- [ ] Create a "Settings" dialog.
- [ ] Add controls for all major configuration options.
- [ ] Implement logic to save all settings to `~/.config/aurora-visualizer/config.toml`.
- [ ] Save the playlist and favorites to files in the same directory.

### Phase 5: Advanced Features
- [ ] Implement refined recording logic with temporary files and playback verification.
- [ ] Add keyboard shortcuts for all major actions.

## Immediate TODO

1.  **Fix flicker**: move ImGui after FBO blit.
2.  **Switch to CMake** + presets, add GitHub Actions.
3.  **Replace CLI parser with CLI11** (single commit).
4.  **Split `main.cpp` into `Config.cpp`, `PresetStore.cpp`, `AudioEngine.cpp`**.
5.  **Add `--version` & `--verbose` flags**.

## Development Roadmap: MilkDrop Feature Parity

### Milestone 1: Core Enhancements & Stability

*   **General:**
    *   [x] Add a `--version` flag to print the application version.
    *   [x] Implement a `--verbose` or `--debug` flag for detailed logging.
    *   [x] Add randomness to the text bouncing logic so things dont get stuck or stale.
    *   [x] <BUG> Currently the song titlel, which is just the filename sanitized, splits it into multiple strings if its overly long but this has the side effect of causing it to turnj into multiple moving chunks of the orignal string instead of them staying "connected" to translate together. </BUG>
*   **Preset Handling:**
    *   [ ] [Not Important] Would be useful to have a means of having the text, colors adjusted based on the colors under them whether as the color at on x y pixel, per letter, of the entire string, so that i.e. text with dark colors doesnt dissapear completely when movnig over dark visualizer regions.
    *   [x] Add `--preset-duration <sec>`: Set the time to play a single preset before switching (replaces `--transition-time` which is poorly named).
    *   [ ] Add `--preset-blend-time <sec>`: Set the blend time for preset transitions.
    *   [ ] Add `--hard-cut`: A boolean flag to force immediate preset transitions without blending.
    *   [ ] Add `--sequential-presets`: A boolean flag to cycle through presets in alphabetical order instead of random.
    *   [ ] Add a keybinding and `--lock-preset-key` to lock/unlock the current preset.
*   **Visuals & Performance:**
    *   [ ] Add `--mesh-size <value>`: Set the mesh size for the visualization (e.g., 32, 48, 64).
    *   [ ] Add `--show-fps`: A boolean flag to display the current frames per second.
    *   [ ] Add `--show-preset-name`: A boolean flag to display the current preset file name.
    *   [ ] Add `--show-preset-rating`: A boolean flag to display the preset rating (if available).
*   **Code Health & Refactoring:**
    *   [x] **Critical:** Replace the `Makefile` with a `CMakeLists.txt` file. This will provide better dependency management, cross-platform compatibility, and easier integration with IDEs.
    *   [x] Refactor `main.cpp`: Move argument parsing and configuration loading into the `Config` class to centralize all configuration logic.
    *   [x] Refactor `renderer.cpp`: Separate the core rendering loop from event handling logic. Event handling should be managed by the `EventHandler` class.
    *   [x] Review and standardize all header includes to use `<>` for system/library headers and `""` for internal project headers.

### Milestone 2: GUI and User Experience

*   **Technology Spike:**
    *   [ ] Research and select a suitable C++ GUI library (e.g., Dear ImGui, Qt, wxWidgets) that integrates well with SDL2 and OpenGL.
*   **Phase 1: Basic Controls:**
    *   [ ] Implement a basic GUI overlay that can be toggled with a key.
    *   [ ] Add buttons for essential controls: Next/Previous Preset, Favorite, Lock Preset.
    *   [ ] Display current track info and preset name in the GUI.
    *   [ ] projectM .milk and .milk2 visualizer files enable/disable key bindings should have easily accessable buttons or a detachable window segment or widget for a list list of the projectm visualizer preset files that can be easily toggled on and off as well as switched to easily during playback, and a button or settings in the settings menu to enable shuffling.
    *   [ ] We will have to handle recording each video, if recording is used, for multiple files, as well as just simple playback so a audio playlist will be needed.
*   **Phase 2: Advanced Configuration:**
    *   [ ] Expand the GUI to allow real-time modification of all major `config.toml` parameters.
    *   [ ] Implement a file dialog to select audio files or preset directories.
    *   [ ] Add a live preview for text and color settings.
    *   [ ] [Not Imporant] Leave a space in the ui and lay out the skeleton or at least a plan here for using packages or projects liek whisper to automate doing speech-to-text for timed lyrics that can be disdplayed karaoke style. The user may research if theres a way to use a cli browser or other means to automatically dump the lyrics that are displayed publically since we dont have API access.

### Milestone 3: Advanced Features & Extensibility

*   **Enhanced Audio Input:**
    *   [ ] Improve cross-platform system audio capture (e.g., using PortAudio) to visualize any system audio.
    *   [ ] Add support for microphone input.
*   **Visual Enhancements:**
    *   [ ] Implement post-processing effects (e.g., bloom, blur).
    *   [ ] Allow custom image/video backgrounds.
*   **Plugin System:**
    *   [ ] Design a plugin architecture for new visualization modules beyond `projectM`.
