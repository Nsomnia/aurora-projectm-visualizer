#!/bin/bash
# Compatible with zsh

# Script to interact with uithub.com for fetching GitHub repository context for LLMs

# ANSI color codes for help output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to display help message
show_help() {
    echo -e "${BLUE}Usage:${NC} $(basename "$0") [OPTIONS] <GitHub URL or owner/repo>"
    echo -e "${BLUE}Description:${NC} Fetches repository context from uithub.com for LLM input by converting GitHub URLs."
    echo
    echo -e "${YELLOW}Options:${NC}"
    echo -e "  ${GREEN}-h, --help${NC}        Display this help message and exit"
    echo -e "  ${GREEN}-a, --api-key${NC}     Specify API key for uithub.com API access (overrides ~/.config/git-api-access-token.key)"
    echo -e "  ${GREEN}-o, --output${NC}      Specify output file base name (appends YYYYMMDD_HHMMSS to prevent overwrites; default: stdout)"
    echo -e "  ${GREEN}-f, --format${NC}      Output format: 'raw' (default), 'json', 'markdown', 'html', 'yaml'"
    echo -e "  ${GREEN}-e, --ext${NC}         File extensions to include (e.g., 'py,md')"
    echo -e "  ${GREEN}-d, --dir${NC}         Directory to fetch (e.g., 'src')"
    echo -e "  ${GREEN}-l, --lines${NC}       Include line numbers (true/false)"
    echo -e "  ${GREEN}-x, --exclude-ext${NC} Exclude file extensions (e.g., 'pyc,log')"
    echo -e "  ${GREEN}-X, --exclude-dir${NC} Exclude directories (e.g., 'node_modules,dist')"
    echo -e "  ${GREEN}-g, --genignore${NC}  Enable .gitignore filtering (true/false)"
    echo -e "  ${GREEN}-s, --max-size${NC}    Max file size in bytes"
    echo -e "  ${GREEN}-t, --max-tokens${NC}  Max tokens for response"
    echo -e "  ${GREEN}-y, --yaml${NC}        YAML string for custom filtering"
    echo -e "  ${GREEN}-F, --omit-files${NC}  Omit file contents (true/false)"
    echo -e "  ${GREEN}-T, --omit-tree${NC}   Omit directory tree (true/false)"
    echo
    echo -e "${YELLOW}Arguments:${NC}"
    echo -e "  ${GREEN}<GitHub URL or owner/repo>${NC}  GitHub repository URL (e.g., https://github.com/owner/repo) or shorthand (owner/repo)"
    echo
    echo -e "${YELLOW}Examples:${NC}"
    echo -e "  $(basename "$0") Nsomnia/suno-ai-song-to-music-video-creator"
    echo -e "  $(basename "$0") https://github.com/Nsomnia/suno-ai-song-to-music-video-creator -o context -f markdown"
    echo -e "  $(basename "$0") -a <your-api-key> owner/repo -e py,md -d src -l true"
    echo
    echo -e "${YELLOW}Notes:${NC}"
    echo -e "  - API key is read from ~/.config/git-api-access-token.key if not specified with -a."
    echo -e "  - Output files append a timestamp (e.g., context_20250711_173125.txt) to prevent overwrites."
    echo -e "  - Invalid URLs or parameters will display an error with redirect details if applicable."
    echo -e "  - See https://uithub.com/openapi.html for API details."
    exit 0
}

