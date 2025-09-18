#!/usr/bin/env python3
import os

# Define extensions and categories
categories = {
    ".hpp": "header files",
    ".cpp": "C++ source files",
    ".shader": "shader files",
    ".sh": "scripts",
    ".py": "scripts"
}

ignore_files = {"CMakeLists.txt"}
ignore_dirs = {"build"}

# Initialize counters
counts = {cat: 0 for cat in categories.values()}

for root, dirs, files in os.walk("."):
    # Skip ignored directories
    dirs[:] = [d for d in dirs if d not in ignore_dirs]

    for f in files:
        if f in ignore_files:
            continue
        _, ext = os.path.splitext(f)
        if ext in categories:
            path = os.path.join(root, f)
            try:
                with open(path, "r", errors="ignore") as fh:
                    line_count = sum(1 for _ in fh)
                    counts[categories[ext]] += line_count
            except Exception as e:
                print(f"Could not read {path}: {e}")

# Print results
sum = 0
for cat, count in counts.items():
    print(f"{count} lines of {cat}")
    sum += count

print(f"-----------------------\n{sum} lines of code total.")

