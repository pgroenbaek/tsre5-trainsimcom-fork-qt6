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

- There are some issues crashing with the Mesa3D drivers for software rendering using OpenGL (maybe just remove it and only allow hardware-based rendering?)
- KML markers are not being displayed in the route editor OpenGL window
- Maps loaded from OpenStreetMap looks different than in earlier versions. More pixelated and with more irrelevant details drawn. Problem with loading XML?
- Crash when trying to display terrain textures in the terrain tool UI.
- Path editing in the activity tool does not seem to work properly (tool selection issue?).
- Activity tools dialogs: Are you supposed to be able to check "This" in the dialog lists?
- There seems to be a problem when linking signals (tool selection issue?).
- More things are probably going to show up while testing...