#!/bin/bash
set -e

BUILD_DIR="$(dirname "$0")/../build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory does not exist. Run ./configure-build.sh first."
    exit 1
fi

echo "Building TSRE5..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

echo "Build complete."
