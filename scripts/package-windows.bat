@echo off
setlocal enabledelayedexpansion

echo ---- TSRE5 Windows Packaging Script ----
echo.

:: Set project directories
set ROOT_DIR=%~dp0..
set BUILD_DIR=%ROOT_DIR%\build
set DIST_DIR=%ROOT_DIR%\dist

if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory does not exist. Run configure-build.bat first.
    exit /b 1
)

if not exist "%BUILD_DIR%\TSRE5.exe" (
    echo [ERROR] TSRE5.exe does not exist. Run build.bat first.
    exit /b 1
)

:: Ask for Qt6 path
set /P QT_PATH="Enter full path to your Qt6 installation directory (e.g., C:\Qt\6.9.3\mingw_64): "

:: Ask for architecture
set /P ARCH="Enter architecture, must match the vcpkg triplet used (x86/x64) [x64]: "
if "%ARCH%"=="" set ARCH=x64

:: Clean previous dist folder
if exist "%DIST_DIR%" rmdir /s /q "%DIST_DIR%"
mkdir "%DIST_DIR%"

:: Copy executable and batch files to dist folder
echo Copying executable and .bat files...
mkdir "%DIST_DIR%"
copy "%BUILD_DIR%\TSRE5.exe" "%DIST_DIR%"
copy "%ROOT_DIR%\ConsistEditor.bat" "%DIST_DIR%"
copy "%ROOT_DIR%\ShapeViewer.bat" "%DIST_DIR%"

:: Copy Qt DLLs using windeployqt
echo Copying Qt dependencies...
set QT_DEPLOY="%QT_PATH%\bin\windeployqt6.exe"
%QT_DEPLOY% "%DIST_DIR%\TSRE5.exe" --release

:: Remove software rendering DLL copied by windeployqt that does not support OpenGL 3.0+
:: The OpenGL pipeline in TSRE5 will break if using this DLL upon a fallback to software-based rendering.
del "%DIST_DIR%\opengl32sw.dll"

:: Copy vcpkg dependencies
echo Copying runtime dependencies from vcpkg...
xcopy "%BUILD_DIR%\vcpkg_installed\%ARCH%-mingw-dynamic\bin\*.dll" "%DIST_DIR%" /Y

:: Download and copy DLLs necessary for software-based rendering with OpenGL 3.0+
:: These DLLs are replacements for opengl32sw.dll from Mesa3D that actually work with the TSRE5 OpenGL pipeline.
set MESA_URL=https://github.com/pal1000/mesa-dist-win/releases/download/25.2.5/mesa3d-25.2.5-release-mingw.7z
set SEVENZIP_URL=https://www.7-zip.org/a/7zr.exe
set TEMP_DIR=%BUILD_DIR%\temp
set MESA_ARCHIVE=%TEMP_DIR%\mesa3d-25.2.5-release-mingw.7z
set SEVENZIP_PATH=%TEMP_DIR%\7zr.exe

mkdir "%TEMP_DIR%"

echo Downloading Mesa3D 25.2.5 MinGW release archive...
curl -L %MESA_URL% -o %MESA_ARCHIVE%

echo Downloading 7-Zip extractor...
curl -L %SEVENZIP_URL% -o %SEVENZIP_PATH%

echo Extracting required Mesa3D DLLs...
"%SEVENZIP_PATH%" x "%MESA_ARCHIVE%" %ARCH%\dxil.dll %ARCH%\opengl32.dll %ARCH%\libgallium_wgl.dll "-o%TEMP_DIR%\extract" -y

echo Copying required Mesa3D DLLs...
xcopy "%TEMP_DIR%\extract\%ARCH%\*.dll" "%DIST_DIR%" /Y

echo Cleaning up temporary files...
rmdir /s /q "%TEMP_DIR%"

:: Copy icons and images
mkdir "%DIST_DIR%\icons"
xcopy "%ROOT_DIR%\resources\icons\*" "%DIST_DIR%\icons" /E /Y
xcopy "%ROOT_DIR%\resources\images\*" "%DIST_DIR%\icons" /E /Y

:: Copy tsre_appdata
mkdir "%DIST_DIR%\tsre_appdata"
xcopy "%ROOT_DIR%\tsre_appdata\*" "%DIST_DIR%\tsre_appdata" /E /Y

:: Copy tsre_assets
mkdir "%DIST_DIR%\tsre_assets"
xcopy "%ROOT_DIR%\tsre_assets\*" "%DIST_DIR%\tsre_assets" /E /Y

echo.
echo Directory created: %DIST_DIR%
echo Done!
pause