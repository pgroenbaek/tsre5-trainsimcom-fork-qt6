![TSRE](./tsre_appdata/0.697/load.png)

# TSRE5
OpenRails editors and tools. 

**Note:** This is an experimental Qt6-based version of the Trainsim.com Fork.

The stable Qt5-based v0.8.00x version is [available here](https://github.com/eric-from-trainsim/TSRE5-Trainsim.Com-Fork). 

This is based off the v0.7.012 codebase as of January 2024. There are multiple changes in functionality that may or may not be present in future versions issued by Goku.

Full change log and testing versions are available at Trainsim.com

https://www.trainsim.com/tsre


## Documentation

- User Manual: [How to use the TSRE5 program](https://github.com/pwillard/TSRE5-Document/blob/master/book.pdf)
- Build Manual: [How to build TSRE5 from source using CMake](./docs/cmake-build-manual.md)

## Unresolved Issues

The experimental Qt6 version has the following unresolved issues:

1. Support for .DDS not built into Qt6 by default
   
   Since around Qt5.8 there is no DDS support in `QImage` by default. The Qt DDS plugin must be compiled from scratch, and even that
   is sketchy because it is not maintained properly. So, it must be implemented from scratch or another library must be used for this (e.g. [OpenImageIO](https://github.com/AcademySoftwareFoundation/OpenImageIO) looks like a good option).

2. If the new pointer-based signal/slot connection syntax is used on a superclass and the subclass redefines the same signal/slot,
   the connection will not happen properly to the subclass. This was the case for MapData and the MapDataOSM/MapDataUrlImage subclasses, as well as for PropertiesAbstract and the PropertiesSignal subclass. If part of the UI does not work, this type of issue is a likely candidate.

3. More things might show up while testing...