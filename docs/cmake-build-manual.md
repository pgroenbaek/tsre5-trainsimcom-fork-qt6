# CMake build manual

**Note:** This manual is work-in-progress and is updated as I go along.

## Prerequisites

| Tool             | Why it's needed                                                          |
| ---------------- | ------------------------------------------------------------------------ |
| **Git**          | To clone the TSRE5 repo *and* vcpkg                                      |
| **C++ compiler** | To compile the project and dependencies                                  |
| **CMake**        | To configure and generate build files                                    |
| **vcpkg**        | To manage and integrate libraries other than Qt itself                   |
| **Qt 6.x**       | To provide the GUI framework and required modules for the application    |

The `vcpkg` tool by Microsoft is a dependency manager that handles downloading, building, and integrating libraries into the project automatically. It is similar to what NuGet does for C#, just for C/C++ instead.

For example, using `vcpkg` you don't need to find openal-soft manually. Qt needs dynamic linking due to the Open Source license, so therefore Qt is not handled through `vcpkg` like the other dependencies. It is also easier to download and use the prebuilt Qt libraries. In addition, it takes a long time to fetch and compile Qt through `vcpkg` and it is more error-prone.

## Windows

### Installing git:

Download from:
https://git-scm.com/downloads/win

Install it.

Open a new powershell window, and verify you can run:

```powershell
git --version
```

If you can't run it, you will need to add the install location to the system env path variable manually.

### Installing CMake:

Download from:
https://cmake.org/download/

Install it.

Open a new powershell window, and verify you can run:

```powershell
cmake --version
```

If you can't run it, you will need to add the install location to the system env path variable manually.

### Installing MinGW:

MinGW 13.1 is listed in the Qt documentation to be compatible with Qt version 6.9.x.

That version of MinGW can be downloaded from here:
https://github.com/niXman/mingw-builds-binaries/releases/tag/13.1.0-rt_v11-rev1

Extract the zip file e.g. into C:/

Add the bin folder to the system path env variable, e.g. "C:\mingw64\bin"

Open a new powershell window, and verify you can run:

```powershell
gcc --version
```

If you can't run it, you will need to adjust the system env path variable.

### Installing and setting up vcpkg:

Open powershell and run:

```powershell
cd C:/
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
```

Run the bootstrap script:

```powershell
.\bootstrap-vcpkg.bat -disableMetrics
```

Add the vcpkg folder to the system path env variable, e.g. "C:\vcpkg"

Restart powershell, and verify you can run:

```powershell
vcpkg --version
```

If you can't run it, you will need to adjust the system env path variable.

Run this to integrate vcpkg with CMake:
```powershell
vcpkg integrate install
```

### Installing Qt6.x:

Download the installer:
https://www.qt.io/download-qt-installer-oss

Select the right OS and install somewhere you can find it.

**Important:** You must select "Customize install", then find and select _"Qt WebSockets"_ under one of the treeview menus on the customization page. Otherwise CMake will not have that package available and TSRE5 cannot be built without it. _"Qt WebSockets"_ is not included in the standard install configuration.

    Check it here in the treeview menu: Qt -> Qt 6.x.x -> Additional Libraries -> Qt WebSockets


Find the path to the lib\cmake folder, you will need it later.

It should look something like "C:\path\to\Qt\6.x.x\mingw_64\lib\cmake", or similar, depending on version and where you installed it.


### Building TSRE5

Clone and change directory to the TSRE5 repository:
```
git clone <repo url>
cd <repo dir name>
```

Create and enter the build directory:
```powershell
mkdir build
cd build
```

Now create the build configuration:

The `../` refers to the project root directory where all the source files are. We execute the `cmake` command from inside the build folder to not make a mess of the source file folder with all sorts of build artefacts later.

```powershell
cmake -DVCPKG_TARGET_TRIPLET="x64-mingw-static" -DVCPKG_HOST_TRIPLET="x64-mingw-static" -G"MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_PREFIX_PATH="C:/path/to/Qt/6.x.x/mingw_64/lib/cmake" -DCMAKE_C_COMPILER="C:\mingw64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\mingw64\bin\g++.exe" -DCMAKE_MAKE_PROGRAM="C:\mingw64\bin\mingw32-make.exe" -S ../ -B .
```

Now these commands can be used in the build directory:

```bash
mingw32-make
```

```bash
mingw32-make clean
```

## Linux (Debian-based distros)

### Installing git:

```bash
sudo apt install git
```

Verify that you can run `git`:
```bash
git --version
```

### Installing gcc:

```bash
sudo apt install build-essential ninja-build
```

Verify that you can run `gcc`:
```bash
gcc --version
```

### Installing CMake:

```bash
sudo apt install cmake
```

Verify that you can run `cmake`:
```bash
cmake --version
```

### Installing and setting up vcpkg:

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git ~/.vcpkg
cd ~/.vcpkg
./bootstrap-vcpkg.sh -disableMetrics
```

Edit your `~/.bashrc` (or similar depending on the shell you use) and add this line at the bottom:
```bash
export PATH="$HOME/.vcpkg:$PATH"
```

To reload your shell
```bash
source ~/.bashrc
```

Verify that you can run `vcpkg`:
```bash
vcpkg --version
```

To integrate `vcpkg` with `cmake`:
```bash
vcpkg integrate install
```

### Installing Qt6.x:

Download the installer:
https://www.qt.io/download-qt-installer-oss

Select the right OS and install somewhere you can find it.

**Important:** You must select "Customize install", then find and select _"Qt WebSockets"_ under one of the treeview menus on the customization page. Otherwise CMake will not have that package available and TSRE5 cannot be built without it. _"Qt WebSockets"_ is not included in the standard install configuration.

    Check it here in the treeview menu: Qt -> Qt 6.x.x -> Additional Libraries -> Qt WebSockets


Find the path to the lib/cmake folder, you will need it later.

It should look something like "/path/to/Qt/6.x.x/gcc_64/lib/cmake", or similar, depending on version and where you installed it.

### Building TSRE5

Clone and change directory to the TSRE5 repository:
```
git clone <repo url>
cd <repo dir name>
```

Create and enter the build directory.

```bash
mkdir build
cd build
```

Now create the build configuration:

The `../` refers to the project root directory where all the source files are. We execute the `cmake` command from inside the build folder to not make a mess of the source file folder with all sorts of build artefacts later.

```bash
cmake -DVCPKG_TARGET_TRIPLET="x64-linux" -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="~/.vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/gcc_64/lib/cmake" -S ../ -B .
```

Now these commands can be used in the build directory:

```bash
make
```

```bash
make clean
```

## macOS

I don't have a Mac, so I can't test and document this process.

It's a somewhat similar process to Linux though. Use 'brew' rather than 'apt', and OSX-specific target triplets when using CMake and vcpkg.

Those target triplets are:
- x64-osx-static
- arm64-osx-static