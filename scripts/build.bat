@echo off
setlocal

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%..\build

if not exist "%BUILD_DIR%" (
    echo Build directory does not exist. Run configure-build.bat first.
    exit /b 1
)

echo Building TSRE5...
cmake --build "%BUILD_DIR%" -- /m

echo Build complete.
pause
