# Aurora Visualizer – KIMI Agent Deep-Dive Refactor / Optimisation Report  
*(command-line core vs. embryonic Qt/GTK GUI)*  

---

## 1. Repository-wide Quick Wins

| Area | Action | Impact |
|---|---|---|
| **Tooling** | Replace the handwritten Makefile with **CMake ≥ 3.20** + presets (`RelWithDebInfo`, `Debug`) | Cross-platform, IDE friendliness, sanitizer flags, easy `vcpkg`/`FetchContent` |
| **Deps** | Vendor **SDL2**, **projectM**, **FFmpeg**, **ImGui** as `FetchContent` or `submodule` | Reproducible builds, offline CI |
| **CI** | Add GitHub Actions matrix: Ubuntu, macOS, Windows, Clang/MSVC | Prevent regressions |
| **Sanitizers** | Enable `-fsanitize=address,undefined` in Debug builds | Catch UB early |
| **LTO/IPO** | Enable `INTERPROCEDURAL_OPTIMIZATION` in Release | ~10-15 % speed bump |
| **clang-format** | Adopt strict `.clang-format`, enforce in CI | Uniform style, smaller diffs |
| **pre-commit** | clang-format, cmake-format, trailing-whitespace | No style wars |

---

## 2. Command-Line Core (`main.cpp`, `Config`, `Renderer`, `ProjectMWrapper`)

### 2.1 Configuration & CLI

| Task | Why |
|---|---|
| **Single source of truth** | Merge CLI args + TOML into a strongly-typed `struct Config`. Use `CLI11` instead of hand-rolled parser → 50 % less code, automatic `--help`, validation. |
| **Version/Verbosity flags** | Add `--version` (CMake writes `git describe`) and `--verbose`/`--debug` that toggle `spdlog` level at runtime. |
| **Config hot-reload** | Watch `config.toml` with `inotify`/`ReadDirectoryChangesW` and re-apply without restart (opt-in). |
| **Validation layer** | Fail fast: check FFmpeg exists, preset dir readable, OpenGL ≥ 3.3, etc. |

### 2.2 Rendering Loop (`Renderer.cpp` / `core.cpp`)

| Task | Why |
|---|---|
| **Flicker fix (current sprint)** | 1. Render projectM to off-screen FBO. 2. Blit FBO texture to back-buffer. 3. **Then** ImGui overlay. 4. SwapBuffers once. |
| **V-Sync control** | Expose `--vsync on|off|adaptive` via `SDL_GL_SetSwapInterval`. |
| **FPS counter** | Add `ImGui::Text("FPS: %.1f", io.Framerate);` behind `--show-fps`. |
| **GPU timer queries** | Optional `--gpu-profile` to measure frame GPU time (helps tune mesh-size). |
| **HDR / sRGB** | Create an sRGB default framebuffer, request 10-bit if available, improves banding. |
| **Swapchain resize** | Handle `SDL_WINDOWEVENT_RESIZED` properly: recreate FBO & ImGui viewport. |

### 2.3 Audio & FFmpeg Integration

| Task | Why |
|---|---|
| **Audio abstraction** | Introduce `IAudioSource` interface: `FileAudioSource`, `SystemLoopbackSource` (PortAudio), `MicSource`. Removes `#ifdef` spaghetti. |
| **Resampler** | Use `libswresample` so any input sample-rate feeds projectM at 48 kHz. |
| **FFmpeg reusable class** | `Recorder` RAII wrapper that owns the pipe, writes raw RGBA frames + audio; supports graceful failure (log + continue instead of `exit()`). |
| **Zero-copy audio** | Send planar float directly to FFmpeg via `-f f32le -ar 48000 -ac 2` to avoid temp files. |
| **Lossless preset** | Provide `--preset-lossless` flag that writes FFV1 + FLAC in MKV for archival. |

### 2.4 Preset Management

| Task | Why |
|---|---|
| **PresetStore** | Singleton that owns the vector of presets, history, favorites, broken list. Exposes `next()`, `prev()`, `random()`, `lock()`. |
| **File watching** | Detect new `.milk` files at runtime (nice with community packs). |
| **Preset scoring** | Store small `.json` sidecar per preset with rating & usage statistics → better shuffle weighting. |
| **Threaded preset load** | Decompress `.milk` on worker thread to avoid frame drops. |

### 2.5 Text & Overlay

| Task | Why |
|---|---|
| **Dynamic line-wrap** | Use `ImFont::CalcTextSizeA` to split title at last space before `max_width = window.x * 0.9`. Cache result until string or size changes. |
| **Independent font sizes** | `song_info_font_size` vs `artist_font_size` in config. |
| **Contrast helper** | Sample 4 corner pixels of text bounding box, compute luminance, auto-switch to white outline if dark background. |

