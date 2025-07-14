#!/bin/bash

# Loop through all files in the current directory
for file in *; do
  # Check if the file exists
  if [ -f "$file" ]; then
    # Remove spaces from the filename
    new_filename=$(echo "$file" | tr ' ' '_')
    
    # Check if the new filename is different from the original
    if [ "$new_filename" != "$file" ]; then
      # Rename the file
      mv "$file" "$new_filename"
      echo "Renamed: $file -> $new_filename"
    fi
  fi
done

echo "All files renamed without spaces."
