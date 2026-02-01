# Setting up VS Code

Visual Studio Code can be set up with IDE-like functionality to get proper debugging capabilities, etc. This will also allow you to build TSRE5 without using the scripts in the `\scripts` folder.

## Required extensions:

The following extension is required:
- CMake Tools (ms-vscode.cmake-tools)

You can search for the extension in the extensions tab using the extension identifier to be sure to get the right one.

![VSCode extension search](./docs/images/vscode-extension-search.png)

## Optional extensions:

- C/C++ (ms-vscode.cpptools)
- Qt Core (theqtcompany.qt-core)
- Qt C++ (theqtcompany.qt-cpp)
- Qt UI (theqtcompany.qt-u)

Helps with debugging, .ui, .qrc, designer integration.

Not required for build/debug/packaging.

# Setup required for building TSRE5

To build TSRE5, you will also need to install Qt6 and vcpkg. And set up the paths to the install-locations in a `CMakeUserPresets.json` file, as described in the [build manual](./docs/cmake-build-manual.md).