#!/usr/bin/env python3

import os
import random
import subprocess
import sys
from pathlib import Path

def main():
    """
    Finds all .mp3 files in the user's music directory, shuffles them,
    and then creates a video for each song using the AuroraVisualizer.
    """
    music_dir = Path.home() / "Music"

    if not music_dir.is_dir():
        print(f"Error: Music directory not found at {music_dir}", file=sys.stderr)
        sys.exit(1)

    print("Searching for .mp3 files...")
    # Use rglob for recursive search. This is case-sensitive on Linux,
    # so we search for both .mp3 and .MP3.
    song_list = list(music_dir.rglob("*.mp3")) + list(music_dir.rglob("*.MP3"))
    
    if not song_list:
        print(f"No .mp3 files found in {music_dir}.", file=sys.stderr)
        sys.exit(1)

    random.shuffle(song_list)
    
    total_songs = len(song_list)
    print(f"Found {total_songs} songs. Starting video creation process...")

    visualizer_executable = "./build/AuroraVisualizer"

    if not Path(visualizer_executable).is_file():
        print(f"Error: Visualizer executable not found at {visualizer_executable}", file=sys.stderr)
        print("Please build the project first using 'make'.", file=sys.stderr)
        sys.exit(1)

    for i, song_path in enumerate(song_list):
        song_number = i + 1
        print("--------------------------------------------------")
        print(f"Processing song {song_number} of {total_songs}: {song_path.name}")
        print("--------------------------------------------------")

        try:
            # Use a list of arguments to avoid shell injection and handle paths correctly.
            command = [
                visualizer_executable,
                "--record-video",
                "--audio-file",
                str(song_path) # Convert Path object to string
            ]
            
            # We use subprocess.run which waits for the command to complete.
            result = subprocess.run(command, check=False, capture_output=True, text=True)

            if result.returncode != 0:
                print(f"Warning: AuroraVisualizer exited with code {result.returncode} for song: {song_path.name}", file=sys.stderr)
                print("STDOUT:", file=sys.stderr)
                print(result.stdout, file=sys.stderr)
                print("STDERR:", file=sys.stderr)
                print(result.stderr, file=sys.stderr)
            else:
                print(f"Successfully created video for: {song_path.name}")
                # Optionally print stdout for successful runs if needed for debugging
                # if result.stdout:
                #     print(result.stdout)

        except FileNotFoundError:
            print(f"Error: Could not find the visualizer executable at {visualizer_executable}", file=sys.stderr)
            sys.exit(1)
        except Exception as e:
            print(f"An unexpected error occurred while processing {song_path.name}: {e}", file=sys.stderr)

    print("--------------------------------------------------")
    print("All songs processed. Video creation complete.")
    print("--------------------------------------------------")

if __name__ == "__main__":
    main()
