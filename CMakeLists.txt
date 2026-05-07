# =============================================================================
# Root CMake Configuration
# =============================================================================
# Project:      plugin test for Gluttony
# Location:     ${PROJECT_SOURCE_DIR}/CMakeLists.txt
# Purpose:      Orchestrates entire build system, sets global policies
#
# Usage:
#   mkdir build && cd build
#   cmake .. [-DOPTION=VALUE ...]
#   cmake --build .
# =============================================================================

cmake_minimum_required(VERSION                      3.20)
project(Gluttony VERSION 0.0                        LANGUAGES CXX)
option(CORE_VERBOSE_INFO                            "Show detailed file list for Core" OFF)
set(CORE_VERBOSE_INFO                               TRUE)

# These global output directories are only fallbacks - each target overrides them individually
# For single-config generators (make, ninja)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/bin/${PLATFORM_NAME}-${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-${CMAKE_BUILD_TYPE})

# For multi-config generators (Visual Studio, Xcode)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_BINARY_DIR}/bin/${PLATFORM_NAME}-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_BINARY_DIR}/bin/${PLATFORM_NAME}-release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${CMAKE_BINARY_DIR}/bin/${PLATFORM_NAME}-rel_with_debug_info)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-rel_with_debug_info)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-rel_with_debug_info)

# Set intermediate directory (object files) - for single-config
set(CMAKE_OBJECT_OUTPUT_DIRECTORY                   ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-${CMAKE_BUILD_TYPE})

# Set intermediate directory for multi-config
set(CMAKE_OBJECT_OUTPUT_DIRECTORY_DEBUG             ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-debug)
set(CMAKE_OBJECT_OUTPUT_DIRECTORY_RELEASE           ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-release)
set(CMAKE_OBJECT_OUTPUT_DIRECTORY_RELWITHDEBINFO    ${CMAKE_BINARY_DIR}/bin-int/${PLATFORM_NAME}-rel_with_debug_info)

set(CMAKE_CXX_STANDARD                              23)
set(CMAKE_CXX_STANDARD_REQUIRED                     ON)
set(CMAKE_CXX_EXTENSIONS                            OFF)

add_subdirectory(vendor)
add_subdirectory(core)
add_subdirectory(plugin)
