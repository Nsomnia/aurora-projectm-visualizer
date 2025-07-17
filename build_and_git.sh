#!/bin/bash

# Color codes for pretty output
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
MAGENTA="\033[0;35m"
CYAN="\033[0;36m"
NC="\033[0m" # No Color

# Bold text
BOLD="\033[1m"
UNBOLD="\033[0m"

# --- Usage Information ---
usage() {
    echo -e "${BOLD}Usage:${NC} $0 [command]"
    echo -e "  ${BOLD}Commands:${NC}"
    echo -e "    ${CYAN}compile${NC}       : Configure and build the project."
    echo -e "    ${CYAN}git${NC}           : Guide through basic Git workflow (status, add, commit, push)."
    echo -e "    ${CYAN}-h, --help${NC}    : Display this help message."
    echo ""
    echo -e "${BOLD}Description:${NC}"
    echo "  This script helps you compile the Aurora Visualizer project and manage basic Git operations."
    echo "  It's designed for beginners to simplify the development workflow."
    echo ""
    echo -e "${BOLD}Git Diff Explanation:${NC}"
    echo -e "  When you see a 'git diff' output, pay attention to these symbols and colors:"
    echo -e "  ${GREEN}+${NC} : A line that has been ${GREEN}added${NC} to the file."
    echo -e "  ${RED}-${NC} : A line that has been ${RED}removed${NC} from the file."
    echo -e "  ${CYAN}Lines starting with '@@'${NC} indicate the section of the file that has changed."
    echo -e "  ${YELLOW}Lines without '+' or '-'${NC} are context lines, showing surrounding code for clarity."
    echo ""
    echo -e "${BOLD}Important Git Notes:${NC}"
    echo -e "  * Always review changes before committing."
    echo -e "  * Commit frequently with clear messages."
    echo -e "  * If you make a mistake, ask for help before trying to revert complex changes."
}

# --- Compile Project Function ---
compile_project() {
    echo -e "${BLUE}${BOLD}--- Compiling Aurora Visualizer ---${NC}"
    echo -e "${YELLOW}Configuring CMake...${NC}"
    cmake -B build
    if [ $? -ne 0 ]; then
        echo -e "${RED}${BOLD}Error:${NC} CMake configuration failed."
        exit 1
    fi

    echo -e "${YELLOW}Building project...${NC}"
    cmake --build build
    if [ $? -ne 0 ]; then
        echo -e "${RED}${BOLD}Error:${NC} Project build failed."
        exit 1
    fi

    echo -e "${GREEN}${BOLD}Compilation successful!${NC}"
    echo -e "${GREEN}Executable is located at: ./build/bin/AuroraVisualizer${NC}"
    echo -e "${YELLOW}Remember to run 'git' command to manage your changes!${NC}"
}

# --- Git Workflow Function ---
git_workflow() {
    echo -e "${BLUE}${BOLD}--- Git Workflow ---${NC}"

    # 1. Git Status
    echo -e "${YELLOW}Checking Git status...${NC}"
    git status
    echo ""

    read -p "$(echo -e "${BOLD}Press Enter to stage all changes (git add .)...${NC}")"
    git add .
    echo -e "${GREEN}All changes staged.${NC}"
    echo ""

    # 2. Show Staged Changes
    echo -e "${YELLOW}Reviewing staged changes (git diff --staged)...${NC}"
    git diff --staged
    echo ""
    echo -e "${BOLD}Please review the changes above. Refer to the 'Git Diff Explanation' in the help message.${NC}"
    echo ""

    # 3. Commit
    read -p "$(echo -e "${BOLD}Enter your commit message (e.g., 'feat: Add new feature', 'fix: Resolve bug'):${NC} ")" commit_message
    if [ -z "$commit_message" ]; then
        echo -e "${RED}${BOLD}Error:${NC} Commit message cannot be empty. Aborting commit."
        exit 1
    fi

    echo -e "${YELLOW}Committing changes...${NC}"
    git commit -m "$commit_message"
    if [ $? -ne 0 ]; then
        echo -e "${RED}${BOLD}Error:${NC} Git commit failed. Please resolve any conflicts or issues."
        exit 1
    fi
    echo -e "${GREEN}Changes committed successfully!${NC}"
    echo ""

    # 4. Push
    read -p "$(echo -e "${BOLD}Do you want to push these changes to the remote repository? (y/N):${NC} ")" -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${YELLOW}Pushing changes...${NC}"
        git push
        if [ $? -ne 0 ]; then
            echo -e "${RED}${BOLD}Error:${NC} Git push failed. You might need to pull first or resolve conflicts."
            exit 1
        fi
        echo -e "${GREEN}Changes pushed successfully!${NC}"
    else
        echo -e "${YELLOW}Skipping push. Your changes are committed locally.${NC}"
    fi

    echo -e "${BLUE}${BOLD}--- Git Workflow Complete ---${NC}"
}

# --- Main Script Logic ---
if [ "$#" -eq 0 ]; then
    usage
    exit 0
fi

case "$1" in
    compile)
        compile_project
        ;;
    git)
        git_workflow
        ;;
    -h|--help|--usage)
        usage
        ;;
    *)
        echo -e "${RED}${BOLD}Error:${NC} Unknown command: $1"
        usage
        exit 1
        ;;
esac
