# CMakeLists.txt Improvements

This document outlines potential improvements for the `CMakeLists.txt` file.

*   **`PipewireAudioRouter.cpp` Reference:**
    *   **Issue:** `src/PipewireAudioRouter.cpp` is listed in `APP_SOURCE_FILES` but is not present in the provided file list.
    *   **Suggestion:** Verify if this file exists or if it's a leftover from previous development. If it doesn't exist and isn't planned, remove its reference from `CMakeLists.txt`.
*   **ProjectM Library Linking:**
    *   **Issue:** Linking directly to `${CMAKE_BINARY_DIR}/projectm_install/lib/libprojectM-4.so` is functional but less robust for general installation.
    *   **Suggestion:** For more standard CMake practices, consider using `find_library(PROJECTM_LIBRARY projectM-4 PATHS ${CMAKE_BINARY_DIR}/projectm_install/lib NO_DEFAULT_PATH)` and then `target_link_libraries(AuroraVisualizer PRIVATE ${PROJECTM_LIBRARY})`. This makes the build more flexible.
