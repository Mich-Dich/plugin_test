
#pragma once

#include "util/io/logger.h"         // expose logger for all plugins

// FORWARD DECLARATIONS ================================================================================================

namespace GLT::plugin_manager {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    #define PLUGIN_GET_NAME         const char* get_name() const override { return GLT_MODULE_NAME; };

    #define EXPORT_PLUGIN_CLASS(plugin_class, descriptor_var)                                                           \
        extern "C" {                                                                                                    \
            const GLT::plugin_manager::plugin_descriptor* gluttony_plugin_descriptor() { return &descriptor_var; }      \
            GLT::plugin_manager::i_plugin* create_plugin() { return new plugin_class(); }                               \
            void destroy_plugin(GLT::plugin_manager::i_plugin* p) { delete p; }                                         \
        }

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


    enum class targeted_interface : u16 {
        none = 0,
        logger,
        window,
        memory_manager,
        configuration,
        input_system,
        virtual_file_system,
        graphics_api,
        render_device,
        renderer_frontend,
        asset_registry,
        resource_cache,
        ecs,
        scene_manager,
        physics,
        audio,
        scripting,
        ui_system,
        editor_core,
        networking,
        online_subsystem,
        custom,                     // needs to be last!
    };


    // Descriptor that every plugin must export.
    struct plugin_descriptor {
        const char*                     name{};
        load_phase                      phase{};                // is a [u16]
        targeted_interface              target{};               // is a [u16]
        int                             dependency_count{};
        const char* const*              dependency_names{};     // array of C‑strings, nullptr if zero
    };

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
