# =============================================================================
# Root CMake Configuration
# =============================================================================
# Project:      plugin test for Gluttony
# Location:     ${PROJECT_SOURCE_DIR}/core/CMakeLists.txt
# Purpose:      compile the core of the application (microkernel)
# =============================================================================

SET(CORE_NAME                                   "Gluttony")

file(GLOB_RECURSE                               CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
add_executable(${CORE_NAME}                     ${CORE_SOURCES})
set_target_properties(${CORE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)
# For the main executable target (e.g., "Gluttony")
target_link_options(${CORE_NAME}                PRIVATE -rdynamic)
target_link_libraries(${CORE_NAME}              PRIVATE
    # glfw_lib
    glm
)
target_include_directories(${CORE_NAME}         PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# Find PCH header if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h")
    # Enable precompiled headers globally for this target
    target_precompile_headers(${CORE_NAME}      PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src/util/pch.h"
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
    SOURCES    ${CORE_SOURCES}
    NAME       ${CORE_NAME}
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBOSE    CORE_VERBOSE_INFO
    TARGET     ${CORE_NAME}
)
