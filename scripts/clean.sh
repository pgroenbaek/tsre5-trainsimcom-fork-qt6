#!/bin/bash
set -e

echo "---- TSRE5 CMake Clean Script ----"
echo

BUILD_DIR="$(dirname "$0")/../build"

if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory using CMake..."

    cmake --build "$BUILD_DIR" --target clean

    echo "Clean complete."
else
    echo "Build directory does not exist. Nothing to clean."
fi