#!/bin/bash

# Define the list of specific files to cat
specific_files=("GEMINI.md" "imgui.ini" "Makefile" "note-pdav-save-restore-devices" "README.md")

# Define the directories to search for files
directories=(config docs include src)

# Function to cat specific files
cat_specific_files() {
    for file in "${specific_files[@]}"; do
        if [ -f "$file" ]; then
            echo "Contents of $file:"
            cat "$file"
            echo ""
        else
            echo "File not found: $file"
        fi
    done
}

# Function to cat files in specified directories
cat_directory_contents() {
    for dir in "${directories[@]}"; do
        echo "Contents of $dir directory:"
        find "$dir" -type f -print0 | xargs -0 cat
        echo ""
    done
}

# Execute the functions
cat_specific_files
cat_directory_contents
