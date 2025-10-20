@echo off
setlocal enabledelayedexpansion

echo ---- TSRE5 Build Configuration Script ----
echo.

set /p VCPKG_PATH="Enter full path to your vcpkg directory (e.g., C:\vcpkg) [C:\vcpkg]: "
set /p TRIPLET="Enter vcpkg triplet (x64-mingw-dynamic/x86-mingw-dynamic) [x64-mingw-dynamic]: "
if "!VCPKG_PATH!"=="" set VCPKG_PATH="C:\vcpkg"
if "!TRIPLET!"=="" set TRIPLET="x64-mingw-dynamic"

:: Build directory relative to script
set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%..\build

:: Remove existing build dir
if exist "%BUILD_DIR%" (
    echo Removing existing build directory...
    rmdir /s /q "%BUILD_DIR%"
)

mkdir "%BUILD_DIR%"

echo.
echo Running CMake...
cmake -B "%BUILD_DIR%" -S "%SCRIPT_DIR%..\." ^
  -DCMAKE_TOOLCHAIN_FILE=%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=%TRIPLET%

echo.
echo CMake configuration complete.
echo To build the project, run: build.bat
pause
