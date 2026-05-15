#pragma once

#include "plugin_interface.h"

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::plugin_manager {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // Scans the given directory for plugin shared libraries (*.so, *.dll, *.dylib)
    // and their corresponding descriptor files (plugin.json). Populates the internal
    // discovery list; actual loading happens later in load_plugins().
    void discover_plugins();


    // Loads all discovered plugins that match the requested phase, respecting
    // inter‑plugin dependencies. Calls on_load() on each loaded plugin.
    void load_plugins(load_phase currentPhase);


    // Gracefully unloads all plugins in reverse dependency order, calling
    // on_unload() on each, then releasing the shared libraries.
    void shutdown();


    // Returns a weak pointer to a loaded plugin by name.
    // If the plugin isn't loaded, the returned pointer will be empty.
    [[nodiscard]] std::weak_ptr<i_plugin> get_plugin(const std::string& name);

    
    [[nodiscard]] std::weak_ptr<i_plugin> get_plugin(const targeted_interface targeted);

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

}
