#!/bin/bash
set -e

BUILD_DIR="$(dirname "$0")/../build"

if [ -d "$BUILD_DIR" ]; then
    echo "Removing build directory..."
    rm -rf "$BUILD_DIR"
    echo "Clean complete."
else
    echo "Build directory does not exist. Nothing to clean."
fi
