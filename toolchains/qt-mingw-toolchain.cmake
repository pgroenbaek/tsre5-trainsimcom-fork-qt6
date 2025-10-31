
if(NOT DEFINED QT_PATH AND DEFINED ENV{QT_PATH})
    set(QT_PATH "$ENV{QT_PATH}" CACHE PATH "Qt path")
endif()

if(NOT DEFINED QT_PATH)
    message(FATAL_ERROR "QT_PATH not defined! Pass it or export as environment variable.")
endif()

set(CMAKE_C_COMPILER "${QT_PATH}/../../Tools/mingw1310_64/bin/gcc.exe" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "${QT_PATH}/../../Tools/mingw1310_64/bin/g++.exe" CACHE FILEPATH "")
set(CMAKE_RC_COMPILER "${QT_PATH}/../../Tools/mingw1310_64/bin/windres.exe" CACHE FILEPATH "")
set(CMAKE_MAKE_PROGRAM "${QT_PATH}/../../Tools/mingw1310_64/bin/mingw32-make.exe" CACHE FILEPATH "")

set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake" CACHE PATH "Qt6 CMake modules")
