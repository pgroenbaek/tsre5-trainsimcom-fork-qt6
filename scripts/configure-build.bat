@echo off
setlocal enabledelayedexpansion

echo ---- TSRE5 Build Configuration Script ----
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
if "!VCPKG_PATH!"=="" set VCPKG_PATH="C:\vcpkg"

:: Check if VCPKG_PATH directory exists
if not exist "%VCPKG_PATH%" (
    echo [ERROR] "%VCPKG_PATH%" does not exist or is not a directory.
    exit /b 1
)

set /p TRIPLET="Enter vcpkg triplet (x64-mingw-static/x86-mingw-static) [x64-mingw-static]: "
if "!TRIPLET!"=="" set TRIPLET="x64-mingw-static"

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
  -DCMAKE_C_COMPILER="%QT_PATH%\..\..\Tools\mingw1310_64\bin\gcc.exe" ^
  -DCMAKE_CXX_COMPILER="%QT_PATH%\..\..\Tools\mingw1310_64\bin\g++.exe" ^
  -DCMAKE_MAKE_PROGRAM="%QT_PATH%\..\..\Tools\mingw1310_64\bin\mingw32-make.exe" ^
  -DCMAKE_PREFIX_PATH="%QT_PATH%\lib\cmake" ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake" ^
  -DVCPKG_TARGET_TRIPLET="%TRIPLET%" ^
  -DVCPKG_HOST_TRIPLET="%TRIPLET%"

echo.
echo CMake configuration complete.
echo To build the project, run: build.bat
pause
