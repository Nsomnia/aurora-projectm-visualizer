# Future-Proofing & Extensibility

This document outlines ideas for future-proofing and extending the Aurora Visualizer.

| Idea | Benefit |
|---|---|
| **Plugin SDK** | Dynamic `.so`/`.dll` loaded via `dlopen` exposing `IVizModule` interface → allow GLSL shader visualizers, NotchLC, etc. |
| **MaterialX / GLSL node editor** | Let power-users create presets visually, export to `.milk` or SPIR-V. |
| **WebAssembly build** | Compile with `emscripten`, run in browser for sharing on social media. |
| **Headless server** | Docker image that listens to HTTP POST `/render` with audio file + params, returns MP4. |
| **Whisper lyrics** | Offline `whisper.cpp` for speech-to-text → LRC file → karaoke overlay. |
| **GPU FFT** | Use `VkFFT` or `OpenCL` to offload FFT from CPU → lower latency, higher FPS. |
