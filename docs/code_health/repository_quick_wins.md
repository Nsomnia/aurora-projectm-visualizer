# Repository-wide Quick Wins

| Area | Action | Impact |
|---|---|---|
| **Tooling** | Replace the handwritten Makefile with **CMake ≥ 3.20** + presets (`RelWithDebInfo`, `Debug`) | Cross-platform, IDE friendliness, sanitizer flags, easy `vcpkg`/`FetchContent` |
| **Deps** | Vendor **SDL2**, **projectM**, **FFmpeg**, **ImGui** as `FetchContent` or `submodule` | Reproducible builds, offline CI |
| **CI** | Add GitHub Actions matrix: Ubuntu, macOS, Windows, Clang/MSVC | Prevent regressions |
| **Sanitizers** | Enable `-fsanitize=address,undefined` in Debug builds | Catch UB early |
| **LTO/IPO** | Enable `INTERPROCEDURAL_OPTIMIZATION` in Release | ~10-15 % speed bump |
| **clang-format** | Adopt strict `.clang-format`, enforce in CI | Uniform style, smaller diffs |
| **pre-commit** | clang-format, cmake-format, trailing-whitespace | No style wars |
