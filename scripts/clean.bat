@echo off
setlocal

echo ---- TSRE5 CMake Clean Script ----
echo.

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%..\build

if exist "%BUILD_DIR%" (
    echo Cleaning build directory using CMake...

    cmake --build "%BUILD_DIR%" --target clean

    echo Clean complete.
) else (
    echo Build directory does not exist. Nothing to clean.
)

pause

