# GUI Development Plan

This document tracks the progress of building the Aurora Visualizer's Qt-based GUI.

## Phase 1: Foundation (Complete)

- [x] Created the main application window with a menu bar (`File`, `Help`).
- [x] Implemented a dockable panel system.
- [x] Created the first dock panel, "Controls".
- [x] Migrated the "Preset Duration" slider and "Shuffle Presets" checkbox into the "Controls" panel.
- [x] Added an "About" dialog.

## Phase 2: Playlist

- [x] Create a dockable "Playlist" panel.
- [x] Display the list of audio files from the config.
- [x] Allow adding/removing files from the playlist.
- [x] Allow reordering files in the playlist.
- [x] Implement playback controls (Play, Pause, Stop, Next, Previous).

## Phase 3: Visualizer Controls

- [x] Create a dockable "Visualizer" panel.
- [x] Display the current preset file.
- [x] Implement Next/Previous preset buttons.
- [ ] Implement a "Favorite" button to add the current preset to the favorites list.
- [ ] Display the list of favorite presets.
- [x] Add option to shuffle only favorite presets.

## Phase 4: Settings & Configuration

- [ ] Create a "Settings" dialog.
- [ ] Add controls for all major configuration options.
- [ ] Implement logic to save all settings to `~/.config/aurora-visualizer/config.toml`.
- [ ] Save the playlist and favorites to files in the same directory.

## Phase 5: Advanced Features

- [ ] Implement refined recording logic with temporary files and playback verification.
- [ ] Add keyboard shortcuts for all major actions.
