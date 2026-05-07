
#pragma once



// FORWARD DECLARATIONS ================================================================================================

namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

    class i_plugin {
    public:

        virtual ~i_plugin() = default;

        // Called when the module is loaded and ready to hook into the engine.
        // Use this to register systems, bind events, extend menus, etc.
        virtual void on_load() = 0;

        // Called when the engine shuts down or the plugin is unloaded.
        // Clean up everything you registered in onLoad().
        virtual void on_unload() = 0;

        // human‑readable name (can also come from the descriptor)
        virtual const char* get_name() const = 0;

    };

}
