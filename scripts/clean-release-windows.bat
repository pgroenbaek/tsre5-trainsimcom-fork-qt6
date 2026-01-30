@echo off
setlocal

echo ---- TSRE5 CMake Clean (Release) ----
echo.

set "ROOT_DIR=%~dp0.."
set "BUILD_DIR=%ROOT_DIR%\build\Release"
set "DIST_DIR=%ROOT_DIR%\dist\Release"

:: Check if release build directory exists
if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory does not exist. Run configure-release-windows.bat first.
    exit /b 1
)

:: Clean build folder
if exist "%BUILD_DIR%" (
    echo Cleaning build directory using CMake...

    cmake --build "%BUILD_DIR%\." --target clean
) else (
    echo Build directory does not exist. Nothing to clean.
)

:: Clean dist folder
echo Cleaning dist directory...
if exist "%DIST_DIR%" rmdir /s /q "%DIST_DIR%"
mkdir "%DIST_DIR%"

echo.
echo Clean complete.

