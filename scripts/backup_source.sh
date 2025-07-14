#!/bin/bash

BASE_BACKUP_DIR="/home/nsomnia/Documents/visualizer-gemini-pro/backups"
SUCCESSFUL_BUILD_ARCHIVE_DIR="$BASE_BACKUP_DIR/successful_builds"
SOURCE_ONLY_ARCHIVE_DIR="$BASE_BACKUP_DIR/source_only_backups"
MAX_ARCHIVES=20

# Function to manage old archives
cleanup_old_archives() {
    local archive_dir=$1
    local prefix=$2
    local archives_to_delete=$(ls -1t "$archive_dir" | grep "$prefix" | tail -n +$((MAX_ARCHIVES + 1)))
    if [ -n "$archives_to_delete" ]; then
        echo "Cleaning up old archives:"
        echo "$archives_to_delete"
        echo "$archives_to_delete" | xargs -d '\n' -I {} rm -- "$archive_dir/{}"
    fi
}

# Function to create a successful build archive (binary + source)
create_successful_build_archive() {
    local TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
    local ARCHIVE_NAME="visualizer_build_and_source_$TIMESTAMP.tar.xz"
    local ARCHIVE_PATH="$SUCCESSFUL_BUILD_ARCHIVE_DIR/$ARCHIVE_NAME"

    mkdir -p "$SUCCESSFUL_BUILD_ARCHIVE_DIR"

    echo "Creating successful build and source archive: $ARCHIVE_NAME"

    # Change to project root to use relative paths for tar
    cd /home/nsomnia/Documents/visualizer-gemini-pro || { echo "Failed to change directory to project root."; exit 1; }

    tar -cJf "$ARCHIVE_PATH" \
        ./src \
        ./include \
        ./config \
        ./Makefile \
        ./README.md \
        ./GEMINI.md \
        ./.gitignore \
        ./commit.sh \
        ./bin/backup_source.sh \
        ./bin/debug-with-gdb.sh \
        ./bin/remove-spaces-from-filenames.sh \
        ./bin/visualizer # Include the compiled binary

    if [ $? -eq 0 ]; then
        echo "Successful build and source archive created."
        cleanup_old_archives "$SUCCESSFUL_BUILD_ARCHIVE_DIR" "visualizer_build_and_source_"
    else
        echo "Failed to create successful build and source archive!"
        exit 1
    fi
}

# Function to create a source-only archive
create_source_only_archive() {
    local TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
    local ARCHIVE_NAME="visualizer_source_only_$TIMESTAMP.tar.xz"
    local ARCHIVE_PATH="$SOURCE_ONLY_ARCHIVE_DIR/$ARCHIVE_NAME"

    mkdir -p "$SOURCE_ONLY_ARCHIVE_DIR"

    echo "Creating source-only archive: $ARCHIVE_NAME"

    # Change to project root to use relative paths for tar
    cd /home/nsomnia/Documents/visualizer-gemini-pro || { echo "Failed to change directory to project root."; exit 1; }

    tar -cJf "$ARCHIVE_PATH" \
        ./src \
        ./include \
        ./config \
        ./Makefile \
        ./README.md \
        ./GEMINI.md \
        ./.gitignore \
        ./commit.sh \
        ./bin/backup_source.sh \
        ./bin/debug-with-gdb.sh \
        ./bin/remove-spaces-from-filenames.sh

    if [ $? -eq 0 ]; then
        echo "Source-only archive created."
        cleanup_old_archives "$SOURCE_ONLY_ARCHIVE_DIR" "visualizer_source_only_"
    else
        echo "Failed to create source-only archive!"
        exit 1
    fi
}

# Main logic based on argument
case "$1" in
    "successful_build")
        create_successful_build_archive
        ;;
    "source_only")
        create_source_only_archive
        ;;
    *) 
        echo "Usage: $0 [successful_build|source_only]"
        exit 1
        ;;
esac
