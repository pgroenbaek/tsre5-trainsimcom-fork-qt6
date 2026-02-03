#!/bin/bash
set -e
set -u

echo "---- TSRE5 CMake Clean (Release) ----"
echo

ROOT_DIR="$(dirname "$(realpath "$0")")/.."
BUILD_DIR="$ROOT_DIR/build/Release"
DIST_DIR="$ROOT_DIR/dist/Release"

# Check if release build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "[ERROR] Build directory does not exist. Run configure-release-linux.sh first."
    exit 1
fi

# Clean build folder
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory using CMake..."
    cmake --build "$BUILD_DIR" --target clean
else
    echo "Build directory does not exist. Nothing to clean."
fi

# Clean dist folder
echo "Cleaning dist directory..."
if [ -d "$DIST_DIR" ]; then
    rm -rf "$DIST_DIR"
fi
mkdir -p "$DIST_DIR"

echo
echo "Clean complete."
