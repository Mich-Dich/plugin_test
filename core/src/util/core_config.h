
#pragma once



// FORWARD DECLARATIONS ================================================================================================

namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    #if 0
        #define RENDER_API_OPENGL
    #else
        #define RENDER_API_VULKAN
    #endif

    // collect timing-data from every major function?
    #define PROFILE								    1	// general
    #define PROFILE_APPLICATION                     1
    #define PROFILE_RENDERER                        0

    // log assert and validation behaviour?
    // NOTE - expr in assert/validation will still be executed
    #define ENABLE_LOGGING_FOR_ASSERTS              1
    #define ENABLE_LOGGING_FOR_VALIDATION           1


    #define ASSET_EXTENTION			                ".gltasset"      // Extension for asset files
    #define PROJECT_EXTENTION    		            ".gltproj"       // Extension for project files
    #define FILE_EXTENSION_CONFIG   	            ".yml"        	// Extension for YAML config files
    #define FILE_EXTENSION_INI      	            ".ini"          // Extension for INI config files

    // Directory structure macros
    #define METADATA_DIR            	            "metadata"      // Directory for metadata files
    #define CONFIG_DIR              	            "config"        // Directory for configuration files
    #define CONTENT_DIR             	            "content"       // Directory for content files
    #define SOURCE_DIR              	            "src"           // Directory for source code

    inline constexpr auto PLUGIN_DIR_NAME           = "plugin";

    // #define PROJECT_PATH				            GLT::application::get().get_project_path()
    // #define PROJECT_NAME				            GLT::application::get().get_project_data().name
    // #define ENGINE_CONTENT_PATH			            GLT::util::get_executable_path() / "assets"
    // #define ENGINE_SHADER_PATH			            GLT::util::get_executable_path() / "shaders_compiled"
    // #define ENGINE_RAW_SHADER_PATH		            GLT::util::get_executable_path() / "shaders"


    #if defined(PLATFORM_LINUX)
        #define GLT_HELPER                          "gluttony_helper"
    #elif defined(PLATFORM_WINDOWS)
        #define GLT_HELPER                          "gluttony_helper.exe"
    #endif

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

}
