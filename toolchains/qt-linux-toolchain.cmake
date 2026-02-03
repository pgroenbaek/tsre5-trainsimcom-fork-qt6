if(NOT QT_ROOT)
  set(QT_ROOT $ENV{QT_ROOT} CACHE PATH "Qt root")
endif()

if(NOT QT_VERSION)
  set(QT_VERSION $ENV{QT_VERSION} CACHE STRING "Qt version")
endif()

if(NOT QT_ARCH)
  set(QT_ARCH $ENV{QT_ARCH} CACHE STRING "Qt arch")
endif()

if(QT_ROOT AND QT_VERSION AND QT_ARCH)
  set(QT_PATH "${QT_ROOT}/${QT_VERSION}/${QT_ARCH}" CACHE PATH "Qt full path")
elseif(NOT QT_PATH AND DEFINED ENV{QT_PATH})
  set(QT_PATH "$ENV{QT_PATH}" CACHE PATH "Qt path")
endif()

if(NOT QT_PATH)
  message(FATAL_ERROR "QT_PATH not defined! Set QT_PATH or QT_ROOT/QT_VERSION/QT_ARCH.")
endif()

set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake" CACHE PATH "Qt6 CMake modules")