@echo off
setlocal

echo ---- TSRE5 CMake Build (Release) ----
echo.

set "ROOT_DIR=%~dp0.."
set "BUILD_DIR=%ROOT_DIR%\build\Release"

:: Check if release build directory exists
if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory does not exist. Run configure-release-windows.bat first.
    exit /b 1
)

:: Run CMake build
echo Building TSRE5...

cd ..
cmake --build "%BUILD_DIR%\." --parallel 4

echo.
echo Build complete.
pause