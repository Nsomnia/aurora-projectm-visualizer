#!/bin/bash

# git_manager.sh - A utility script for managing Git operations in the Aurora Visualizer project.
# Designed to be idiot-proof, future-proof, and easily maintainable.

# --- Configuration ---
REPO_URL="https://github.com/Nsomnia/suno-ai-song-to-music-video-creator"
MAIN_BRANCH="main"
EXCLUDE_FILES=(".env" "project_structure_improvements.md" "backups/" "scripts/" "build/" "deps/") # Files/directories to never commit

# --- Colors (ANSI Escape Codes) ---
RED='[0;31m'
GREEN='[0;32m'
YELLOW='[0;33m'
BLUE='[0;34m'
MAGENTA='[0;35m'
CYAN='[0;36m'
NC='[0m' # No Color

# --- Helper Functions ---

# Function to check if the current directory is a Git repository
is_git_repo() {
    git rev-parse --is-inside-work-tree &>/dev/null
}

# Function to display error messages
error_message() {
    echo -e "${RED}ERROR: $1${NC}" >&2
}

# Function to display warning messages
warning_message() {
    echo -e "${YELLOW}WARNING: $1${NC}"
}

# Function to display info messages
info_message() {
    echo -e "${CYAN}INFO: $1${NC}"
}

# Function to display success messages
success_message() {
    echo -e "${GREEN}SUCCESS: $1${NC}"
}

# Function to get user confirmation
confirm_action() {
    read -p "$(echo -e "${YELLOW}$1 (y/N)? ${NC}")" -n 1 -r
    echo # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        return 0
    else
        return 1
    fi
}

# Function to display the help and usage information
display_help() {
    echo -e "${BLUE}--- Aurora Visualizer Git Manager Help ---${NC}"
    echo -e "This script helps manage Git operations for the Aurora Visualizer project."
    echo -e "It ensures consistency with the main repository and prevents accidental commits of sensitive files."
    echo
    echo -e "${YELLOW}Usage:${NC} $0 [command] [options]"
    echo
    echo -e "${MAGENTA}Available Commands:${NC}"
    echo -e "  ${GREEN}status${NC}                 Show the working tree status."
    echo -e "  ${GREEN}diff${NC}                   Show changes between commits, commit and working tree, etc."
    echo -e "  ${GREEN}log${NC}                    Show commit logs (last 5 commits)."
    echo -e "  ${GREEN}add [files...]${NC}       Add file contents to the index. If no files are specified, adds all changes, excluding ignored files."
    echo -e "  ${GREEN}commit [message]${NC}     Record changes to the repository. If no message is provided, prompts for one."
    echo -e "  ${GREEN}pull${NC}                   Fetch from and integrate with another repository or a local branch."
    echo -e "  ${GREEN}push${NC}                   Update remote refs along with associated objects."
    echo -e "  ${GREEN}sync${NC}                   Perform a full synchronization: pull, add all, commit (if changes), and push."
    echo -e "  ${GREEN}help${NC}                   Display this help message."
    echo
    echo -e "${MAGENTA}Options:${NC}"
    echo -e "  ${GREEN}-h, --help, --usage${NC}  Display this help message."
    echo
    echo -e "${MAGENTA}Important Notes:${NC}"
    echo -e "  - This script operates on the '${MAIN_BRANCH}' branch."
    echo -e "  - The following files/directories are ${RED}EXCLUDED${NC} from 'add' operations to prevent accidental commits:"
    for item in "${EXCLUDE_FILES[@]}"; do
        echo -e "    - ${YELLOW}${item}${NC}"
    done
    echo -e "  - Always review changes using '${GREEN}status${NC}' and '${GREEN}diff${NC}' before committing or pushing."
    echo -e "  - For critical operations, you will be prompted for confirmation."
    echo -e "${BLUE}------------------------------------------${NC}"
}

# Function to handle git status
git_status() {
    info_message "Running git status..."
    git status
}

# Function to handle git diff
git_diff() {
    info_message "Running git diff HEAD..."
    git diff HEAD
}

