# CMake build instructions

**Note:** These instructions are work-in-progress and are updated as I go along.

## Prerequisites

- A compiler (like mingw on windows, GCC on linux)
- CMake
- OpenAL
- OpenGL
- A Qt installation

## Windows

TODO (but very similar steps to linux)

You would for example use something different than "Unix Makefiles" when generating makefiles using cmake.

## Linux

### Install dependencies

Needed for CMake and GCC:

```bash
sudo apt install build-essential cmake
```

Needed for Qt5:

```bash
sudo apt install qt5-default qtbase5-dev qtbase5-dev-tools
sudo apt install libqt5websockets5-dev
sudo apt install libqt5opengl5-dev
sudo apt install qttools5-dev qttools5-dev-tools
```

Needed for OpenAL:

```bash
sudo apt install libopenal-dev
```

Needed for OpenGL:

```bash
sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

### Preparing the build makefiles with CMake

Make and enter the build directory.

```bash
mkdir build
cd build
```

Now create the makefiles with CMake.

The path to `lib/cmake` is inside the Qt installation. It could look something like `/home/peter/Programs/Qt5.7.0/5.7/gcc_64/lib/cmake`.

The `../` refers to the project root directory where all the source files are. We execute the `cmake` command from inside the build folder to not make a mess of the source file folder with all sorts of build artefacts.

```bash
cmake -DCMAKE_PREFIX_PATH="/path/to/lib/cmake" -G"Unix Makefiles" ../
```

### Building

First `cd` to the build folder.

Then:

```bash
make
```

### Cleaning the build directory

First `cd` to the build folder.

Then:

```bash
make clean
```