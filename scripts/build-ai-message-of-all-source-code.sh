#!/bin/bash

# Set output file
OUTPUT_FILE="ai_prompt_input.txt"

# Print all files in the current working directory
echo "📁 Listing all files in the current directory (recursively):"
find . -type f ! -name "$OUTPUT_FILE"

# Prompt the user for their message to the AI
echo ""
echo "💬 Enter your message for the AI (end input with Ctrl+D on a new line):"
USER_MESSAGE=$(</dev/stdin)

# Start writing to output file
echo "📝 Preparing formatted output in '$OUTPUT_FILE'..."

{
  echo "===== PROJECT SOURCE CODE START ====="
  echo ""

  # For each file, add formatted source code
  find . -type f ! -name "$OUTPUT_FILE" | while read -r FILE; do
    # Skip binary files or large files (>500KB)
    if file --mime "$FILE" | grep -q text && [ "$(stat -c%s "$FILE")" -le 512000 ]; then
      echo "----- FILE: $FILE -----"
      echo '```'
      cat "$FILE"
      echo '```'
      echo ""
    fi
  done

  echo "===== PROJECT SOURCE CODE END ====="
  echo ""
  echo "===== USER INSTRUCTION START ====="
  echo ""
  echo "$USER_MESSAGE"
  echo ""
  echo "===== USER INSTRUCTION END ====="
} > "$OUTPUT_FILE"

echo "✅ Done! Contents saved in '$OUTPUT_FILE'. You can now paste this into an AI chat."
