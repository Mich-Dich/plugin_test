
#include "util/pch.h"
#include "plugin_system/plugin_manager.h"

// FORWARD DECLARATIONS ================================================================================================

// CONSTANTS =======================================================================================================

// MACROS ==========================================================================================================

#if defined(PLATFORM_LINUX)
    #define ARGC        argc
    #define ARGV        argv
    #define MAIN_FUNC   main(int argc, char* argv[])
#elif defined(PLATFORM_WINDOWS)
    #include <Windows.h>
    #define ARGC        __argc
    #define ARGV        __argv
    #define MAIN_FUNC   WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif

// TYPES ===========================================================================================================

// STATIC VARIABLES ================================================================================================

// FUNCTION IMPLEMENTATION =========================================================================================

int MAIN_FUNC {
    
    {
        // The core version of the logger should save the logger calls in a log-message-buffer variable
        // until a real logger is bound (plugin),
        // that logger then is responsible for loading the saved log messages and formatting them.
        
        GLT::plugin_manager::discover_plugins(GLT::util::get_executable_path() / "plugin");
        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::pre_engine_init);
        // Need to check if a real logger was bound,
        //     if yes - everything is ok
        //     if not - frow away the log-message-buffer and unbind the core logger function binding
        
        GLT::crash_handler::attach();
        GLT::logger::init("[$B$T:$J$E] [$B$L$X $Q - $I:$P:$G$E] $C$Z", true, GLT::util::get_executable_path() / "logs",
            "gluttony.log", true);
        GLT::logger::set_buffer_threshold(GLT::logger::severity::warn);
        GLT::logger::register_label_for_thread("main");
        GLT::crash_handler::subscribe(GLT::logger::shutdown);
    }

    {
        GLT::plugin_manager::discover_plugins(GLT::util::get_executable_path() / GLT::PLUGIN_DIR_NAME);
        // GLT::application app{ARGC, ARGV};
        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::post_engine_init);
        // app.run();
    }

    {
        GLT::logger::shutdown();
        GLT::crash_handler::detach();
        GLT::plugin_manager::shutdown();
    }

    return EXIT_SUCCESS;
}

#undef MAIN_FUNC

// CLASS IMPLEMENTATION ============================================================================================

// CLASS PUBLIC ====================================================================================================

// CLASS PROTECTED =================================================================================================

// CLASS PRIVATE ===================================================================================================
