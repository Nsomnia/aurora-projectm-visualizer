# GUI Development Plan

This document tracks the progress of building the Aurora Visualizer's Qt-based GUI.

## Phase 1: Foundation (Complete)

- [x] Created the main application window with a menu bar (`File`, `Help`).
- [x] Implemented a dockable panel system.
- [x] Created the first dock panel, "Controls".
- [x] Migrated the "Preset Duration" slider and "Shuffle Presets" checkbox into the "Controls" panel.
- [x] Added an "About" dialog.

## Phase 2: Playlist

- [ ] Create a dockable "Playlist" panel.
- [ ] Display the list of audio files from the config.
- [ ] Allow adding/removing files from the playlist.
- [ ] Allow reordering files in the playlist.
- [ ] Implement playback controls (Play, Pause, Stop, Next, Previous).

## Phase 3: Visualizer Controls

- [ ] Create a dockable "Visualizer" panel.
- [ ] Display the current preset file.
- [ ] Implement Next/Previous preset buttons.
- [ ] Implement a "Favorite" button to add the current preset to the favorites list.
- [ ] Display the list of favorite presets.

## Phase 4: Settings & Configuration

- [ ] Create a "Settings" dialog.
- [ ] Add controls for all major configuration options.
- [ ] Implement logic to save all settings to `~/.config/aurora-visualizer/config.toml`.
- [ ] Save the playlist and favorites to files in the same directory.

## Phase 5: Advanced Features

- [ ] Implement refined recording logic with temporary files and playback verification.
- [ ] Add keyboard shortcuts for all major actions.
