#!/bin/bash
set -e

echo "---- TSRE5 Clean Build Directory Script ----"
echo

BUILD_DIR="$(dirname "$0")/../build"

if [ -d "$BUILD_DIR" ]; then
    echo "Removing build directory..."
    rm -rf "$BUILD_DIR"
    echo "Clean complete."
else
    echo "Build directory does not exist. Nothing to clean."
fi
