#!/usr/bin/env bash

# Find all .mp3 files recursively under ~/Music, handle special characters safely
mapfile -d '' mp3_files < <(find ~/Music -type f -iname '*.mp3' -print0)

# Exit if no mp3 files found
if (( ${#mp3_files[@]} == 0 )); then
    echo "No .mp3 files found in ~/Music."
    exit 1
fi

# Pick a random index
random_index=$(( RANDOM % ${#mp3_files[@]} ))

# Get the selected file
selected_file="${mp3_files[$random_index]}"

# Run the visualizer with correct quoting
exec build/AuroraVisualizer --record-video --audio-file "$selected_file"
