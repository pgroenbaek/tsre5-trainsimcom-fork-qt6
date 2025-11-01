if(NOT QT_PATH)
    if(DEFINED ENV{QT_PATH})
        set(QT_PATH "$ENV{QT_PATH}" CACHE PATH "Qt path")
    endif()
endif()

if(NOT DEFINED QT_PATH)
    message(FATAL_ERROR "QT_PATH not defined! Pass it or export as environment variable.")
endif()

set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake" CACHE PATH "Qt6 CMake modules")