# Function to validate and normalize repository input
normalize_repo_input() {
    local input="$1"
    # Check if input is a full URL
    if [[ "$input" =~ ^https?://github\.com/([^/]+)/([^/]+) ]]; then
        repo="${BASH_REMATCH[1]}/${BASH_REMATCH[2]}"
        echo "$repo"
    elif [[ "$input" =~ ^[a-zA-Z0-9_-]+/[a-zA-Z0-9_-]+$ ]]; then
        echo "$input"
    else
        echo -e "${RED}Error:${NC} Invalid repository format. Use 'owner/repo' or a valid GitHub URL."
        show_help
        exit 1
    fi
}

# Function to convert GitHub URL to uithub.com URL
convert_to_uithub_url() {
    local repo="$1"
    echo "https://uithub.com/$repo/tree/main"
}

# Function to read API key from file
read_api_key() {
    local api_key_file="$HOME/.config/git-api-access-token.key"
    if [[ -f "$api_key_file" ]]; then
        cat "$api_key_file" | tr -d '[:space:]' # Remove whitespace
    else
        echo ""
    fi
}

# Function to fetch repository context
fetch_context() {
    local url="$1"
    local api_key="$2"
    local output_file="$3"
    local format="$4"
    local ext="$5"
    local dir="$6"
    local lines="$7"
    local exclude_ext="$8"
    local exclude_dir="$9"
    local genignore="${10}"
    local max_size="${11}"
    local max_tokens="${12}"
    local yaml_string="${13}"
    local omit_files="${14}"
    local omit_tree="${15}"

    # Check if API key is provided
    if [[ -z "$api_key" ]]; then
        echo -e "${RED}Error:${NC} No API key provided and ~/.config/git-api-access-token.key not found."
        show_help
        exit 1
    fi

    # Build query parameters
    local query="apiKey=$api_key"
    [[ -n "$ext" ]] && query="$query&ext=$ext"
    [[ -n "$dir" ]] && query="$query&dir=$dir"
    [[ -n "$lines" ]] && query="$query&lines=$lines"
    [[ -n "$exclude_ext" ]] && query="$query&exclude-ext=$exclude_ext"
    [[ -n "$exclude_dir" ]] && query="$query&exclude-dir=$exclude_dir"
    [[ -n "$genignore" ]] && query="$query&disableGenignore=$genignore"
    [[ -n "$max_size" ]] && query="$query&maxFileSize=$max_size"
    [[ -n "$max_tokens" ]] && query="$query&maxTokens=$max_tokens"
    [[ -n "$yaml_string" ]] && query="$query&yamlString=$yaml_string"
    [[ -n "$omit_files" ]] && query="$query&omitFiles=$omit_files"
    [[ -n "$omit_tree" ]] && query="$query&omitTree=$omit_tree"
    [[ "$format" != "raw" ]] && query="$query&accept=text%2F$format"

    # Build curl command with redirect following
    local curl_cmd="curl -L -s --request GET"
    curl_cmd="$curl_cmd --url '$url?$query'"
    curl_cmd="$curl_cmd --header 'Accept: application/json, text/yaml, text/markdown, text/html, text/plain'"
    curl_cmd="$curl_cmd --header 'accept: text/$format'"
    curl_cmd="$curl_cmd --write-out '%{http_code}|%{url_effective}'"

    # Execute curl command and capture output and metadata
    response=$(eval "$curl_cmd")
    curl_exit_code=$?
    http_code=$(echo "$response" | tail -n1 | cut -d'|' -f1)
    final_url=$(echo "$response" | tail -n1 | cut -d'|' -f2)
    response_body=$(echo "$response" | sed -e '$d') # Remove the last line (metadata)

    if [[ $curl_exit_code -ne 0 ]]; then
        echo -e "${RED}Error:${NC} Failed to fetch content from $url?$query. Curl error code: $curl_exit_code."
        exit 1
    fi

    # Check HTTP status and response
    if [[ "$http_code" =~ ^3[0-9]{2}$ ]]; then
        echo -e "${RED}Error:${NC} Redirect encountered. Final URL: $final_url"
        echo -e "Response: $response_body"
        exit 1
    elif [[ "$http_code" != "200" || -z "$response_body" || "$response_body" =~ "error" ]]; then
        echo -e "${RED}Error:${NC} Invalid response from $url?$query (HTTP $http_code)."
        echo -e "Final URL: $final_url"
        echo -e "Response: $response_body"
        exit 1
    fi

    # Format output
    case "$format" in
        json)
            # Attempt to format as JSON
            echo "$response_body" | jq . 2>/dev/null || {
                echo -e "${RED}Error:${NC} Failed to format as JSON."
                echo "$response_body"
            }
            ;;
        markdown|html|yaml)
            echo "$response_body"
            ;;
        raw|*)
            echo "$response_body"
            ;;
    esac > "${output_file:-/dev/stdout}"
}

# Check for curl dependency
if ! command -v curl >/dev/null 2>&1; then
    echo -e "${RED}Error:${NC} curl is required but not installed."
    exit 1
fi

# Check for jq dependency (for JSON formatting)
if [[ "$format" == "json" ]] && ! command -v jq >/dev/null 2>&1; then
    echo -e "${RED}Error:${NC} jq is required for JSON formatting but not installed."
    exit 1
