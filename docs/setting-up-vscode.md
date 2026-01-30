# Setting up VS Code

Here's how to set up integrations for GDB, CMake and Qt6 in Visual Studio Code.

## Install these extensions:

**Core**
- CMake Tools (ms-vscode.cmake-tools)
- C/C++ (ms-vscode.cpptools)

**Debugging (optional but convenient)**
- C/C++ Extension Pack (ms-vscode.cpptools-extension-pack)

No separate GDB extension needed - cpptools handles it.

**Qt (optional but nice)**
- Qt Tools (qttools.qttools)

Helps with .ui, .qrc, designer integration.

Not required for build/debug/packaging.

## Configure environment variables

Copy `.env.template` to a file named `.env` in the project root directory.

Fill out the paths to your Qt6 and vcpkg install locations in the `.env` file. 