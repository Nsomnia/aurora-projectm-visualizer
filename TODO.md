## Development Roadmap: MilkDrop Feature Parity

The long-term goal is to replicate the rich feature set of MilkDrop. This will be achieved through a series of milestones, primarily focused on building a comprehensive settings panel within the GUI.

*   **Settings Panel (ImGui):** A comprehensive panel to manage all configuration.
    *   **General Settings:** Toggle shuffle, preset duration, text animations.
    *   **File Paths:** Configure directories for presets, music, and video output.
    *   **FFmpeg/Video:** Adjust video bitrate, framerate, codec, and container format.
    *   **Display Toggles:** Checkboxes to show/hide song title, artist, URL, and other on-screen elements.
    *   **Text Customization:** Input fields for custom text strings and colors.
*   **Batch Rendering Queue:**
    *   A UI element to add audio files to a list.
    *   A "Start Batch" button to process the queue sequentially, rendering a video for each file.
    *   Display progress for the current file and the overall queue.

This roadmap outlines planned features, enhancements, and refactoring efforts. It is a living document that will be updated as the project evolves.

https://github.com/milkdrop2077/MilkDrop3 has a great fully fleshed out set of features to use for inspriation.

### `[ ]` Milestone 1: Core Enhancements & Stability

This milestone focuses on implementing high-impact features inspired by classic visualizers and improving the overall stability and usability of the application from the command line.

*   **`[x]` General:**
    *   `[x]` Add a `--version` flag to print the application version.
    *   `[x]` Implement a `--verbose` or `--debug` flag for detailed logging.
    *   `[x]` Add randomness to the text bouncing logic so things dont get stuck or stale.
    *   `[x]` <BUG> Currently the song titlel, which is just the filename sanitized, splits it into multiple strings if its overly long but this has the side effect of causing it to turnj into multiple moving chunks of the orignal string instead of them staying "connected" to translate together.  </BUG>
*   **`[ ]` Preset Handling:**
    *   `[ ]` [Not Important] Would be useful to have a means of having the text, colors adjusted based on the colors under them whether as the color at on x y pixel, per letter, of the entire string, so that i.e. text with dark colors doesnt dissapear completely when movnig over dark visualizer regions.

<QUICK GEMINI INQUIRY WHILE RUNNING FOR NEXT EDIT> How do I adjust how many character count the title text has new lined placed? Can we set that font size different from the artist name as well since now its a bit large.  Can it automatically adjuist the number of characters to split into a new line at the next space found (once sanitized) based on the visualizer dimensions to be more dynamically safe? </INQUIRY>
<RESPONSE IF NOT JUST DONE IN THE CODE> </RESPONSE>

<REQUEST 2> Please make sure all command line options are and conitnue to be logiocal and are documented here or in a USAGE.md file. We may want a docs directory too. </REQUEST>

* MilkDrop (Windows only) keybinds/features to think about adding:
NOTE: A lot of these can be done in the qt/gtk gui only to make simpiler
CORE:
ESC exit help / menus
ALT+ENTER toggle fullscreen (or double-click on mouse)

PRESETS:
SPACE / H soft (space) / hard (H) cut to next preset
BACKSPACE go back to previous preset (with memory)
R toggle random / sequential preset order
A / SHIFT+A randomize colors / back to previous random color
Z / SHIFT+Z do random mini-mash-up / back to previous random preset
P / SHIFT+P force pattern / blending progress ( or : to reset)
CTRL+DELETE press to directly erase current preset
CTRL+INSERT save all the options + window size and position

HOTKEYS:
F1 help
F2 change max FPS
F3 change time between preset
F4 show preset name
F5 show preset rating
F6 always on top / no borders
F7 auto cut to next preset based on music
F8 double-preset mode
F9 toggle effects
F11 toggle sprites

SPRITES:
SHIFT+K 00-9 toggle sprite / message mode
DELETE clear latest sprite from the slot
CTRL+DELETE clear oldest sprite from the slot
RIGHT-CLICK move all the sprites
SCROLL-WHEEL resize sprite
G / SHIFT+J unblock saved sprite from milk file
J toggle sprite blendmodes
PLAYER:
CTRL+right next track
CTRL+left previous track


