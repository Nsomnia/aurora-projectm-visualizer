# ConfigLoader Component Improvements

This document outlines potential improvements for the `ConfigLoader` component.

## `src/ConfigLoader.cpp` / `include/visualizer/ConfigLoader.h`

*   **Duplicated Utility Functions:**
    *   **Issue:** `trim` and `remove_quotes` are static helper functions within `ConfigLoader.cpp`. The path resolution logic for `~` (home directory) is also duplicated here and in `PresetManager.cpp`.
    *   **Suggestion:** Move `trim`, `remove_quotes`, and a generic `resolve_home_path(const std::string& path)` function to `visualizer/utils/common.h` and `src/utils/common.cpp`. This centralizes common utilities.
*   **Verbose `load_from_file` Function:**
    *   **Issue:** Similar to `CliParser::parse`, the `load_from_file` function has a large `unordered_map` for parsing configuration values.
    *   **Suggestion:** Abstract the configuration parsing logic into a more generic, reusable component or pattern. This could involve a `ConfigParser` class that takes a map of key-value pairs and applies them to a `Config` object, reducing duplication between `CliParser` and `ConfigLoader`.
*   **FFmpeg Command Truncation Warning:**
    *   **Issue:** The `ffmpeg_command` truncation warning is specific to `char[]`.
    *   **Suggestion:** This issue will be resolved by changing `ffmpeg_command` to `std::string` in `Config.h`.
