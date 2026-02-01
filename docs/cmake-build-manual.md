# CMake build manual

**Table of Contents:**
- [Prerequisites](#prerequisites)
- [Windows](#windows)
    - [Setting up the prerequisites on Windows](#setting-up-the-prerequisites-on-windows)
    - [Building TSRE5 for Windows](#building-tsre5-for-windows)
- [Linux](#linux)
    - [Setting up the prerequisites on Linux](#setting-up-the-prerequisites-on-linux)
    - [Building TSRE5 for Linux](#building-tsre5-for-linux)

## Prerequisites

| Tool             | Why it's needed                                                          |
| ---------------- | ------------------------------------------------------------------------ |
| **Git**          | To clone the TSRE5 repo *and* vcpkg                                      |
| **Qt 6.x**     | To provide the GUI framework and required modules for the application    |
| **C++ compiler** | To compile the project and dependencies                                  |
| **CMake**        | To configure and generate build files                                    |
| **vcpkg**        | To manage and integrate libraries other than Qt itself                   |

The `vcpkg` tool by Microsoft is a dependency manager that handles downloading, building, and integrating libraries into the project automatically. It is similar to what NuGet does for C#, just for C/C++ instead.

For example, when using `vcpkg`, you don't need to find libraries like openal-soft manually. The dependency manager will fetch sources and build the libraries as needed. Qt is not handled through `vcpkg` like the other dependencies, as it is easier to download and use the prebuilt Qt libraries. It takes a long time to fetch and compile Qt through `vcpkg` and doing so is more error-prone than using the prebuilt binaries.

## Windows

### Setting up the prerequisites on Windows

#### Installing Git

Download the installer for Windows:
https://git-scm.com/downloads/win

1. Install it.

2. Open a new PowerShell window, and verify you can run:

    ```powershell
    git --version
    ```

    If you are unable to run it, you will need to add the installation directory to the system PATH environment variable.

3. Done!

#### Installing Qt6.x:

Download the installer for _Windows x64_:
https://www.qt.io/download-qt-installer-oss

1. Start the installer and log in. Make a free Qt account if you don't have one already.

2. Accept the license and make sure to check _"I am an individual and do not use Qt for any company"_.

3. Installation options:

    Specify where you want Qt installed.

    Make sure that _"Qt 6.x for desktop development"_ and _"Customize install"_ are checked.

    You can also check _"Qt Design Studio"_ if you want to be able to edit `.ui` files with a drag and drop editor.

    ![Qt6 Windows Installation Options](/docs/images/qt6-windows-installation-options.png)

4. Customization:

    Check the Qt version you want installed.

    ![Qt6 Windows Installation Customization](/docs/images/qt6-windows-installation-customize.png)

    In the expanded dropdown menu of the selected Qt version, you can uncheck `MSVC 2022 ARM64`, `LLVM-MinGW 17.0.6 64-bit`, `MSVC 2022 64-bit` and `Android`. They are not needed for building TSRE5 and will only take up unnecessary disk space.

    ![Qt6 Windows Installation Customization - Qt Arch](/docs/images/qt6-windows-installation-customize-qt-arch.png)

    Make sure `Qt WebSockets` is checked under _Additional Libraries_. TSRE5 depends on it and cannot be built without it.

    ![Qt6 Windows Installation Customization - Qt WebSockets](/docs/images/qt6-windows-installation-customize-qt-websockets.png)

    Also make sure `MinGW 13.1.0 64-bit` and `CMake` are checked under Build Tools.

    ![Qt6 Windows Installation Customization - Compilers](/docs/images/qt6-windows-installation-customize-compilers.png)

5. Now proceed with the Qt6 installation.

6. Done!

#### Installing and setting up vcpkg:

The vcpkg dependency manager must be fetched through git and requires a few extra steps to set it up correctly.

1. Open PowerShell and change directory to where you want to put vcpkg. Then run:

    ```powershell
    git clone https://github.com/microsoft/vcpkg.git
    ```

2. Enter the vcpkg directory:

    ```powershell
    cd vcpkg
    ```

3. Next, run the bootstrap script:

    ```powershell
    .\bootstrap-vcpkg.bat -disableMetrics
    ```

4. Add the vcpkg folder to the system path env variable, e.g. `C:\vcpkg`, depending on where you cloned it to.

5. Restart PowerShell, and verify you can run:

    ```powershell
     vcpkg --version
    ```

    If you are unable to run it, you will need to adjust the system PATH environment variable.

6. Run this to integrate vcpkg:

    ```powershell
    vcpkg integrate install
    ```

7. Done!

### Building TSRE5 for Windows

Once you have the prerequisites in place you can perform the initial setup for building TSRE5:

1. First, clone a copy of the TSRE5 repository to your local machine.

2. Place a file named `CMakeUserPresets.json` into the local project root directory.

    If you cloned the TSRE5 repository into e.g. `C:/path/to/TSRE5Repo`, then place the file at `C:/path/to/TSRE5Repo/CMakeUserPresets.json`.

    The file must contain the following content:

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

3. Change the `QT_ROOT` and `VCPKG_ROOT` values to where you installed Qt6 and vcpkg.

    If you selected different versions of Qt6 and MinGW during the Qt6 installation, then adjust the corresponding values in the preset. The values correspond to the folder names found in `C:/path/to/Qt6` and `C:/path/to/Qt6/Tools` respectively.

You are now ready to build TSRE5 using CMake. There are two ways to do this. Either use the CMake extension in Visual Studio Code, or run the build scripts provided in the `/scripts` folder by hand.

#### Building through the CMake extension in VS Code

To build through Visual Studio Code, you first need to install the CMake extension. For how to do this, see the [setting up VS Code](/docs/setting-up-vscode.md) guide.

Once the CMake extension is installed, and you open the project in VS Code, the extension will automatically pick up on the project configurations and the information you put into the `CMakeUserPresets.json` file.

To apply a configuration preset, click the edit button under _"Configure"_ and select either Debug or Release in the menu that pops up.

**Note:** The first time you configure, vcpkg will fetch and build all dependencies. Expect it to take more than 20 minutes. Be patient. This is normal. On subsequent configuration runs, vcpkg will use cached libraries and be much quicker.

![VSCode CMake configuration selection](/docs/images/vscode-cmake-ext-configurations.png)

You can build the project when CMake has been configured with either the Debug or Release preset. To build, click one of the buttons marked with red in the image below.

On Windows, CMake is configured to create `/dist` folders as part of the build process. These folders contain all libraries and assets needed to run TSRE5, and the contents can be packaged into a zip file or an installer.

Once you've built TSRE5, you can launch it through the debugger or through the executable itself using the buttons marked with green. You can also launch it clicking the executable or `.bat` files in the `/dist` folders.

![VSCode CMake running build, debug, and launch](/docs/images/vscode-cmake-ext-annotated.png)


#### Building through the provided `.bat` scripts

As an alternative to using the VS Code CMake extension, you can also run the `.bat` files in the `/scripts` directory.

They do the same as the CMake extension and will also use the information you put into the `CMakeUserPresets.json` file.

To use the scripts (either release or debug):

1. If you execute the scripts in a terminal, first enter the `/scripts` directory:

    ```powershell
    cd scripts
    ```

    You can also double-click the scripts in explorer, and then this step is not needed.

2. Execute the `configure-<config>-windows.bat` script. This script will set up the build directory using CMake.

    ```powershell
    ./configure-<config>-windows.bat
    ```

    **Note:** The first time you configure, vcpkg will fetch and build all dependencies. Expect it to take more than 20 minutes. Be patient. This is normal. On subsequent configuration runs, vcpkg will use cached libraries and be much quicker.

3. Execute the `build-<config>-windows.bat` script. This script will actually build the TSRE5 executable.

    ```powershell
    ./build-<config>-windows.bat
    ```

    On Windows, CMake is configured to create `/dist` folders as part of the build process. These folders contain all libraries and assets needed to run TSRE5, and the contents can be packaged into a zip file or an installer.

4. You can now launch TSRE5 using the executable or `.bat` files in the `/dist/<config>` folder.

Using the `clean-<config>-windows.bat` script, you can also remove the contents of the `/dist/<config>` folder and clean up any artifacts in the build folder.

# Linux

## Setting up the prerequisites on Linux

### Installing git

```bash
sudo apt install git
```

Verify that you can run `git`:
```bash
git --version
```

### Installing Qt6.x

Download the installer for _Linux x64_:
https://www.qt.io/download-qt-installer-oss

1. Make the installer executable:

    ```bash
    chmod +x ./qt-online-installer-linux-x64-4.10.0.run
    ```

2. Start the installer:

    ```bash
    ./qt-online-installer-linux-x64-4.10.0.run
    ```

   Log in when the installer asks you to. Make a free Qt account if you don't have one already.

3. Accept the license and make sure to check _"I am an individual and do not use Qt for any company"_.

4. Installation options:

    Specify where you want Qt installed.

    Make sure that _"Qt 6.x for desktop development"_ and _"Customize install"_ are checked.

    You can also check _"Qt Design Studio"_ if you want to be able to edit `.ui` files with a drag and drop editor.

    ![Qt6 Linux Installation Options](/docs/images/qt6-linux-installation-options.png)

5. Customization:

    Check the Qt version you want installed.

    ![Qt6 Linux Installation Customization](/docs/images/qt6-linux-installation-customize.png)

    In the expanded dropdown menu of the selected Qt version, you can uncheck `Android`. It is not needed for building TSRE5 and will only take up unnecessary disk space.

    ![Qt6 Linux Installation Customization - Qt Arch](/docs/images/qt6-linux-installation-customize-qt-arch.png)

    Make sure `Qt WebSockets` is checked under _Additional Libraries_. TSRE5 depends on it and cannot be built without it.

    ![Qt6 Linux Installation Customization - Qt WebSockets](/docs/images/qt6-linux-installation-customize-qt-websockets.png)

    Also make sure `Ninja` and `CMake` are checked under Build Tools.

    ![Qt6 Linux Installation Customization - Compilers](/docs/images/qt6-linux-installation-customize-compilers.png)

5. Now proceed with the Qt6 installation.

6. Done!


### Installing gcc and ninja:

```bash
sudo apt install build-essential ninja-build
```

Verify that you can run `gcc` and `ninja`:
```bash
gcc --version
ninja --version
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

The vcpkg dependency manager must be fetched through git and requires a few extra steps to set it up correctly.

1. First clone the vcpkg repository:

    ```bash
    git clone https://github.com/microsoft/vcpkg.git ~/.vcpkg
    ```
2. Change directory to the cloned repo:

    ```bash
    cd ~/.vcpkg
    ```

3. Next, run the bootstrap script:

    ```bash
    ./bootstrap-vcpkg.sh -disableMetrics
    ```

4. Edit your `~/.bashrc` (or similar, depending on the shell you use) and add this line at the bottom:

    ```bash
    export PATH="$HOME/.vcpkg:$PATH"
    ```

    To reload your shell:

    ```bash
    source ~/.bashrc
    ```

5. Verify that you can run `vcpkg`:

    ```bash
    vcpkg --version
    ```

6. To integrate `vcpkg`:

    ```bash
    vcpkg integrate install
    ```

7. Done!

### Building TSRE5 for Linux

Once you have the prerequisites in place you can perform the initial setup for building TSRE5:

1. First, clone a copy of the TSRE5 repository to your local machine.

2. Place a file named `CMakeUserPresets.json` into the local project root directory.

    If you cloned the TSRE5 repository into e.g. `/path/to/TSRE5Repo`, then place the file at `/path/to/TSRE5Repo/CMakeUserPresets.json`.

    The file must contain the following content:

    ```json
    {
      "version": 3,
      "configurePresets": [
        {
          "name": "linux-userenv",
          "hidden": true,
          "environment": {
            "QT_ROOT": "/path/to/Qt6",
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

3. Change the `QT_ROOT` and `VCPKG_ROOT` values to where you installed Qt6 and vcpkg.

    If you selected a different version of Qt6 during the installation, then adjust the Qt version in the preset. The value corresponds to the folder name found in `/path/to/Qt6`.

You are now ready to build TSRE5 using CMake. There are two ways to do this. Either use the CMake extension in Visual Studio Code, or run the build scripts provided in the `/scripts` folder by hand.


<!-- 
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
``` -->

## macOS

I don't have a Mac, so I can't test and document this process.

It's a somewhat similar process to Linux, using Qt6 for Mac and OSX-specific vcpkg triplets.

Those vcpkg triplets are:
- x64-osx-dynamic
- arm64-osx-dynamic