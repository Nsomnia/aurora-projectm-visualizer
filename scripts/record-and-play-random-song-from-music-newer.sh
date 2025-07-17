#!/usr/bin/env bash

# Recursively find all .mp3 files under the given path
mapfile -d '' mp3_files < <(find ~/Music/cajun-witch-bitch-stomp-rolling-tumbling/ -type f -iname '*.mp3' -print0)

# Exit if no mp3 files found
if (( ${#mp3_files[@]} == 0 )); then
    echo "No .mp3 files found in ~/Music/cajun-witch-bitch-stomp-rolling-tumbling/."
    exit 1
fi

# Infinite loop to keep playing random mp3 files
while true; do
    # Pick a random index
    random_index=$(( RANDOM % ${#mp3_files[@]} ))

    # Get the selected file
    selected_file="${mp3_files[$random_index]}"

    echo "Now playing: $selected_file"

    # Run the visualizer and wait for it to finish
#    build/AuroraVisualizer --record-video --audio-file "$selected_file"
    AuroraVisualizer-latest --record-video --audio-file "$selected_file" --preset-list-file /home/nsomnia/Documents/zzzz_VISUALIZER-WORKING-COPY/config/presets.txt

    # Optional: add delay or skip logic
    echo "Finished: $selected_file"
    sleep 1
done


