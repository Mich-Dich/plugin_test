# =============================================================================
# Specific Plugin CMake Configuration
# =============================================================================
# Project:      plugin test for Gluttony
# Location:     ${PROJECT_SOURCE_DIR}/plugin/PLUGIN_NAME/CMakeLists.txt
# Purpose:      compile the plugin in this die as a shared lib
# =============================================================================

set(PLUGIN_NAME                                 "logger")
file(GLOB_RECURSE                               PLUGIN_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
add_library(${PLUGIN_NAME}                      SHARED ${PLUGIN_SOURCES})
target_compile_definitions(${PLUGIN_NAME}       PRIVATE GLT_MODULE_NAME="${PLUGIN_NAME}")
target_include_directories(${PLUGIN_NAME}       PRIVATE
    ${CMAKE_SOURCE_DIR}/core/src/               # core sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/            # this plugin sources
)
set_target_properties(${PLUGIN_NAME}            PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY                    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugin/${PLUGIN_NAME}"
    OBJECT_OUTPUT_DIRECTORY                     "${CMAKE_BINARY_DIR}/bin-int/$<CONFIG>/plugin/${PLUGIN_NAME}"
)
target_link_options(${PLUGIN_NAME}              PRIVATE -rdynamic)

# Find PCH header if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
    # Enable precompiled headers globally for this target
    target_precompile_headers(${CORE_NAME}      PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"
    )

    # Force all files to use the PCH
    target_compile_options(${CORE_NAME}         PRIVATE
            "$<$<CXX_COMPILER_ID:MSVC>:/FI${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
            "$<$<CXX_COMPILER_ID:GNU>:-include${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
            "$<$<CXX_COMPILER_ID:Clang>:-include${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
    )
endif()


include(${CMAKE_SOURCE_DIR}/helper.cmake)
print_source_file_details(
    SOURCES         ${PLUGIN_SOURCES}
    NAME            ${PLUGIN_NAME}
    SOURCE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBOSE         CORE_VERBOSE_INFO
    TARGET          ${PLUGIN_NAME}
)