fi

# Parse command-line arguments
api_key=""
output_file=""
format="raw"
ext=""
dir=""
lines=""
exclude_ext=""
exclude_dir=""
genignore=""
max_size=""
max_tokens=""
yaml_string=""
omit_files=""
omit_tree=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            show_help
            ;;
        -a|--api-key)
            shift
            api_key="$1"
            shift
            ;;
        -o|--output)
            shift
            output_file="$1"
            shift
            ;;
        -f|--format)
            shift
            format="$1"
            if [[ "$format" != "raw" && "$format" != "json" && "$format" != "markdown" && "$format" != "html" && "$format" != "yaml" ]]; then
                echo -e "${RED}Error:${NC} Invalid format. Use 'raw', 'json', 'markdown', 'html', or 'yaml'."
                show_help
                exit 1
            fi
            shift
            ;;
        -e|--ext)
            shift
            ext="$1"
            shift
            ;;
        -d|--dir)
            shift
            dir="$1"
            shift
            ;;
        -l|--lines)
            shift
            lines="$1"
            if [[ "$lines" != "true" && "$lines" != "false" && -n "$lines" ]]; then
                echo -e "${RED}Error:${NC} Invalid lines value. Use 'true' or 'false'."
                show_help
                exit 1
            fi
            shift
            ;;
        -x|--exclude-ext)
            shift
            exclude_ext="$1"
            shift
            ;;
        -X|--exclude-dir)
            shift
            exclude_dir="$1"
            shift
            ;;
        -g|--genignore)
            shift
            genignore="$1"
            if [[ "$genignore" != "true" && "$genignore" != "false" && -n "$genignore" ]]; then
                echo -e "${RED}Error:${NC} Invalid genignore value. Use 'true' or 'false'."
                show_help
                exit 1
            fi
            shift
            ;;
        -s|--max-size)
            shift
            max_size="$1"
            if [[ -n "$max_size" && ! "$max_size" =~ ^[0-9]+$ ]]; then
                echo -e "${RED}Error:${NC} Invalid max-size value. Must be a positive integer."
                show_help
                exit 1
            fi
            shift
            ;;
        -t|--max-tokens)
            shift
            max_tokens="$1"
            if [[ -n "$max_tokens" && ! "$max_tokens" =~ ^[0-9]+$ ]]; then
                echo -e "${RED}Error:${NC} Invalid max-tokens value. Must be a positive integer."
                show_help
                exit 1
            fi
            shift
            ;;
        -y|--yaml)
            shift
            yaml_string="$1"
            shift
            ;;
        -F|--omit-files)
            shift
            omit_files="$1"
            if [[ "$omit_files" != "true" && "$omit_files" != "false" && -n "$omit_files" ]]; then
                echo -e "${RED}Error:${NC} Invalid omit-files value. Use 'true' or 'false'."
                show_help
                exit 1
            fi
            shift
            ;;
        -T|--omit-tree)
            shift
            omit_tree="$1"
            if [[ "$omit_tree" != "true" && "$omit_tree" != "false" && -n "$omit_tree" ]]; then
                echo -e "${RED}Error:${NC} Invalid omit-tree value. Use 'true' or 'false'."
                show_help
                exit 1
            fi
            shift
            ;;
        *)
            repo_input="$1"
            shift
            ;;
    esac
done

# Check for valid input
if [[ -z "$repo_input" ]]; then
    echo -e "${RED}Error:${NC} No repository specified."
    show_help
    exit 1
fi

# Read API key from file if not provided via command line
if [[ -z "$api_key" ]]; then
    api_key=$(read_api_key)
fi

# Append timestamp to output file if specified
if [[ -n "$output_file" ]]; then
    timestamp=$(date +%Y%m%d_%H%M%S)
    output_file="${output_file}_${timestamp}"
fi

# Normalize repository input
repo=$(normalize_repo_input "$repo_input")

# Convert to uithub.com URL
uithub_url=$(convert_to_uithub_url "$repo")

# Fetch and output context
fetch_context "$uithub_url" "$api_key" "$output_file" "$format" "$ext" "$dir" "$lines" "$exclude_ext" "$exclude_dir" "$genignore" "$max_size" "$max_tokens" "$yaml_string" "$omit_files" "$omit_tree"

exit 0