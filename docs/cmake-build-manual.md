# CMake build manual

## Prerequisites

| Tool             | Why it's needed                                                          |
| ---------------- | ------------------------------------------------------------------------ |
| **Git**          | To clone the TSRE5 repo *and* vcpkg                                      |
| **Qt 6.9.x**     | To provide the GUI framework and required modules for the application    |
| **C++ compiler** | To compile the project and dependencies                                  |
| **CMake**        | To configure and generate build files                                    |
| **vcpkg**        | To manage and integrate libraries other than Qt itself                   |

The `vcpkg` tool by Microsoft is a dependency manager that handles downloading, building, and integrating libraries into the project automatically. It is similar to what NuGet does for C#, just for C/C++ instead.

For example, using `vcpkg` you don't need to find openal-soft manually. The package manager will fetch sources and build the libraries as needed. Qt is not handled through `vcpkg` like the other dependencies, since it is easier to download and use the prebuilt Qt libraries. In addition, it takes a long time to fetch and compile Qt through `vcpkg` and it is more error-prone.

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

### Installing Qt6.x:

Download the installer:
https://www.qt.io/download-qt-installer-oss

Select the right OS and install somewhere you can find it.

**Important:** You must select "Customize install", then find and select _"Qt WebSockets"_ under one of the treeview menus on the customization page. Otherwise CMake will not have that package available and TSRE5 cannot be built without it. _"Qt WebSockets"_ is not included in the standard install configuration. Make sure to check it in the treeview menu: `Qt -> Qt 6.x.x -> Additional Libraries -> Qt WebSockets`

Find the path to the `mingw_64` folder within the Qt6 installation, you will need it later.

It should look something like `C:\path\to\Qt\6.x.x\mingw_64`, or similar, depending on version and where you installed Qt6.

### Installing CMake:

In the windows build of Qt6, CMake is included. So you don't need to download and install it yourself.

The scripts will find the path to the CMake binary automatically when you provide the Qt6 directory.

### Installing MinGW:

In the windows build of Qt6, MinGW 13.1 is included. So you don't need to download and install it yourself.

The script `build-configuration.bat` will find the paths to the MinGW binaries automatically when you provide the Qt6 directory.

### Installing and setting up vcpkg:

Open powershell and run:

```powershell
cd C:/
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
```

Next, run the bootstrap script:

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

### Building TSRE5

Clone and change directory to the TSRE5 repository:
```powershell
git clone <repo url>
cd <local repo directory name>
```

Enter the scripts directory:

```powershell
cd scripts
```

Now run `configure-build.bat`, this script will set up the build directory using CMake.

The build configuration script will ask you to provide the Qt installation directory, the vcpkg directory and which vcpkg triplet you want to use.

The vcpkg triplets you can use are:
- x64-mingw-dynamic
- x86-mingw-dynamic

To run the build configuration script:
```powershell
./configure-build.bat
```

You only need to run the build configuration script once. Or again if you later want to change directory paths or triplets.

When the build directory is configured you can run `build.bat`.

To run the build script:
```powershell
./build.bat
```

### Packaging TSRE5

First, make sure you have run `configure-build.bat` as well as `build.bat`.

Then run:

```powershell
./package-windows.bat
```

This script will create a `./dist` folder with the compiled .exe, along with all DLLs and app data required to run TSRE5.

Windows 10+ is required as the script uses `curl`. On earlier versions of windows, you can probably download curl and add it to the system environment path to run the script.

The script uses `curl` to download replacements for `opengl32sw.dll` that is included with Qt6. This DLL is used when no GPU or hardware acceleration is available on a machine and OpenGL falls back to using software-based rendering. The `opengl32sw.dll` binary does not support OpenGL 3.0+, and therefore does not work with TSRE. So, the script downloads replacements from the Mesa3D package that actually work with OpenGl 3.0+ for this purpose.

Otherwise, the script only copies binaries and other assets from the local machine into the `./dist` folder. These are binaries from the Qt6 installation, binaries built through vcpkg and assets from the project folder.

## Linux (Debian-based distros)

### Installing git:

```bash
sudo apt install git
```

Verify that you can run `git`:
```bash
git --version
```

### Installing Qt6.9.x:

Download the installer:
https://www.qt.io/download-qt-installer-oss

Select the right OS and install somewhere you can find it.

**Important:** You must select "Customize install", then find and select _"Qt WebSockets"_ under one of the treeview menus on the customization page. Otherwise CMake will not have that package available and TSRE5 cannot be built without it. _"Qt WebSockets"_ is not included in the standard install configuration. Make sure to check it in the treeview menu: `Qt -> Qt 6.x.x -> Additional Libraries -> Qt WebSockets`

Find the path to the `gcc_64` folder within the Qt6 installation, you will need it later.

It should look something like `/path/to/Qt/6.x.x/gcc_64`, or similar, depending on version and where you installed Qt6.

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
```

Next, run the bootstrap script:

```bash
./bootstrap-vcpkg.sh -disableMetrics
```

Edit your `~/.bashrc` (or similar, depending on the shell you use) and add this line at the bottom:
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

### Building TSRE5

Clone and change directory to the TSRE5 repository:
```bash
git clone <repo url>
cd <local repo directory name>
```

Enter the scripts directory:

```bash
cd scripts
```

Now run `configure-build.sh`, this script will set up the build directory using CMake.
You might need to use `chmod +x configure-build.sh` before you can execute the script.

The vcpkg triplet you can use is:
- x64-linux-dynamic

Qt will always be linked dynamically regardless of what you use, it needs to be due to their OSS license.

To run the build configuration script:
```bash
./configure-build.sh
```

You only need to run the build configuration script once. Or again if you later want to change directory paths or triplets.

When the build directory is configured you can run `build.sh`.
Again, you might need to use `chmod +x build.sh` before you can execute the script.

To run the build script:
```bash
./build.sh
```

## macOS

I don't have a Mac, so I can't test and document this process.

It's a somewhat similar process to Linux. Use 'brew' rather than 'apt', and OSX-specific vcpkg triplets.

Those vcpkg triplets are:
- x64-osx-dynamic
- arm64-osx-dynamic