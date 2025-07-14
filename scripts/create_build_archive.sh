#!/bin/bash

# This script creates a compressed archive of the project's source code,
# build files, and documentation. It's intended to be run after a
# successful build.

set -e

BACKUP_DIR="backups"
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
ARCHIVE_NAME="aurora_visualizer_backup_${TIMESTAMP}.tar.xz"
ARCHIVE_PATH="${BACKUP_DIR}/${ARCHIVE_NAME}"

# Ensure the backup directory exists
mkdir -p "${BACKUP_DIR}"

# List of files and directories to include in the archive
# Add any new files or directories here as the project grows.
FILES_TO_ARCHIVE=(
    "src"
    "include"
    "config"
    "Makefile"
    "CMakeLists.txt"
    "README.md"
    "TODO.md"
    "commit.sh"
    "setup-context-chat-for-ai-chat-agent.sh"
    "list-all-files.sh"
    "build-ai-message-of-all-source-code.sh"
    ".gitignore"
    "message.txt"
    "imgui.ini"
)

echo "Creating backup archive at: ${ARCHIVE_PATH}"

# Create the compressed archive
tar -cJf "${ARCHIVE_PATH}" --exclude-from=<(echo "build/*"; echo "backups/*"; echo "videos/*") "${FILES_TO_ARCHIVE[@]}"

echo "Backup created successfully."
