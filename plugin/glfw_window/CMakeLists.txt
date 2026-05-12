# =============================================================================
# Specific Plugin CMake Configuration
# =============================================================================
# Project:      plugin test for Gluttony
# Location:     ${PROJECT_SOURCE_DIR}/plugin/PLUGIN_NAME/CMakeLists.txt
# Purpose:      compile the plugin in this die as a shared lib
# =============================================================================

set(PLUGIN_NAME                                 "glfw_window")
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
target_link_libraries(${PLUGIN_NAME}            PRIVATE
    glm
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")              # Find PCH header if it exists
    target_precompile_headers(${CORE_NAME}      PRIVATE         # Enable precompiled headers globally for this target
            "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"
    )

    target_compile_options(${CORE_NAME}         PRIVATE         # Force all files to use the PCH
            "$<$<CXX_COMPILER_ID:MSVC>:/FI${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
            "$<$<CXX_COMPILER_ID:GNU>:-include${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
            "$<$<CXX_COMPILER_ID:Clang>:-include${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h>"
    )
endif()


# ----------------- GLFW vendor function: ensures GLFW is present in vender/ and links it -----------------
function(ensure_glfw VENDOR_DIR)
    file(MAKE_DIRECTORY "${VENDOR_DIR}")                        # Create the vender directory if it doesn't exist

    set(GLFW_DIR "${VENDOR_DIR}/glfw")
    if(NOT EXISTS "${GLFW_DIR}")
        message(STATUS "GLFW not found in ${VENDOR_DIR}. Cloning from GitHub...")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --depth 1
            "https://github.com/glfw/glfw.git" "${GLFW_DIR}"
            RESULT_VARIABLE clone_result
            ERROR_VARIABLE  clone_error
        )
        if(NOT clone_result EQUAL 0)
            message(FATAL_ERROR "Failed to clone GLFW: ${clone_error}")
        endif()
        message(STATUS "GLFW cloned successfully into ${GLFW_DIR}")
    else()
        message(STATUS "GLFW already present in ${GLFW_DIR}")
    endif()
    set(GLFW_BUILD_OPENGL                           OFF CACHE BOOL "Disable OpenGL support"             FORCE)
    set(GLFW_BUILD_WAYLAND                          OFF CACHE BOOL "Disable Wayland support"            FORCE)
    # set(GLFW_BUILD_X11                              OFF CACHE BOOL "Disable X11 support"                FORCE)
    set(GLFW_BUILD_EXAMPLES                         OFF CACHE BOOL "Build the GLFW example programs"    FORCE)
    set(GLFW_BUILD_TESTS                            OFF CACHE BOOL "Build the GLFW test programs"       FORCE)
    set(GLFW_BUILD_DOCS                             OFF CACHE BOOL "Build the GLFW documentation"       FORCE)
    set(GLFW_INSTALL                                OFF CACHE BOOL "Generate installation target"       FORCE)
    target_compile_definitions(${PLUGIN_NAME}       PRIVATE GLFW_INCLUDE_NONE)
    add_subdirectory("${GLFW_DIR}"                  EXCLUDE_FROM_ALL)            # Add GLFW as a subdirectory – this defines the 'glfw' target (static library)
endfunction()
ensure_glfw("${CMAKE_CURRENT_SOURCE_DIR}/vender")               # Call the function with the local vender directory
target_link_libraries(${PLUGIN_NAME}                PRIVATE glfw)       # Link GLFW to the plugin



include(${CMAKE_SOURCE_DIR}/helper.cmake)
print_source_file_details(
    SOURCES         ${PLUGIN_SOURCES}
    NAME            ${PLUGIN_NAME}
    SOURCE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBOSE         CORE_VERBOSE_INFO
    TARGET          ${PLUGIN_NAME}
)
