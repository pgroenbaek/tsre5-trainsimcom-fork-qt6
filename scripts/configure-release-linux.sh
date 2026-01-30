#!/bin/bash
set -e
set -u

echo "---- TSRE5 CMake Configure (Release) ----"
echo

ROOT_DIR="$(dirname "$(realpath "$0")")/.."
BUILD_DIR="$ROOT_DIR/build/Release"

# Remove and re-create existing build directory
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR"

# Run CMake configuration
echo
echo "Running CMake..."
cd "$ROOT_DIR"
if ! cmake --preset linux-qt-release-userenv; then
    echo
    echo "[ERROR] CMake configuration failed."
    exit 1
fi

echo
echo "Configuration complete."
