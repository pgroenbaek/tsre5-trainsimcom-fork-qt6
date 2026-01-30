#!/bin/bash
set -e

echo "---- TSRE5 CMake Build Script ----"
echo

BUILD_DIR="$(dirname "$0")/../build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory does not exist. Run ./configure-build.sh first."
    exit 1
fi

echo "Building TSRE5..."
cmake --build "$BUILD_DIR" --parallel 4

echo "Build complete."
