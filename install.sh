#!/usr/bin/env bash
set -e

# Paths
PROJECT_ROOT="$(pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
INSTALL_SCRIPT="$PROJECT_ROOT/install_dependencies.sh"

# Function to build the project
build_project() {
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    echo "Running cmake..."
    if ! cmake ..; then
        return 1
    fi

    echo "Running make..."
    if ! make -j"$(nproc)"; then
        return 1
    fi

    return 0
}

# First attempt
echo "Attempting to build the project..."
if build_project; then
    echo "Build succeeded!"
    exit 0
fi

# If build fails, install dependencies
echo "Initial build failed. Attempting to install dependencies..."
if [ -x "$INSTALL_SCRIPT" ]; then
    "$INSTALL_SCRIPT"
else
    echo "Error: Install script not found or not executable at $INSTALL_SCRIPT"
    exit 1
fi

# Retry build
echo "Retrying build after installing dependencies..."
if build_project; then
    echo "Build succeeded after installing dependencies!"
else
    echo "Build still failed. Please check the error messages above."
    exit 1
fi
