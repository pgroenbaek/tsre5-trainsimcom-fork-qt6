@echo off
setlocal

echo ---- TSRE5 CMake Configure (Release) ----
echo.

set "ROOT_DIR=%~dp0.."
set "BUILD_DIR=%ROOT_DIR%\build\Release"

:: Remove and re-create existing build directory
if exist "%BUILD_DIR%" (
    echo Removing existing build directory...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"

:: Run CMake configuration
echo.
echo Running CMake...

cd ..
cmake --preset windows-qt-release-userenv

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed.
    exit /b 1
)

echo.
echo Configuration complete.
pause