*   **`[x]` Preset Handling:**
    *   `[x]` Add `--preset-duration <sec>`: Set the time to play a single preset before switching (replaces `--transition-time` which is poorly named).
    *   `[ ]` Add `--preset-blend-time <sec>`: Set the blend time for preset transitions.
    *   `[ ]` Add `--hard-cut`: A boolean flag to force immediate preset transitions without blending.
    *   `[ ]` Add `--sequential-presets`: A boolean flag to cycle through presets in alphabetical order instead of random.
    *   `[ ]` Add a keybinding and `--lock-preset-key` to lock/unlock the current preset.
*   **`[ ]` Visuals & Performance:**
    *   `[ ]` Add `--mesh-size <value>`: Set the mesh size for the visualization (e.g., 32, 48, 64).
    *   `[ ]` Add `--show-fps`: A boolean flag to display the current frames per second.
    *   `[ ]` Add `--show-preset-name`: A boolean flag to display the current preset file name.
    *   `[ ]` Add `--show-preset-rating`: A boolean flag to display the preset rating (if available).
*   **`[ ]` Code Health & Refactoring:**
    ## Code Health & Refactoring

    *   `[x]` **Critical:** Replace the `Makefile` with a `CMakeLists.txt` file. This will provide better dependency management, cross-platform compatibility, and easier integration with IDEs.
    *   `[x]` Refactor `main.cpp`: Move argument parsing and configuration loading into the `Config` class to centralize all configuration logic.
    *   `[x]` Refactor `renderer.cpp`: Separate the core rendering loop from event handling logic. Event handling should be managed by the `EventHandler` class.
    *   `[x]` Review and standardize all header includes to use `<>` for system/library headers and `""` for internal project headers.

### `[ ]` Milestone 2: GUI and User Experience

This milestone introduces a graphical user interface to make the visualizer more accessible and interactive.

*   **`[ ]` Technology Spike:**
    *   `[ ]` Research and select a suitable C++ GUI library (e.g., Dear ImGui, Qt, wxWidgets) that integrates well with SDL2 and OpenGL.
*   **`[ ]` Phase 1: Basic Controls:**
    *   `[ ]` Implement a basic GUI overlay that can be toggled with a key.
    *   `[ ]` Add buttons for essential controls: Next/Previous Preset, Favorite, Lock Preset.
    *   `[ ]` Display current track info and preset name in the GUI.
    *   `[ ]` projectM .milk and .milk2 visualizer files enable/disable key bindings should have easily accessable buttons or a detachable window segment or widget for a list list of the projectm visualizer preset files that can be easily toggled on and off as well as switched to easily during playback, and a button or settings in the settings menu to enable shuffling.
    *   `[ ]` We will have to handle recording each video, if recording is used, for multiple files, as well as just simple playback so a audio playlist will be needed.
*   **`[ ]` Phase 2: Advanced Configuration:**
    *   `[ ]` Expand the GUI to allow real-time modification of all major `config.toml` parameters.
    *   `[ ]` Implement a file dialog to select audio files or preset directories.
    *   `[ ]` Add a live preview for text and color settings.
    *   `[ ]` [Not Imporant] Leave a space in the ui and lay out the skeleton or at least a plan here for using packages or projects liek whisper to automate doing speech-to-text for timed lyrics that can be disdplayed karaoke style. The user may research if theres a way to use a cli browser or other means to automatically dump the lyrics that are displayed publically  since we dont have API access.
    

### `[ ]` Milestone 3: Advanced Features & Extensibility

This milestone focuses on expanding the core capabilities of the visualizer.

*   **`[ ]` Enhanced Audio Input:**
    *   `[ ]` Improve cross-platform system audio capture (e.g., using PortAudio) to visualize any system audio.
    *   `[ ]` Add support for microphone input.
*   **`[ ]` Visual Enhancements:**
    *   `[ ]` Implement post-processing effects (e.g., bloom, blur).
    *   `[ ]` Allow custom image/video backgrounds.
*   **`[ ]` Plugin System:**
    *   `[ ]` Design a plugin architecture for new visualization modules beyond `projectM`.
