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
set(CMAKE_CXX_STANDARD                              23)
set(CMAKE_CXX_STANDARD_REQUIRED                     ON)
set(CMAKE_CXX_EXTENSIONS                            OFF)

add_subdirectory(vendor)
add_subdirectory(core)
add_subdirectory(plugin)
