@echo off
setlocal enabledelayedexpansion

:: Note: There is no need to run this script manually.
::       CMake is set up to run this script automatically upon build completion.
::       It will run no matter if you build via the VSCode CMake extension or via build-debug-windows.bat.

echo ---- TSRE5 Windows Post-Build (Release) ----
echo.

:: Set root directory
set "ROOT_DIR=%~dp0.."

:: Validate required variables
for %%V in (QT_ROOT QT_VERSION QT_ARCH VCPKG_ROOT VCPKG_TRIPLET) do (
    if not defined %%V (
        echo [ERROR] Environment variable %%V is not defined.
        exit /b 1
    )
)

set "QT_PATH=%QT_ROOT%\%QT_VERSION%\%QT_ARCH%"

:: Set project directories
set "BUILD_DIR=%ROOT_DIR%\build\Debug"
set "DIST_DIR=%ROOT_DIR%\dist\Debug"

:: Copy batch files to dist folder
echo Copying executable and .bat files...
xcopy "%BUILD_DIR%\TSRE5.exe" "%DIST_DIR%" /Y
xcopy "%ROOT_DIR%\ConsistEditor.bat" "%DIST_DIR%" /Y
xcopy "%ROOT_DIR%\ShapeViewer.bat" "%DIST_DIR%" /Y

:: Copy Qt DLLs using windeployqt
echo Copying Qt dependencies...
set "WINDEPLOYQT=%QT_PATH%\bin\windeployqt6.exe"

if not exist "%WINDEPLOYQT%" (
    echo [ERROR] windeployqt6.exe not found at %WINDEPLOYQT%
    exit /b 1
)

"%WINDEPLOYQT%" "%DIST_DIR%\TSRE5.exe" --release

:: Copy vcpkg dependencies
echo Copying runtime dependencies from vcpkg...
xcopy "%BUILD_DIR%\vcpkg_installed\%VCPKG_TRIPLET%\bin\*.dll" "%DIST_DIR%" /Y

:: Remove software rendering DLL copied by windeployqt that does not support OpenGL 3.0+
:: The OpenGL pipeline in TSRE5 will break if using this DLL upon a fallback to software-based rendering.
echo Removing opengl32sw.dll...
del "%DIST_DIR%\opengl32sw.dll"

:: Copy icons and images
echo Copying icons and images...
mkdir "%DIST_DIR%\icons"
xcopy "%ROOT_DIR%\resources\icons\*" "%DIST_DIR%\icons" /E /Y
xcopy "%ROOT_DIR%\resources\images\*" "%DIST_DIR%\icons" /E /Y

:: Copy tsre_appdata
echo Copying tsre_appdata...
mkdir "%DIST_DIR%\tsre_appdata"
xcopy "%ROOT_DIR%\tsre_appdata\*" "%DIST_DIR%\tsre_appdata" /E /Y

:: Copy tsre_assets
echo Copying tsre_assets...
mkdir "%DIST_DIR%\tsre_assets"
xcopy "%ROOT_DIR%\tsre_assets\*" "%DIST_DIR%\tsre_assets" /E /Y

echo.
echo Directory created: %DIST_DIR%
echo Done!
pause