---

## 3. Code Health (cross-cutting)

| Task | Why |
|---|---|
| **Header hygiene** | `<> ` for system, `""` for project. IWYU in CI. |
| **Namespaces** | `namespace av` (Aurora Visualizer) to avoid collision. |
| **Split large TUs** | Keep every `.cpp` < 600 lines. Extract `PresetManager`, `AudioEngine`, `Recorder`, `EventHandler`. |
| **Modern C++** | Replace raw `new`/`delete` with smart pointers. Use `std::filesystem` instead of `dirent.h`. |
| **Error handling** | `tl::expected<T,E>` or `std::variant<T,Error>` rather than `bool` + `std::cerr`. |
| **Logging** | Replace `printf` with `spdlog` with rotating file sink. |
| **Unit tests** | Catch2 for `Config`, `PresetStore`, `AudioSource`. Run in CI. |
| **Benchmarks** | `google-benchmark` for rendering hot paths (preset switch time, FFT cost). |

---

## 4. GUI Front-End (Qt or GTK – embryonic)

### 4.1 Architecture Blueprint

```
┌-------------------------┐
│   GUI Process (Qt/QML)  │
│  ┌---------------------┐ │
│  │   Presenter/Store   │ │ <-- stateless, serializable
│  └---------┬-----------┘ │
│            │ ZeroMQ/GRPC│
│  ┌---------▼-----------┐ │
│  │   Renderer Process  │ │ <-- headless, owns OpenGL
│  └---------------------┘ │
└-------------------------┘
```

* **Rationale**: Keeps heavy OpenGL + FFmpeg + projectM in a sandbox that can crash/restart without losing UI state. Enables future headless cloud rendering.

### 4.2 Qt Quick (QML) Starter Plan

| Component | Tech | Notes |
|---|---|---|
| **Main window** | `QQuickWindow` + `QSGRenderer` | Overlay controls on top of native GL context |
| **Settings panel** | `SettingsModel` (C++) bound to `SettingsForm.ui.qml` | Auto-generated from `Config` struct via `Q_GADGET` |
| **Preset list** | `QAbstractListModel` + `GridView` | Live search, star ratings |
| **Audio playlist** | `QMediaPlaylist` (Qt6) or custom model | Drag-drop reorder, metadata via `taglib` |
| **Batch queue** | `QList<QFuture>` + `QtConcurrent` | Progress bar, cancel button |
| **Thumbnail preview** | Render first 5 s of each preset to tiny WebP in worker thread | 128×128 thumbnails load in < 50 ms |
| **Hotkey map editor** | `QKeySequenceEdit` | Store platform-neutral `QKeySequence::PortableText` |
| **Theme switcher** | `QStyleHints` + `Material` style | Light/Dark/auto |
| **IPC** | `QLocalSocket`/`QProcess` | Send JSON commands: `{"cmd":"nextPreset"}` |

### 4.3 GTK (gtkmm 4) Alternative

| Component | Tech |
|---|---|
| **Window** | `Gtk::ApplicationWindow` + `Gtk::GLArea` |
| **Preferences** | `Gtk::PreferencesWindow` with `Adw` (libadwaita) |
| **Playlist** | `Gtk::ColumnView` + `Gtk::SingleSelection` |
| **Batch view** | `Gtk::ListBox` of `Gtk::ProgressBar` rows |
| **IPC** | `Gio::Subprocess` + `Gio::UnixConnection` (D-Bus for flatpak) |

---

## 5. Future-Proofing & Extensibility

| Idea | Benefit |
|---|---|
| **Plugin SDK** | Dynamic `.so`/`.dll` loaded via `dlopen` exposing `IVizModule` interface → allow GLSL shader visualizers, NotchLC, etc. |
| **MaterialX / GLSL node editor** | Let power-users create presets visually, export to `.milk` or SPIR-V. |
| **WebAssembly build** | Compile with `emscripten`, run in browser for sharing on social media. |
| **Headless server** | Docker image that listens to HTTP POST `/render` with audio file + params, returns MP4. |
| **Whisper lyrics** | Offline `whisper.cpp` for speech-to-text → LRC file → karaoke overlay. |
| **GPU FFT** | Use `VkFFT` or `OpenCL` to offload FFT from CPU → lower latency, higher FPS. |

---

## 6. Immediate TODO (next 48 h)

1. **Fix flicker**: move ImGui after FBO blit.  
2. **Switch to CMake** + presets, add GitHub Actions.  
3. **Replace CLI parser with CLI11** (single commit).  
4. **Split `main.cpp` into `Config.cpp`, `PresetStore.cpp`, `AudioEngine.cpp`**.  
5. **Add `--version` & `--verbose` flags**.  

---

> *“Make it work, make it right, make it fast… then make it gorgeous.”*