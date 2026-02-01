# CMake build manual

## Prerequisites

| Tool             | Why it's needed                                                          |
| ---------------- | ------------------------------------------------------------------------ |
| **Git**          | To clone the TSRE5 repo *and* vcpkg                                      |
| **Qt 6.x**     | To provide the GUI framework and required modules for the application    |
| **C++ compiler** | To compile the project and dependencies                                  |
| **CMake**        | To configure and generate build files                                    |
| **vcpkg**        | To manage and integrate libraries other than Qt itself                   |

The `vcpkg` tool by Microsoft is a dependency manager that handles downloading, building, and integrating libraries into the project automatically. It is similar to what NuGet does for C#, just for C/C++ instead.

For example, using `vcpkg` you don't need to find openal-soft manually. The package manager will fetch sources and build the libraries as needed. Qt is not handled through `vcpkg` like the other dependencies, as it is easier to download and use the prebuilt Qt libraries. It takes a long time to fetch and compile Qt through `vcpkg` and doing so is more error-prone.

## Windows

### Setting up the prerequisites on Windows

#### Installing Git

Download the installer for Windows:
https://git-scm.com/downloads/win

1. Install it.

2. Open a new powershell window, and verify you can run:

    ```powershell
    git --version
    ```

    If you are unable to run it, you will need to add the installation directory to the system env path variable.

3. Done!

#### Installing Qt6.x:

Download the installer for _Windows x64_:
https://www.qt.io/download-qt-installer-oss

1. Start the installer and login. Make a free Qt account if you don't have one already.

2. Accept the license and make sure to check _"I am an individual and do not use Qt for any company"_.

3. Installation options:

    Specify where you want Qt installed.

    Make sure that _"Qt 6.x for desktop development"_ and _"Customize install"_ are checked.

    You can also check _"Qt Design Studio"_ if you want to be able to edit .ui files with a drag and drop editor.

    ![Qt6 Windows Installation Options](/docs/images/qt6-windows-installation-options.png)

4. Customization:

    Check the Qt version you want installed.

    ![Qt6 Windows Installation Customization](/docs/images/qt6-windows-installation-customize.png)

    In the expanded dropdown menu of the selected Qt version you can uncheck `MSVC 2022 ARM64`, `LLVM-MinGW 17.0.6 64-bit`, `MSVC 2022 64-bit` and `Android`. They are not needed for building TSRE5 and will only take up unnecessary disk space.

    ![Qt6 Windows Installation Customization - Qt Arch](/docs/images/qt6-windows-installation-customize-qt-arch.png)

    Make sure `Qt WebSockets` is checked under _Additional Libraries_. TSRE5 depends on it and cannot be built without it.

    ![Qt6 Windows Installation Customization - Qt WebSockets](/docs/images/qt6-windows-installation-customize-qt-websockets.png)

    Also make sure `MinGW 13.1.0 64-bit` and `CMake` are checked under Build Tools.

    ![Qt6 Windows Installation Customization - Compilers](/docs/images/qt6-windows-installation-customize-compilers.png)

5. Now proceed with the Qt6 installation.

6. Done!

#### Installing and setting up vcpkg:

The vcpkg package manager must be fetched through git and requires a few extra steps to set it up correctly.

1. Open powershell and run:

    ```powershell
    git clone https://github.com/microsoft/vcpkg.git
    ```

2. Next, run the bootstrap script:

    ```powershell
    cd vcpkg

    .\bootstrap-vcpkg.bat -disableMetrics
    ```

3. Add the vcpkg folder to the system path env variable, e.g. `C:\vcpkg`, depending on where you cloned it to.

4. Restart powershell, and verify you can run:

    ```powershell
     vcpkg --version
    ```

    If you are unable to run it, you will need to adjust the system env path variable.

5. Run this to integrate vcpkg:

    ```powershell
    vcpkg integrate install
    ```

6. Done!

### Building TSRE5 on Windows


Clone and change directory to the TSRE5 repository:
```powershell
git clone <repo url>
cd <local repo directory name>
```

#### Building through the CMake Extension in VS Code



#### Building through the provided .bat scripts

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

**Note:** Make sure you don't have spaces in the path to the cloned TSRE directory with code. The build of OpenSSL handled through vcpkg does not like spaces in the path.

You only need to run the build configuration script once. Or again if you later want to change directory paths or triplets.

When the build directory is configured you can run `build.bat`.

To run the build script:
```powershell
./build.bat
```

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "windows-userenv",
      "hidden": true,
      "environment": {
        "QT_ROOT": "C:/path/to/Qt6",
        "QT_VERSION": "6.9.3",
        "QT_ARCH": "mingw_64",
        "QT_MINGW_VERSION": "mingw1310_64",
        "VCPKG_ROOT": "C:/path/to/vcpkg",
        "VCPKG_TRIPLET": "x64-mingw-dynamic"
      }
    },
    {
      "name": "windows-qt-debug-userenv",
      "inherits": ["windows-userenv", "windows-qt-debug"],
      "displayName": "Windows Qt MinGW Debug (User Env)"
    },
    {
      "name": "windows-qt-release-userenv",
      "inherits": ["windows-userenv", "windows-qt-release"],
      "displayName": "Windows Qt MinGW Release (User Env)"
    }
  ]
}
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


```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "linux-userenv",
      "hidden": true,
      "environment": {
        "QT_ROOT": "/path/to/Qt/Qt6.9",
        "QT_VERSION": "6.9.3",
        "QT_ARCH": "gcc_64",
        "VCPKG_ROOT": "$env{HOME}/.vcpkg",
        "VCPKG_TRIPLET": "x64-linux-dynamic"
      }
    },
    {
      "name": "linux-qt-debug-userenv",
      "inherits": ["linux-userenv", "linux-qt-debug"],
      "displayName": "Linux Qt GCC Debug (User Env)"
    },
    {
      "name": "linux-qt-release-userenv",
      "inherits": ["linux-userenv", "linux-qt-release"],
      "displayName": "Linux Qt GCC Release (User Env)"
    }
  ]
}
```

## macOS

I don't have a Mac, so I can't test and document this process.

It's a somewhat similar process to Linux. Use 'brew' rather than 'apt', and OSX-specific vcpkg triplets.

Those vcpkg triplets are:
- x64-osx-dynamic
- arm64-osx-dynamic