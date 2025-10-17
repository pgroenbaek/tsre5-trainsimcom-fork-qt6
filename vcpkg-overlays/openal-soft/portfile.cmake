# Fetch the OpenAL-Soft source code from GitHub
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO kcat/openal-soft
    REF openal-soft-1.18.2
    SHA512 85c62d3d16d2a371c1930310eed7219031203824289d9a30d60000f8e124ffa67e1bbfb15f1ba6841ef7346e88c000b9cca51c79d32c02e5dc9870392c536723
)

# Platform-specific setup
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # On Linux, use ALSA and/or OSS if needed
    set(ALSOFT_EXAMPLES OFF CACHE BOOL "Disable examples")
    set(ALSOFT_TESTS OFF CACHE BOOL "Disable tests")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # On Windows, OpenAL Soft should use DirectSound or WASAPI by default
    set(ALSOFT_EXAMPLES OFF CACHE BOOL "Disable examples")
    set(ALSOFT_TESTS OFF CACHE BOOL "Disable tests")

endif()

# Configuring the CMake build for OpenAL-Soft
vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DALSOFT_EXAMPLES=${ALSOFT_EXAMPLES}
        -DALSOFT_TESTS=${ALSOFT_TESTS}
        -DALSOFT_INSTALL=ON
)

# Perform the build and installation
vcpkg_build_cmake()
vcpkg_install_cmake()

# Handle installation of the OpenAL share directory for both configurations
foreach(CONFIG_TYPE IN ITEMS release debug)
    file(INSTALL "${CURRENT_PACKAGES_DIR}/share/openal"
         DESTINATION "${CURRENT_PACKAGES_DIR}/${CONFIG_TYPE}/share/openal-soft")
endforeach()

# Fix up any CMake targets to ensure proper linking
vcpkg_fixup_cmake_targets()
