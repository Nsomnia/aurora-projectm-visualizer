# GUI Front-End Blueprint

This document outlines the architectural blueprint for the GUI front-end, considering both Qt and GTK alternatives.

## Architecture Blueprint

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

*   **Rationale**: Keeps heavy OpenGL + FFmpeg + projectM in a sandbox that can crash/restart without losing UI state. Enables future headless cloud rendering.

## Qt Quick (QML) Starter Plan

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

## GTK (gtkmm 4) Alternative

| Component | Tech |
|---|---|
| **Window** | `Gtk::ApplicationWindow` + `Gtk::GLArea` |
| **Preferences** | `Gtk::PreferencesWindow` with `Adw` (libadwaita) |
| **Playlist** | `Gtk::ColumnView` + `Gtk::SingleSelection` |
| **Batch view** | `Gtk::ListBox` of `Gtk::ProgressBar` rows |
| **Batch queue** | `QList<QFuture>` + `QtConcurrent` | Progress bar, cancel button |
| **Thumbnail preview** | Render first 5 s of each preset to tiny WebP in worker thread | 128×128 thumbnails load in < 50 ms |
| **Hotkey map editor** | `QKeySequenceEdit` | Store platform-neutral `QKeySequence::PortableText` |
| **Theme switcher** | `QStyleHints` + `Material` style | Light/Dark/auto |
| **IPC** | `Gio::Subprocess` + `Gio::UnixConnection` (D-Bus for flatpak) |
