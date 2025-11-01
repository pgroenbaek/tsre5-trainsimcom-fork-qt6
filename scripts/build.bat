@echo off
setlocal

echo ---- TSRE5 CMake Build Script ----
echo.

:: Default values
set "DEFAULT_QT_PATH=C:\Qt6.9\6.9.3\mingw_64"

:: Ask for Qt6 path
set /P QT_PATH="Enter full path to your Qt6 installation directory (e.g., C:\path\to\Qt\6.x.x\mingw_64) [%DEFAULT_QT_PATH%]: "
if "%QT_PATH%"=="" set "QT_PATH=%DEFAULT_QT_PATH%"

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

:: Prepend the MinGW compiler and CMake from the Qt installation to path
set "PATH=%QT_PATH%\..\..\Tools\mingw1310_64\bin;%PATH%"
set "PATH=%QT_PATH%\..\..\Tools\CMake_64\bin;%PATH%"

:: Build directory relative to script
set "SCRIPT_DIR=%~dp0"
set "BUILD_DIR=%SCRIPT_DIR%..\build"

if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory does not exist. Run configure-build.bat first.
    exit /b 1
)

echo Building TSRE5...
cmake --build "%BUILD_DIR%" --parallel 4

echo Build complete.
pause
