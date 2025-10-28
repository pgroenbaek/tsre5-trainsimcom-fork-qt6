#!/bin/bash
set -e

echo "---- TSRE5 CMake Build Configuration Script ----"
echo

read -p "Enter full path to your Qt6 installation directory (e.g., ~/Programs/Qt/6.9.3/gcc_64): " QT_PATH

# Check that QT_PATH is not empty
if [[ -z "$QT_PATH" ]]; then
    echo "Error: Qt installation path cannot be empty."
    exit 1
fi

# Check that QT_PATH exists
if [[ ! -d "$QT_PATH" ]]; then
    echo "Error: '$QT_PATH' is not a valid directory."
    exit 1
fi

read -p "Enter full path to your vcpkg directory (e.g., ~/.vcpkg) [~/.vcpkg]: " VCPKG_PATH
VCPKG_PATH=${VCPKG_PATH:-~/.vcpkg}

# Check that VCPKG_PATH exists
if [[ ! -d "$VCPKG_PATH" ]]; then
    echo "Error: '$VCPKG_PATH' is not a valid directory."
    exit 1
fi

read -p "Enter vcpkg triplet (x64-linux-dynamic/x64-osx-dynamic/arm64-osx-dynamic) [x64-linux-dynamic]: " TRIPLET
TRIPLET=${TRIPLET:-x64-linux-dynamic}

# Build directory relative to project root
BUILD_DIR="$(dirname "$0")/../build"

# Remove existing build dir
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

echo
echo "Running CMake..."
cmake -B "$BUILD_DIR" -S "$(dirname "$0")/.." \
  -G"Unix Makefiles" \
  -DCMAKE_PREFIX_PATH="$QT_PATH/lib/cmake" \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_PATH/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_TARGET_TRIPLET="$TRIPLET"

echo
echo "CMake configuration complete."
echo "To build the project, run: ./build.sh"
