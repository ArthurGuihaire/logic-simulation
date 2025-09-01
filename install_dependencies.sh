#!/usr/bin/env bash

set -e

# Function to detect package manager
detect_package_manager() {
    if command -v apt >/dev/null; then
        echo "apt"
    elif command -v pacman >/dev/null; then
        echo "pacman"
    elif command -v dnf >/dev/null; then
        echo "dnf"
    else
        echo "unsupported"
    fi
}

# Function to install dependencies
install_dependencies() {
    local pm=$1

    case "$pm" in
        apt)
            sudo apt update
            sudo apt install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev \
                                libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxi-dev \
                                libglfw3-dev libglm-dev
            ;;
        pacman)
            sudo pacman -Syu --noconfirm base-devel cmake mesa glu glfw-x11 glm
            ;;
        dnf)
            sudo dnf install -y gcc gcc-c++ make cmake mesa-libGL-devel mesa-libGLU-devel \
                                libX11-devel libXext-devel libXrandr-devel libXinerama-devel \
                                libXi-devel glfw-devel glm-devel
            ;;
        *)
            echo "Unsupported Linux distribution. Please install dependencies manually."
            exit 1
            ;;
    esac
}

# Main script
echo "Detecting package manager..."
PM=$(detect_package_manager)
echo "Package manager detected: $PM"

echo "Installing dependencies..."
install_dependencies "$PM"

echo "Dependencies installed successfully!"
echo "You can now run:"
echo "  mkdir -p build && cd build"
echo "  cmake .."
echo "  make"
