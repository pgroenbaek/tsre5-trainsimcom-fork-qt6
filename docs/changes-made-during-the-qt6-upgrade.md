# Changes made while upgrading to Qt6 and CMake/vcpkg:

## Things added
- A step-by-step build manual.
- A document describing how to set up a folder structure for the source files, if that is wanted at some point.
- Easy-to-use build scripts in the /scripts folder.
- vcpkg.json for the dependency configuration.
- CMakeLists.txt for the build configuration.
<!-- - TODO: Implemented a new DdsLib.cpp that uses the OpenImageIO library to handle DDS textures. QImage has no support for DDS in Qt6, and any DDS textures were therefore rendered as black. -->

## Deprecated / changed Qt API's
- All compiler errors due to the Qt6 upgrade have been fixed.
- All deprecation warnings due to the Qt6 upgrade have been fixed.
- All signal/slot connections have been refactored to use the newer pointer-based syntax that result in compile-time errors. Previously, they were using the old SIGNAL/SLOT macros that only resulted in run-time errors. Some connections were broken after the Qt6 upgrade due to API changes, and the easiest way to ensure all of them was working was to have them throw errors during compilation. All broken connections have been fixed.

## Changes not related to Qt6 specifically
- GLUU.cpp: Rather than hardcoding shader version per platform, TSRE now asks OpenGL what version of shader it supports and will select version 130 or 330 based on that. Some extra info for debugging is printed to log.txt.
- main.cpp: On windows, the terminal popup is now requested to close, but only if TSRE owns it. So, launching TSRE5.exe from e.g. powershell will not close the powershell window, whereas launching TSRE5.exe directly by clicking on the executable or on a shortcut will now close the terminal popup.

## Things removed
- OpenAL headers in the /AL directory (fetching these is now handled by cmake/vcpkg the build folder).
- Auto-generated moc_*.cpp files (now handled by cmake in the build folder).
- Old build scripts and configurations that were used by QMake and the Netbeans IDE.