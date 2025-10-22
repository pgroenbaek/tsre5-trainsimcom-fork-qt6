@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

SET /P QT_PATH="Enter full path to your Qt6 installation directory (e.g., C:\Qt\6.9.3\mingw_64): "

:: Ask for build type
SET /P BUILD_TYPE="Enter build type (Testing/Release) [Testing]: "
IF "%BUILD_TYPE%"=="" SET BUILD_TYPE=Testing

:: Ask for version
SET /P VERSION="Enter version (e.g. v8.006a) [v8.006a]: "
IF "%VERSION%"=="" SET VERSION=v8.006a

:: Ask for architecture
SET /P ARCH="Enter architecture (x86/x64) [x64]: "
IF "%ARCH%"=="" SET ARCH=x64

:: Set project directories
SET ROOT_DIR=%~dp0\..
SET BUILD_DIR=%ROOT_DIR%\build
SET PACKAGE_DIR=%ROOT_DIR%\package
SET ZIP_NAME="TSRE-%BUILD_TYPE%-%VERSION%-windows-%ARCH%.zip"

:: Clean previous build/package
IF EXIST "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
IF EXIST "%PACKAGE_DIR%" rmdir /s /q "%PACKAGE_DIR%"
mkdir "%BUILD_DIR%"
mkdir "%PACKAGE_DIR%"

:: Configure CMake with vcpkg toolchain
echo Configuring CMake...
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -A %ARCH% -DCMAKE_PREFIX_PATH="C:\Qt\5.7\msvc2015_%ARCH%\lib\cmake" -G "NMake Makefiles"

IF %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    exit /b 1
)

:: Build project
echo Building project...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b 1
)

:: Copy executable to package folder
echo Copying executable...
mkdir "%PACKAGE_DIR%\bin"
copy "%BUILD_DIR%\%BUILD_TYPE%\TSRE5.exe" "%PACKAGE_DIR%\bin\"

:: Deploy Qt DLLs using windeployqt
echo Deploying Qt dependencies...
SET QT_BIN="C:\Qt\5.7\msvc2015_%ARCH%\bin\windeployqt.exe"
%QT_BIN% "%PACKAGE_DIR%\bin\TSRE5.exe" --release

:: Copy vcpkg dependencies (OpenAL/OpenGL)
echo Copying vcpkg runtime dependencies...
xcopy "%VCPKG_ROOT%\installed\%ARCH%\bin\*.dll" "%PACKAGE_DIR%\bin\" /Y

:: Optional: copy other resources (configs, assets, etc.)
xcopy "%ROOT_DIR%\resources" "%PACKAGE_DIR%\resources" /E /Y

:: Create ZIP package
echo Creating ZIP archive...
powershell -Command "Compress-Archive -Path '%PACKAGE_DIR%\*' -DestinationPath '%ROOT_DIR%\%ZIP_NAME%' -Force"

echo.
echo Package created: %ZIP_NAME%
echo Done!
pause