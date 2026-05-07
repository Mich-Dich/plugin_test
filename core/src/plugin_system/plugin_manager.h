#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// FORWARD DECLARATIONS ================================================================================================

namespace GLT {
    class i_plugin;
}

namespace GLT::plugin_manager {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    enum class load_phase {
        // -- Earliest possible (nothing is ready) --
        pre_engine_init = 0,          // Before even core logs, config, or memory.

        // -- Core foundation --
        post_config_init,             // Config files and command line loaded.
        post_platform_file_init,      // Virtual file system established.

        // -- Platform window --
        post_window_creation,         // Application window exists, but not yet shown.

        // -- Rendering chain --
        post_graphics_api_selection,  // RHI/D3D/Vulkan backend chosen, device not created.
        post_render_device_init,      // GPU device, swapchain, primary render targets ready.

        // -- Other engine subsystems --
        post_input_init,              // Input devices and bindings ready.
        post_audio_init,              // Audio engine initialised.
        post_physics_init,            // Physics world and collision ready.
        post_networking_init,         // Sockets, replication subsystem up.
        post_ui_init,                 // Slate or similar UI framework ready.

        // -- Engine hub --
        post_engine_init,             // All mandatory engine subsystems are online.
                                    // UObject system, asset registry, etc. available.

        // -- World and gameplay --
        post_world_loaded,            // Initial world/level loaded (not yet ticking).
        post_gameplay_init,           // GameInstance, GameMode, GameState created.

        // -- Editor (if applicable) --
        post_editor_init,             // Editor‑specific systems (menus, toolkits) ready.

        // -- Fully operational --
        ready,                        // Engine is about to enter main loop / start ticking.

        // -- Shutdown (reverse order) --
        pre_shutdown,                 // Engine stopping, but everything still valid.
        post_shutdown                 // After all subsystems have shut down.
    };

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // Scans the given directory for plugin shared libraries (*.so, *.dll, *.dylib)
    // and their corresponding descriptor files (plugin.json). Populates the internal
    // discovery list; actual loading happens later in load_plugins().
    void discover_plugins(const std::filesystem::path& pluginDir);

    // Loads all discovered plugins that match the requested phase, respecting
    // inter‑plugin dependencies. Calls on_load() on each loaded plugin.
    void load_plugins(load_phase currentPhase);

    // Gracefully unloads all plugins in reverse dependency order, calling
    // on_unload() on each, then releasing the shared libraries.
    void shutdown();

    // Returns a weak pointer to a loaded plugin by name.
    // If the plugin isn't loaded, the returned pointer will be empty.
    std::weak_ptr<i_plugin> get_plugin(const std::string& name);

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

}