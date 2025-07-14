#!/bin/bash

# A helper script for common, guided Git operations.
# Mode 1: Sync an external directory to a fresh clone of a repo.
# Mode 2: Commit all changes in the current Git repo.

# --- Configuration for --sync mode ---
SOURCE_DIR="$HOME/Documents/visualizer-gemini-pro"
GIT_URL="https://github.com/Nsomnia/suno-ai-song-to-music-video-creator.git"
CLONE_DIR_NAME="suno-ai-song-to-music-video-creator-temp-clone"
CLONE_DIR_PATH="$HOME/Documents/$CLONE_DIR_NAME"

# --- Colors for output ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Helper Function for Confirmation ---
confirm() {
    local prompt="$1"
    while true; do
        read -p "$(echo -e "${YELLOW}$prompt [y/n]: ${NC}")" response
        case "$response" in
            [yY]|[yY][eE][sS]) return 0 ;;
            [nN]|[nN][oO]) return 1 ;;
            *) echo -e "${RED}Invalid input. Please enter 'yes' or 'no'.${NC}" ;;
        esac
    done
}

# --- Function to Show Help Message ---
show_help() {
    echo -e "${GREEN}Git Helper Tool${NC}"
    echo "This script provides guided workflows for common Git tasks."
    echo
    echo -e "${YELLOW}Usage:${NC}"
    echo "  ./git_helper.sh [option]"
    echo
    echo -e "${YELLOW}Options:${NC}"
    echo -e "  ${GREEN}--sync${NC}          Runs the full sync process. Replaces a repo's content with an"
    echo "                 external directory's content. Use this to fix projects"
    echo "                 developed outside of Git."
    echo
    echo -e "  ${GREEN}--commit, -c${NC}    Runs a quick commit process. Assumes you are inside the correct"
    echo "                 Git repository. It will stage all changes, ask for a commit"
    echo "                 message, and push."
    echo
    echo -e "  ${GREEN}--help, -h${NC}      Displays this help message."
    echo
}

# --- Function for Sync/Replace Mode ---
do_sync() {
    echo -e "${GREEN}--- Starting Repository Sync Mode ---${NC}"
    echo "This will replace the contents of a fresh clone of a repository."
    echo "Source of new code: ${SOURCE_DIR}"
    echo "Git repository:     ${GIT_URL}"
    echo "-------------------------------------------"

    if [ -d "$CLONE_DIR_PATH" ]; then
        echo -e "${RED}Error: The target directory '${CLONE_DIR_PATH}' already exists.${NC}"
        echo "Please remove it before running this script again."
        exit 1
    fi

    if confirm "Step 1: Clone '${GIT_URL}' into '${CLONE_DIR_PATH}'?"; then
        echo "Cloning repository..."
        git clone "$GIT_URL" "$CLONE_DIR_PATH"
        echo -e "${GREEN}Repository cloned successfully.${NC}"
    else
        echo -e "${RED}Operation cancelled.${NC}"; exit 1
    fi

    cd "$CLONE_DIR_PATH"

    echo -e "\n${YELLOW}--- ACTION REQUIRED ---${NC}"
    echo "The script is paused. Please copy your new project files from:"
    echo -e "   ${GREEN}${SOURCE_DIR}${NC}"
    echo "Into the newly cloned Git directory:"
    echo -e "   ${GREEN}${CLONE_DIR_PATH}${NC}"
    echo -e "\n${YELLOW}When you are done, press [Enter] to continue...${NC}"
    read

    echo -e "\n${GREEN}Analyzing changes...${NC}"
    echo -e "${YELLOW}--- PENDING CHANGES ---${NC}"
    git status
    echo "--------------------------"

    if ! confirm "\nDoes this list of changes look correct?"; then
        echo -e "${RED}Operation cancelled.${NC}"; exit 1
    fi

    local COMMIT_MESSAGE=""
    while [ -z "$COMMIT_MESSAGE" ]; do
        read -p "Please enter a commit message: " COMMIT_MESSAGE
    done

    echo "Staging all changes..."
    git add .

    echo -e "\n${RED}--- FINAL CONFIRMATION ---${NC}"
    if confirm "Commit and PUSH all changes to GitHub?"; then
        echo "Committing and pushing..."
        git commit -m "$COMMIT_MESSAGE"
        git push
        echo -e "\n${GREEN}--- Sync Complete! ---${NC}"
        echo "GitHub repository has been updated."
        echo "You can now delete the temporary directory: ${CLONE_DIR_PATH}"
    else
        echo -e "${RED}Push cancelled.${NC}"; exit 1
    fi
}

# --- Function for Quick Commit Mode ---
do_commit() {
    echo -e "${GREEN}--- Starting Quick Commit Mode ---${NC}"

    if [ ! -d ".git" ]; then
        echo -e "${RED}Error: Not a Git repository.${NC}"
        echo "Please 'cd' into your project directory before running with --commit."
        exit 1
    fi

    echo "Showing current status of your repository:"
    echo -e "${YELLOW}--- PENDING CHANGES ---${NC}"
    git status
    echo "--------------------------"

    if ! confirm "\nStage all of the above changes for a new commit?"; then
        echo -e "${RED}Operation cancelled.${NC}"; exit 1
    fi
    
    local COMMIT_MESSAGE=""
    while [ -z "$COMMIT_MESSAGE" ]; do
        read -p "Please enter a commit message: " COMMIT_MESSAGE
    done
    
    echo "Staging all changes..."
    git add .

    echo -e "\n${RED}--- FINAL CONFIRMATION ---${NC}"
    if confirm "Commit with message '$COMMIT_MESSAGE' and push to remote?"; then
        echo "Committing and pushing..."
        git commit -m "$COMMIT_MESSAGE"
        git push
        echo -e "\n${GREEN}--- Push Complete! ---${NC}"
    else
        echo -e "${RED}Push cancelled.${NC}"; exit 1
    fi
}

# --- Main Script Logic: Parse Arguments ---
if [ "$#" -eq 0 ]; then
    show_help
    exit 0
fi

case "$1" in
    --sync)
        do_sync
        ;;
    --commit|-c)
        do_commit
        ;;
    --help|-h)
        show_help
        ;;
    *)
        echo -e "${RED}Error: Unknown option '$1'${NC}"
        show_help
        exit 1
        ;;
esac