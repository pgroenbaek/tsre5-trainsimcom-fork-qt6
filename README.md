![TSRE](http://koniec.org/tsre5/1.png)

# TSRE5
OpenRails editors and tools. 

**Note:** This is an experimental Qt6 version of the Trainsim.com Fork

The stable Qt5 version is [available here](https://github.com/eric-from-trainsim/TSRE5-Trainsim.Com-Fork). 

This is based off the v7.012 codebase as of January 2024. There are multiple changes in functionality that may or may not be present in future versions issued by Goku.

Full change log and testing versions are available at Trainsim.com

https://www.trainsim.com/tsre


## Unresolved Issues

The experimental Qt6 version has the following unresolved issues:

1. Broken OpenGL rendering pipeline
   
   Textures are not rendered properly. The OpenGL compatibility profile is handled in a
   much more strict manner by Qt6 in comparison to Qt5. What needs changing here is unknown at the moment.
   Figuring this out might require some deep research into how it works and what exactly has changed.

2. Broken Qt signal/slot connections
   
   There are roughly 800 signal/slot connections in total scattered throughout the application
   that potentially could be broken (and a fair amount are). The old SIGNAL/SLOT macros result in errors at run-time when broken.
   Therefore changing them to the newer pointer-based syntax that result in compile-time errors is probably the best
   way to make sure everything still works. This is easy enough to do but will take a fair amount of time due to the sheer number of connections.

3. Support for .DDS not built into Qt6 by default
   
   Since around Qt5.8 there is no DDS support in QImage by default. The Qt DDS plugin must be compiled from scratch, and even that
   is sketchy because it is not maintained properly. So another library must be used for this (e.g. `gli` is a good option).