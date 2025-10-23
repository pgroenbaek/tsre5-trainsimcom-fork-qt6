@echo off
setlocal enabledelayedexpansion

set /P QT_PATH="Enter full path to your Qt6 installation directory (e.g., C:\Qt\6.9.3\mingw_64): "

:: Ask for build type
set /P BUILD_TYPE="Enter build type (Testing/Release) [Testing]: "
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Testing

:: Ask for version
set /P VERSION="Enter version (e.g. v8.006a) [v8.006a]: "
if "%VERSION%"=="" set VERSION=v8.006a

:: Ask for architecture
set /P ARCH="Enter architecture, must match the vcpkg triplet used (x86/x64) [x64]: "
if "%ARCH%"=="" set ARCH=x64

:: Set project directories
set ROOT_DIR=%~dp0\..
set BUILD_DIR=%ROOT_DIR%\build
set DIST_DIR=%ROOT_DIR%\dist
set ZIP_NAME="TSRE-%BUILD_TYPE%-%VERSION%-windows-%ARCH%.zip"

:: Clean previous dist folder
if exist "%DIST_DIR%" rmdir /s /q "%DIST_DIR%"
mkdir "%DIST_DIR%"

:: Copy executable to dist folder
echo Copying executable...
mkdir "%DIST_DIR%\bin"
copy "%BUILD_DIR%\%BUILD_TYPE%\TSRE5.exe" "%DIST_DIR%"

:: Deploy Qt DLLs using windeployqt
echo Deploying Qt dependencies...
set QT_DEPLOY="%QT_PATH%\bin\windeployqt.exe"
%QT_DEPLOY% "%DIST_DIR%\TSRE5.exe" --release

:: Copy vcpkg dependencies (OpenAL/OpenGL)
:: TODO
::echo Copying vcpkg runtime dependencies...
::xcopy "%VCPKG_ROOT%\installed\%ARCH%\bin\*.dll" "%DIST_DIR%\bin\" /Y

:: Copy other resources (configs, assets, etc.)
:: TODO
::xcopy "%ROOT_DIR%\resources" "%DIST_DIR%\resources" /E /Y

:: Create ZIP archive
echo Creating ZIP archive...
powershell -Command "Compress-Archive -Path '%DIST_DIR%\*' -DestinationPath '%ROOT_DIR%\%ZIP_NAME%' -Force"

echo.
echo Package created: %ZIP_NAME%
echo Done!
pause