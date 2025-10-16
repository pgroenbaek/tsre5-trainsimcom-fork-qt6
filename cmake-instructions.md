# CMake build instructions

**Note:** These instructions are work-in-progress and the build does not even compile yet using CMake.

## Prerequisites

- CMake
- A Qt installation

## Windows

TODO (but very similar steps to linux)

You would for example use something different than "Unix Makefiles" when generating makefiles using cmake.

## Linux

### Preparing the build makefiles with CMake

Make and enter the build directory.

```
mkdir build
cd build
```

Now create the makefiles with CMake.

The path to `lib/cmake` is inside the Qt installation. It could look something like `/home/peter/Programs/Qt5.7.0/5.7/gcc_64/lib/cmake`.

The `../` refers to the project root directory where all the source files are. We execute the `cmake` command from inside the build folder to not make a mess of the source file folder with all sorts of build artefacts.

```
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