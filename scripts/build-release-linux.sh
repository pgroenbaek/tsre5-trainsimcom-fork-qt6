#!/bin/bash
set -e
set -u

echo "---- TSRE5 CMake Build (Release) ----"
echo

ROOT_DIR="$(dirname "$(realpath "$0")")/.."
BUILD_DIR="$ROOT_DIR/build/Release"

# Check if release build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "[ERROR] Build directory does not exist. Run configure-release-linux.sh first."
    exit 1
fi

# Run CMake build
echo "Building TSRE5..."
cmake --build "$BUILD_DIR" --parallel 4

echo
echo "Build complete."
