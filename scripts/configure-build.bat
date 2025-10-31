@echo off
setlocal enabledelayedexpansion

echo ---- TSRE5 CMake Build Configuration Script ----
echo.

set /p QT_PATH="Enter full path to your Qt6 installation directory (e.g., C:\Qt\6.9.3\mingw_64): "

:: Enforce that QT_PATH is not empty
if "%QT_PATH%"=="" (
    echo [ERROR] Qt installation path cannot be empty.
    exit /b 1
)

:: Check if QT_PATH directory exists
if not exist "%QT_PATH%" (
    echo [ERROR] "%QT_PATH%" does not exist or is not a directory.
    exit /b 1
)

set /p VCPKG_PATH="Enter full path to your vcpkg directory (e.g., C:\vcpkg) [C:\vcpkg]: "
if "!VCPKG_PATH!"=="" set "VCPKG_PATH=C:\vcpkg"

:: Check if VCPKG_PATH directory exists
if not exist "%VCPKG_PATH%" (
    echo [ERROR] "%VCPKG_PATH%" does not exist or is not a directory.
    exit /b 1
)

set /p TRIPLET="Enter vcpkg triplet (x86-mingw-dynamic/x64-mingw-dynamic) [x64-mingw-dynamic]: "
if "!TRIPLET!"=="" set "TRIPLET=x64-mingw-dynamic"

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
  -G "MinGW Makefiles" ^
  -DQT_PATH="%QT_PATH%" ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake" ^
  -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="%SCRIPT_DIR%..\toolchains\qt-mingw-toolchain.cmake" ^
  -DVCPKG_TARGET_TRIPLET="%TRIPLET%" ^
  -DVCPKG_HOST_TRIPLET="%TRIPLET%" ^
  -DVCPKG_FEATURE_FLAGS="manifests"

echo.
echo CMake configuration complete.
echo To build the project, run: build.bat
pause
