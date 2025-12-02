#!/bin/bash
set -e

echo "---- TSRE5 CMake Build Configuration Script ----"
echo

# Default values
DEFAULT_QT_PATH="$HOME/Programs/Qt/6.9.3/gcc_64"
DEFAULT_VCPKG_PATH="$HOME/.vcpkg"
DEFAULT_TRIPLET="x64-linux-dynamic"

read -p "Enter full path to your Qt6 installation directory (e.g., /path/to/Qt/6.9.3/gcc_64) [$DEFAULT_QT_PATH]: " QT_PATH
QT_PATH=${QT_PATH:-$DEFAULT_QT_PATH}

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

read -p "Enter full path to your vcpkg directory (e.g., ~/.vcpkg) [$DEFAULT_VCPKG_PATH]: " VCPKG_PATH
VCPKG_PATH=${VCPKG_PATH:-$DEFAULT_VCPKG_PATH}

# Check that VCPKG_PATH exists
if [[ ! -d "$VCPKG_PATH" ]]; then
    echo "Error: '$VCPKG_PATH' is not a valid directory."
    exit 1
fi

read -p "Enter vcpkg triplet (x64-linux-dynamic/x64-osx-dynamic/arm64-osx-dynamic) [$DEFAULT_TRIPLET]: " TRIPLET
TRIPLET=${TRIPLET:-$DEFAULT_TRIPLET}

# Set Qt path so that it is visible to CMake
export QT_PATH

# Build directory relative to script
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"

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
  -DQT_PATH="$QT_PATH" \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_PATH/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="$SCRIPT_DIR/../toolchains/qt-linux-toolchain.cmake" \
  -DVCPKG_TARGET_TRIPLET="$TRIPLET" \
  -DVCPKG_FEATURE_FLAGS="manifests"

echo
echo "CMake configuration complete."
echo "To build the project, run: ./build.sh"
