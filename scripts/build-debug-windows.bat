@echo off
setlocal

echo ---- TSRE5 CMake Build (Debug) ----
echo.

set "ROOT_DIR=%~dp0.."
set "BUILD_DIR=%ROOT_DIR%\build\Debug"

:: Check if debug build directory exists
if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory does not exist. Run configure-debug-windows.bat first.
    exit /b 1
)

:: Run CMake build
echo Building TSRE5...

cd ..
cmake --build "%BUILD_DIR%\." --parallel 4

echo.
echo Build complete.
pause