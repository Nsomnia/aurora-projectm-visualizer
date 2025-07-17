# Command-Line Core Overview

This document outlines the structure and key areas of the command-line core of the Aurora Visualizer, primarily focusing on `main.cpp`, `Config`, `Renderer`, and `ProjectMWrapper`.

## Configuration & CLI

| Task | Why |
|---|---|
| **Single source of truth** | Merge CLI args + TOML into a strongly-typed `struct Config`. Use `CLI11` instead of hand-rolled parser → 50 % less code, automatic `--help`, validation. |
| **Version/Verbosity flags** | Add `--version` (CMake writes `git describe`) and `--verbose`/`--debug` that toggle `spdlog` level at runtime. |
| **Config hot-reload** | Watch `config.toml` with `inotify`/`ReadDirectoryChangesW` and re-apply without restart (opt-in). |
| **Validation layer** | Fail fast: check FFmpeg exists, preset dir readable, OpenGL ≥ 3.3, etc. |

## Rendering Loop

| Task | Why |
|---|---|
| **Flicker fix (current sprint)** | 1. Render projectM to off-screen FBO. 2. Blit FBO texture to back-buffer. 3. **Then** ImGui overlay. 4. SwapBuffers once. |
| **V-Sync control** | Expose `--vsync on|off|adaptive` via `SDL_GL_SetSwapInterval`. |
| **FPS counter** | Add `ImGui::Text("FPS: %.1f", io.Framerate);` behind `--show-fps`. |
| **GPU timer queries** | Optional `--gpu-profile` to measure frame GPU time (helps tune mesh-size). |
| **HDR / sRGB** | Create an sRGB default framebuffer, request 10-bit if available, improves banding. |
| **Swapchain resize** | Handle `SDL_WINDOWEVENT_RESIZED` properly: recreate FBO & ImGui viewport. |

## Audio & FFmpeg Integration

| Task | Why |
|---|---|
| **Audio abstraction** | Introduce `IAudioSource` interface: `FileAudioSource`, `SystemLoopbackSource` (PortAudio), `MicSource`. Removes `#ifdef` spaghetti. |
| **Resampler** | Use `libswresample` so any input sample-rate feeds projectM at 48 kHz. |
| **FFmpeg reusable class** | `Recorder` RAII wrapper that owns the pipe, writes raw RGBA frames + audio; supports graceful failure (log + continue instead of `exit()`). |
| **Zero-copy audio** | Send planar float directly to FFmpeg via `-f f32le -ar 48000 -ac 2` to avoid temp files. |
| **Lossless preset** | Provide `--preset-lossless` flag that writes FFV1 + FLAC in MKV for archival. |

## Preset Management

| Task | Why |
|---|---|
| **PresetStore** | Singleton that owns the vector of presets, history, favorites, broken list. Exposes `next()`, `prev()`, `random()`, `lock()`. |
| **File watching** | Detect new `.milk` files at runtime (nice with community packs). |
| **Preset scoring** | Store small `.json` sidecar per preset with rating & usage statistics → better shuffle weighting. |
| **Threaded preset load** | Decompress `.milk` on worker thread to avoid frame drops. |

## Text & Overlay

| Task | Why |
|---|---|
| **Dynamic line-wrap** | Use `ImFont::CalcTextSizeA` to split title at last space before `max_width = window.x * 0.9`. Cache result until string or size changes. |
| **Independent font sizes** | `song_info_font_size` vs `artist_font_size` in config. |
| **Contrast helper** | Sample 4 corner pixels of text bounding box, compute luminance, auto-switch to white outline if dark background. |
