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
5.  **Git**:
    *   A reminder to commit is shown before and after the build.
    *   New source files must be manually added (`git add <file>`).
    *   Use `git status` to check for untracked files.

