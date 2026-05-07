# =============================================================================
# Vendor Dependencies
# =============================================================================
# Location:     ${PROJECT_SOURCE_DIR}/vendor/CMakeLists.txt
# Purpose:      Integrate third-party libraries (submodules, packages, downloads)
# Important:    Builds before core to satisfy dependencies
# =============================================================================

# GLM - Header-only library
add_library(glm                     INTERFACE)
target_include_directories(glm      INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
    # ${CMAKE_CURRENT_SOURCE_DIR}/vma/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
    # ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    # ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# # GLFW - Library that needs to be built
# set(GLFW_BUILD_OPENGL       OFF CACHE BOOL "Disable OpenGL support"             FORCE)
# set(GLFW_BUILD_WAYLAND      OFF CACHE BOOL "Disable Wayland support"            FORCE)
# # set(GLFW_BUILD_X11          OFF CACHE BOOL "Disable X11 support"                FORCE)
# set(GLFW_BUILD_EXAMPLES     OFF CACHE BOOL "Build the GLFW example programs"    FORCE)
# set(GLFW_BUILD_TESTS        OFF CACHE BOOL "Build the GLFW test programs"       FORCE)
# set(GLFW_BUILD_DOCS         OFF CACHE BOOL "Build the GLFW documentation"       FORCE)
# set(GLFW_INSTALL            OFF CACHE BOOL "Generate installation target"       FORCE)

# add_subdirectory(glfw)
# add_subdirectory(vma)
# add_subdirectory(vk-bootstrap)
# add_subdirectory(vk_ray)

# add_library(glfw_lib ALIAS glfw)        # Create an alias for consistency

# # ------------- ImGui -------------
# add_library(imgui STATIC)

# file(GLOB IMGUI_BACKEND_SOURCES                     # Add ImGui backends (GLFW + Vulkan)
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
# )

# target_sources(imgui PRIVATE                        # Add all sources to the imgui library
#     ${IMGUI_BACKEND_SOURCES}
# )

# target_include_directories(imgui PUBLIC             # Include directories
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
# )

# target_link_libraries(imgui PUBLIC                  # Link dependencies
#     glfw
#     Vulkan::Vulkan
# )

# target_compile_definitions(imgui                    # Add compile definitions for imgui
#     PRIVATE
#         IMGUI_IMPL_GLFW_DISABLE_X11
#         IMGUI_IMPL_VULKAN_NO_PROTOTYPES             # Use Vulkan SDK headers instead of volk
#         GLFW_INCLUDE_NONE                           # Don't include OpenGL headers
# )

# message(STATUS "Vendor libraries configured")
# message(STATUS "  - GLM: header-only (interface target)")
# message(STATUS "  - GLFW: library target 'glfw'")
# message(STATUS "  - ImGui: static library with Vulkan+GLFW backends")
# message(STATUS "  - vk-bootstrap: header-only library")
# message(STATUS "  - vk_ray: Vulkan Raytracing Library")

# # Make sure to install the following libs whenworking with:
# # sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxi-dev libxcursor-dev libxxf86vm-dev
