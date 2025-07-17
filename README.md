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

## Building and Git Workflow

This project uses CMake and requires a C++17 compiler. Dependencies are managed automatically.

To simplify the build process and manage Git operations, use the `build_and_git.sh` script.

### Usage of `build_and_git.sh`

```bash
./build_and_git.sh [command]
```

**Commands:**

*   `compile`: Configure and build the project.
*   `git`: Guide through basic Git workflow (status, add, commit, push).
*   `-h, --help, --usage`: Display detailed help message and Git explanation.

**Example:**

To compile the project:
```bash
./build_and_git.sh compile
```

To manage your Git changes:
```bash
./build_and_git.sh git
```

## Usage

For detailed usage instructions, please see [USAGE.md](USAGE.md).

## Versioning

The project version is defined in `CMakeLists.txt`. This version is used for display purposes and for naming backup archives. To update the project version, change the `project(AuroraVisualizer VERSION ...)` line in `CMakeLists.txt`.

## Backups

A full project backup can be created by running the `create_backup` target. This will create a timestamped `.tar.gz` archive in the `backups/` directory containing the source code, documentation, configuration, and the compiled executable.

```bash
cmake --build build --target create_backup
```

## Contributing

Contributions are highly encouraged! If you have ideas for new features, bug fixes, or performance improvements, please see the [CONTRIBUTING.md](CONTRIBUTING.md) file for details.

## License

This project is licensed under the MIT License.