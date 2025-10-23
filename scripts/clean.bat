@echo off
setlocal

echo ---- TSRE5 Clean Build Directory Script ----
echo.

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%..\build

if exist "%BUILD_DIR%" (
    echo Removing build directory...
    rmdir /s /q "%BUILD_DIR%"
    echo Clean complete.
) else (
    echo Build directory does not exist. Nothing to clean.
)
pause
