## Project Overview

This is a C++ project that uses CMake for building, Qt6 for the GUI, and projectM for music visualization.

## Key Technologies

*   **Build System**: CMake
*   **GUI**: Qt6
*   **Visualization**: projectM
*   **Dependencies**: Managed via CPM.cmake. Dependencies are cached in `deps/cpm_cache`.

## Workflow

1.  **Versioning**: The project version is set in `CMakeLists.txt`.
2.  **Building**: Use `cmake -B build && cmake --build build`.
3.  **Running**: The executable is `build/bin/AuroraVisualizer`.
4.  **Backups**: A full project backup can be created with `cmake --build build --target create_backup`. Do not use `rm`; move files to the `backups` directory instead. The project is under git source control.
5.  **Configuration**: Ensure `config/default.toml` settings are correctly nested under their respective TOML sections (e.g., `[display]`, `[text]`) for proper parsing.
6.  **Git**:
    *   A reminder to commit is shown before and after the build.
    *   New source files must be manually added (`git add <file>`).
    *   Use `git status` to check for untracked files.

## Development Best Practices

1.  **User Testing & Verification:** As this is a GUI application, thorough testing of visual and interactive elements must be performed by the user. After any code changes or feature implementations, please test the application and report back on functionality or required fixes.
2.  **Commit Granularity:** Commit changes frequently after each logical step, bug fix, or implemented functionality. This ensures a clear Git history.
3.  **Model Protocol:** The model will prompt the user for GUI testing and feedback, and will remind about committing after changes. The model's direct testing capabilities are limited to command-line output and build processes.

