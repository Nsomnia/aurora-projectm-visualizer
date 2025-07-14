#!/bin/bash

# Script to upload files and directories recursively to a GitHub repository
# Uses git and gh (GitHub CLI) commands

# Repository URL (set as a variable)
REPO_URL="https://github.com/Nsomnia/suno-ai-song-to-music-video-creator.git"

# Directory to upload (default to current directory)
UPLOAD_DIR="$(pwd)"

# Ensure Git is installed
if ! command -v git &> /dev/null; then
    echo "Error: Git is not installed. Please install Git first."
    echo "Visit https://git-scm.com/downloads for instructions."
    exit 1
fi

# Ensure GitHub CLI (gh) is installed
if ! command -v gh &> /dev/null; then
    echo "Error: GitHub CLI (gh) is not installed. Please install it first."
    echo "Visit https://cli.github.com/ for instructions."
    exit 1
fi

# Check if the current directory is a Git repository
if [ ! -d ".git" ]; then
    echo "This directory is not a Git repository. Initializing a new Git repository..."
    git init || { echo "Error: Failed to initialize Git repository."; exit 1; }
fi

# Check if the repository is linked to the remote
if ! git remote get-url origin &> /dev/null; then
    echo "Setting remote repository to $REPO_URL..."
    git remote add origin "$REPO_URL" || { echo "Error: Failed to set remote repository."; exit 1; }
else
    CURRENT_REMOTE=$(git remote get-url origin)
    if [ "$CURRENT_REMOTE" != "$REPO_URL" ]; then
        echo "Warning: Current remote URL ($CURRENT_REMOTE) does not match the specified URL ($REPO_URL)."
        read -p "Do you want to update the remote URL to $REPO_URL? (y/n): " UPDATE_REMOTE
        if [ "$UPDATE_REMOTE" = "y" ] || [ "$UPDATE_REMOTE" = "Y" ]; then
            git remote set-url origin "$REPO_URL" || { echo "Error: Failed to update remote URL."; exit 1; }
            echo "Remote URL updated to $REPO_URL."
        else
            echo "Keeping existing remote URL."
        fi
    fi
fi

# Check if authenticated with GitHub CLI
if ! gh auth status &> /dev/null; then
    echo "You are not authenticated with GitHub CLI. Running 'gh auth login'..."
    gh auth login || { echo "Error: GitHub CLI authentication failed."; exit 1; }
fi

# Get the current local branch
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [ -z "$CURRENT_BRANCH" ]; then
    echo "Error: Could not determine current branch."
    exit 1
fi
echo "Current local branch: $CURRENT_BRANCH"

# Get the remote repository's default branch using gh
REMOTE_BRANCH=$(gh repo view Nsomnia/suno-ai-song-to-music-video-creator --json defaultBranchRef --jq .defaultBranchRef.name 2>/dev/null)
if [ -z "$REMOTE_BRANCH" ]; then
    echo "Warning: Could not determine remote default branch. Assuming 'main'."
    REMOTE_BRANCH="main"
else
    echo "Remote default branch: $REMOTE_BRANCH"
fi

# Check if local branch matches remote default branch
if [ "$CURRENT_BRANCH" != "$REMOTE_BRANCH" ]; then
    echo "Warning: Local branch ($CURRENT_BRANCH) does not match remote default branch ($REMOTE_BRANCH)."
    read -p "Do you want to rename local branch '$CURRENT_BRANCH' to '$REMOTE_BRANCH'? (y/n): " RENAME_BRANCH
    if [ "$RENAME_BRANCH" = "y" ] || [ "$RENAME_BRANCH" = "Y" ]; then
        git branch -m "$CURRENT_BRANCH" "$REMOTE_BRANCH" || { echo "Error: Failed to rename branch."; exit 1; }
        echo "Renamed local branch to $REMOTE_BRANCH."
        CURRENT_BRANCH="$REMOTE_BRANCH"
    else
        echo "Keeping local branch as $CURRENT_BRANCH. You may need to push to a matching branch or create a new one."
    fi
fi

# Prompt for commit message
read -p "Enter commit message: " COMMIT_MESSAGE
if [ -z "$COMMIT_MESSAGE" ]; then
    COMMIT_MESSAGE="Update files and directories"
    echo "No commit message provided. Using default: '$COMMIT_MESSAGE'"
fi

# Add all files and directories recursively
echo "Adding all files and directories in $UPLOAD_DIR..."
git add . || { echo "Error: Failed to stage files."; exit 1; }

# Check if there are changes to commit
if git diff --staged --quiet; then
    echo "No changes to commit."
    exit 0
fi

# Commit changes
echo "Committing changes with message: '$COMMIT_MESSAGE'..."
git commit -m "$COMMIT_MESSAGE" || { echo "Error: Failed to commit changes."; exit 1; }

# Push to the current branch
echo "Pushing changes to $REPO_URL (branch: $CURRENT_BRANCH)..."
git push origin "$CURRENT_BRANCH" || { echo "Error: Failed to push to repository."; exit 1; }

# Verify push with GitHub CLI
echo "Verifying repository state on GitHub..."
gh repo view Nsomnia/suno-ai-song-to-music-video-creator || { echo "Warning: Could not verify repository state."; }

echo "Successfully uploaded files to $REPO_URL!"