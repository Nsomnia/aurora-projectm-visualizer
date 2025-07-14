#!/bin/bash

# Create a list of all files, including hidden files, recursively
find . -type f | tee -a project-tree.md

echo "File list has been saved to project-tree.md"
