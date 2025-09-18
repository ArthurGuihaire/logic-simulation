#!/bin/bash

# Get the script's own filename
SELF_SCRIPT="$(basename "$0")"
PROTECTED_SCRIPT_1="build_and_run.sh"
PROTECTED_SCRIPT_2="clean_build_and_run.sh"
PROTECTED_SCRIPT_3="debug_build.sh"
PROTECTED_SCRIPT_4="compile_commands.json" # For vscode

# Loop through all items (including hidden ones) in the current directory
for item in * .*; do
  # Skip current and parent directory entries
  [[ "$item" == "." || "$item" == ".." ]] && continue

  # Skip the protected scripts
  [[ "$item" == "$SELF_SCRIPT" || "$item" == "$PROTECTED_SCRIPT_1" || "$item" == "$PROTECTED_SCRIPT_2" || "$item" == "$PROTECTED_SCRIPT_3" || "$item" == "$PROTECTED_SCRIPT_4" ]] && continue

  # Delete the item
  rm -rf -- "$item"
done