# Function to handle git log
git_log() {
    info_message "Running git log -n 5..."
    git log -n 5
}

# Function to handle git add
git_add() {
    info_message "Adding files to staging area..."
    local files_to_add=("$@")
    local exclude_args=()
    for item in "${EXCLUDE_FILES[@]}"; do
        exclude_args+=(":(exclude)$item")
    done

    if [ ${#files_to_add[@]} -eq 0 ]; then
        # Add all changes, respecting .gitignore and custom excludes
        git add . "${exclude_args[@]}"
    else
        # Add specific files, still respecting custom excludes
        for file in "${files_to_add[@]}"; do
            local excluded=false
            for exclude_item in "${EXCLUDE_FILES[@]}"; do
                if [[ "$file" == *"$exclude_item"* ]]; then
                    warning_message "Skipping '$file' as it matches an excluded pattern: '$exclude_item'."
                    excluded=true
                    break
                fi
            done
            if [ "$excluded" = false ]; then
                git add "$file"
            fi
        done
    fi

    if [ $? -eq 0 ]; then
        success_message "Files added successfully."
    else
        error_message "Failed to add files."
    fi
}

# Function to handle git commit
git_commit() {
    local commit_message="$1"
    if [ -z "$commit_message" ]; then
        read -p "$(echo -e "${YELLOW}Enter commit message: ${NC}")" commit_message
        if [ -z "$commit_message" ]; then
            error_message "Commit message cannot be empty. Aborting commit."
            return 1
        fi
    fi

    info_message "Committing changes with message: "$commit_message""
    git commit -m "$commit_message"
    if [ $? -eq 0 ]; then
        success_message "Changes committed successfully."
    else
        error_message "Failed to commit changes. Please check 'git status' for details."
    fi
}

# Function to handle git pull
git_pull() {
    info_message "Pulling latest changes from ${MAIN_BRANCH}..."
    if confirm_action "Are you sure you want to pull? This might overwrite local changes."; then
        git pull origin "${MAIN_BRANCH}"
        if [ $? -eq 0 ]; then
            success_message "Pull successful."
        else
            error_message "Failed to pull changes."
        fi
    else
        info_message "Pull aborted."
    fi
}

# Function to handle git push
git_push() {
    info_message "Pushing changes to ${MAIN_BRANCH}..."
    if confirm_action "Are you sure you want to push changes to the remote repository?"; then
        git push origin "${MAIN_BRANCH}"
        if [ $? -eq 0 ]; then
            success_message "Push successful."
        else
            error_message "Failed to push changes."
        fi
    else
        info_message "Push aborted."
    fi
}

# Function to handle full synchronization
git_sync() {
    info_message "Starting full synchronization process..."
    git_pull || { error_message "Sync failed during pull."; return 1; }
    git_add || { error_message "Sync failed during add."; return 1; }
    
    # Check if there's anything to commit after adding
    if ! git diff --cached --quiet; then
        git_commit || { error_message "Sync failed during commit."; return 1; }
    else
        info_message "No new changes to commit after pull and add."
    fi
    
    git_push || { error_message "Sync failed during push."; return 1; }
    success_message "Synchronization complete."
}

# --- Main Logic ---

# Check for help flags first
if [[ "$1" == "-h" || "$1" == "--help" || "$1" == "--usage" ]]; then
    display_help
    exit 0
fi

# Check if inside a Git repository
if ! is_git_repo; then
    error_message "Not a Git repository. Please run this script from the project root or a subdirectory of a Git repository."
    exit 1
fi

# Process commands
case "$1" in
    status)
        git_status
        ;;
    diff)
        git_diff
        ;;
    log)
        git_log
        ;;
    add)
        shift # Remove 'add' from arguments
        git_add "$@"
        ;;
    commit)
        shift # Remove 'commit' from arguments
        git_commit "$@"
        ;;
    pull)
        git_pull
        ;;
    push)
        git_push
        ;;
    sync)
        git_sync
        ;;
    *)
        error_message "Invalid command or no command provided."
        display_help
        exit 1
        ;;
esac

exit 0
