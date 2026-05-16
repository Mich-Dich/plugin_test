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


    // // Returns a weak pointer to a loaded plugin by name.
    // // If the plugin isn't loaded, the returned pointer will be empty.
    // [[nodiscard]] weak_ref<i_plugin> get_plugin(const std::string& name);

    
    // [[nodiscard]] weak_ref<i_plugin> get_plugin(const targeted_interface targeted);


    // CAUTION, this will create an owning ref, only use when you know what you are doing
    [[nodiscard]] ref<i_plugin> get_plugin_base(const std::string& name);


    // CAUTION, this will create an owning ref, only use when you know what you are doing
    [[nodiscard]] ref<i_plugin> get_plugin_base(const targeted_interface targeted);


    // CAUTION, this will create an owning ref, only use when you know what you are doing
    template<typename T = i_plugin>
    FORCE_INLINE_R ref<T> get_plugin_ref(const std::string& name);


    // CAUTION, this will create an owning ref, only use when you know what you are doing
    template<typename T = i_plugin>
    FORCE_INLINE_R ref<T> get_plugin_ref(const targeted_interface targeted);


    // Weak reference with automatic downcast
    template<typename T = i_plugin>
    FORCE_INLINE_R weak_ref<T> get_plugin(const std::string& name);


    template<typename T = i_plugin>
    FORCE_INLINE_R weak_ref<T> get_plugin(const targeted_interface targeted);

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

}

#include "plugin_manager.inl"
