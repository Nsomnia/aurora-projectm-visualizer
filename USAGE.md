# Usage

Run the visualizer from the command line, optionally providing one or more audio files. If no audio files are provided, the visualizer will run in a "listen" mode, reacting to system audio input (if configured).

```bash
./build/bin/AuroraVisualizer [options] <audio_file_1> <audio_file_2> ...
```

## Command-Line Options

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

## Keybindings (Default)

*   **Next Preset:** `n`
*   **Previous Preset:** `p`
*   **Mark as Broken:** `b`
*   **Toggle Favorite:** `f`
*   **Quit:** `q`

## Configuration File (`config/default.toml`)

The primary configuration is managed through `config/default.toml`. This file provides default values for all configurable parameters. When the application starts, it first attempts to load `~/.config/aurora-visualizer/config.toml`. If that file doesn't exist, it falls back to `config/default.toml` located relative to the executable. Command-line arguments always override settings from any loaded TOML file.

**Important Note on Configuration Structure:**

For settings to be correctly parsed from `config/default.toml`, they must be organized under specific TOML sections (tables) as expected by the application. For example, `width`, `height`, and `fps` must be under a `[display]` section, `font_path` and `song_info_font_size` under `[text]`, and so on. Refer to `config/default.toml` for the correct sectioning.

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
