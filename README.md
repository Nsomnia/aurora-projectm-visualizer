# Aurora Visualizer

**Aurora Visualizer** is a cross-platform music visualization application built with C++, powered by the renowned [projectM](https://github.com/projectM-visualizer/projectm) visualization library. It aims to provide a highly customizable and visually stunning experience for music lovers and developers alike.

This project is under active development, with a focus on robust performance, a rich feature set, and a clean, maintainable codebase.

## Features

*   **Real-time Music Visualization:** Utilizes `libprojectM` for a wide variety of audio-reactive visuals.
*   **Highly Configurable:** Customize visuals, text overlays, and application settings via a simple TOML configuration file and command-line arguments.
*   **Dynamic Text Overlays:** Display the song title, artist, and a custom URL with configurable animations, including fading, bouncing, and transparency effects.
*   **Video Recording (FFmpeg Integration):** Record your visualizations to a video file, including the audio. Full control over FFmpeg command-line arguments allows for highly customized video and audio encoding presets.
*   **Advanced Preset Management:**
    *   **Comprehensive Preset Loading:** Automatically loads all `.milk` presets from a specified directory.
    *   **Random or Sequential Cycling:** Navigate through presets randomly or in order.
    *   **Preset History:** Easily go back to previously viewed presets.
    *   **Favorites System:** Mark presets as "favorites" for quick access or to create curated lists.
    *   **Broken Preset Management:** Mark problematic presets to move them to a separate directory, preventing them from being loaded again.
    *   **Configurable Keybindings:** Customize keyboard shortcuts for all preset management actions.

## Building from Source

This project uses CMake and requires a C++17 compiler. Dependencies are managed automatically.

1.  **Prerequisites:**
    *   A C++ compiler (like GCC or Clang)
    *   CMake (version 3.20 or higher)
    *   Git
    *   Development libraries for Qt6, OpenGL, GLEW, GLM, Freetype, and FFmpeg.
        *   On Debian/Ubuntu: `sudo apt-get install build-essential cmake git qt6-base-dev libgl1-mesa-dev libglew-dev libglm-dev libfreetype6-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev`

2.  **Configure and Build:**
    ```bash
    cmake -B build
    cmake --build build
    ```
    The executable will be located at `build/bin/AuroraVisualizer`.

3.  **Run:**
    ```bash
    ./build/bin/AuroraVisualizer
    ```

## Usage

Run the visualizer from the command line, optionally providing one or more audio files. If no audio files are provided, the visualizer will run in a "listen" mode, reacting to system audio input (if configured).

```bash
./bin/visualizer [options] <audio_file_1> <audio_file_2> ...
```

### Command-Line Options

Command-line arguments take precedence over settings in `config.toml`.

*   **Display & Performance:**
    *   `--width <px>`: Set window width (default: `640`).
    *   `--height <px>`: Set window height (default: `420`).
    *   `--fps <value>`: Set frames per second (default: `24`).
*   **Text & Font:**
    *   `--font-path <path>`: Path to the font file (TTF/OTF) for text overlays (default: `/usr/share/fonts/TTF/DejaVuSans-Bold.ttf`).
    *   `--song-info-font-size <pt>`: Font size for song title and artist (default: `42`).
    *   `--url-font-size <pt>       `: Font size for the URL text (default: `28`).
    *   `--song-info-color <hex>    `: Font color for song info in hexadecimal (e.g., `AAFF00`).
    *   `--url-color <hex>          `: Font color for URL in hexadecimal (e.g., `AA0099`).
    *   `--song-info-border-color <hex> `: Color for the song info text border in hexadecimal (e.g., `AA00FF`).
    *   `--url-border-color <hex>   `: Color for the URL text border in hexadecimal (e.g., `000000`).
    *   `--song-info-border-thickness <val> `: Thickness of the song info text border (float, default: `0.2`).
    *   `--url-border-thickness <val>   `: Thickness of the URL text border (float, default: `0.2`).
*   **Text Content & Visibility:**
    *   `--title <text>`: Custom text to display as the song title.
    *   `--artist-name <name>`: Set custom artist name overlay.
    *   `--url-text <text>`: Set custom URL text overlay.
    *   `--hide-title`: Hide the song title text overlay.
    *   `--hide-artist`: Hide the artist name text overlay.
    *   `--hide-url`: Hide the URL text overlay.
*   **Animation:**
    *   `--disable-text-animation`: Disable all text animation effects.
    *   `--transition-time <sec>`: Duration of the fade effect at the end of text display (float, default: `10.0`).
    *   `--pre-fade-delay <sec>`: Time to wait at full opacity before text starts fading out (float, default: `10.0`).
    *   `--bounce-duration <sec>`: How long the text bounces before fading (float, default: `7.0`).
    *   `--bounce-speed <pixels/sec>`: Speed of the bouncing text (float, default: `67.0`).
    *   `--fade-to-min-duration <sec>`: Time it takes for text to fade to its minimum transparency (float, default: `10.0`).
    *   `--min-transparency <0-1>`: The minimum transparency for the text (float, default: `0.15`).
*   **Presets:**
    *   `--preset-list-file <path>`: Path to a file containing a list of `.milk` presets to load.
    *   `--broken-preset-directory <path>`: Directory to move broken presets to.
    *   `--favorites-file <path>`: Path to the favorites file.
    *   `--shuffle-enabled`: Enable or disable random preset shuffling (default: `true`).
    *   `--next-preset-key <key>`: Key to load the next random preset (e.g., `n`).
    *   `--prev-preset-key <key>`: Key to load the previous preset (e.g., `p`).
    *   `--mark-broken-preset-key <key>`: Key to mark the current preset as broken (e.g., `b`).
    *   `--favorite-preset-key <key>`: Key to mark the current preset as a favorite (e.g., `f`).
*   **Recording:**
    *   `--record-video`: Enable video recording.
    *   `--audio-input-mode <mode>`: Set audio input mode for recording. Options: `SystemDefault` (default system audio), `PipeWire` (creates a virtual sink for combined playback/recording, recommended), `PulseAudio` (attempts PulseAudio routing, similar to PipeWire via bridge), `File` (audio from provided `--audio-file`). Default: `PipeWire`.
    *   `--pipewire-sink-name <name>`: Set the name of the virtual PipeWire sink to create (default: `AuroraSink`).
    *   `--output-directory <path>`: Directory to save recorded videos (default: `videos`).
    *   `--video-framerate <value>`: Set video recording framerate (default: `24`).
    *   `--ffmpeg-command <cmd>`: The FFmpeg command template for recording. This is a powerful option allowing full customization of video and audio encoding.
        *   **Placeholders:** Use `{WIDTH}`, `{HEIGHT}`, `{FPS}`, `{FRAMERATE}`, and `{OUTPUT_PATH}`. These will be replaced by the application at runtime. Audio input is handled by the selected `--audio-input-mode`.
        *   **Important:** Ensure paths with spaces are enclosed in double quotes within the command string (e.g., `"{OUTPUT_PATH}"`).
        *   **Example:** `--ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 28 -preset medium -c:a aac -b:a 192k {OUTPUT_PATH}"`
*   **Other:**
    *   `--audio-file <path>`: Add an audio file to the playlist. Can be used multiple times to create a queue.
    *   `-h, --help`: Display the help message.

### Keybindings (Default)

*   **Next Preset:** `n`
*   **Previous Preset:** `p`
*   **Mark as Broken:** `b`
*   **Toggle Favorite:** `f`
*   **Quit:** `q`

### Configuration File (`config/default.toml`)

The primary configuration is managed through `config/default.toml`. This file provides default values for all configurable parameters. When the application starts, it first attempts to load `~/.config/aurora-visualizer/config.toml`. If that file doesn't exist, it falls back to `config/default.toml` located relative to the executable. Command-line arguments always override settings from any loaded TOML file.

**Example `config/default.toml` snippet for FFmpeg:**

```toml
```toml
# --- Recording ---
record_video = false
audio_input_mode = "PipeWire" # Options: "SystemDefault", "PipeWire", "PulseAudio", "File"
pipewire_sink_name = "AuroraSink" # Name of the virtual PipeWire sink to create
output_directory = "videos"
video_framerate = 24
# Note: {WIDTH}, {HEIGHT}, {FPS}, and {OUTPUT_PATH} are placeholders.
ffmpeg_command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 28 -preset medium -c:a aac -b:a 192k {OUTPUT_PATH}"
```
```

## Versioning

The project version is defined in `CMakeLists.txt`. This version is used for display purposes and for naming backup archives. To update the project version, change the `project(AuroraVisualizer VERSION ...)` line in `CMakeLists.txt`.

## Backups

A full project backup can be created by running the `create_backup` target. This will create a timestamped `.tar.gz` archive in the `backups/` directory containing the source code, documentation, configuration, and the compiled executable.

```bash
cmake --build build --target create_backup
```

## Contributing

Contributions are highly encouraged! If you have ideas for new features, bug fixes, or performance improvements, please:

1.  **Open an Issue:** Discuss your proposed changes or report bugs by opening an issue on the GitHub repository. This helps ensure alignment with the project's vision and avoids duplicate work.
2.  **Fork the Repository:** Create your own fork of the project.
3.  **Create a Feature Branch:** Work on your changes in a dedicated branch (e.g., `feature/my-new-feature` or `bugfix/fix-xyz`).
4.  **Write Clean Code:** Adhere to the existing coding style and conventions. Ensure your code is well-commented where necessary.
5.  **Test Your Changes:** If applicable, write unit or integration tests for your new features or bug fixes. Ensure all existing tests pass.
6.  **Submit a Pull Request:** Once your changes are complete and tested, submit a pull request to the `main` branch of the original repository. Provide a clear description of your changes and reference any related issues.

## License

This project is licensed under the MIT License.