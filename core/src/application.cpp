
#include "util/pch.h"
#include "application.h"

#include "event/event_bus.h"
#include "event/application_event.h"
#include "plugin_system/plugin_manager.h"
#include "plugin_system/i_window_plugin.h"


// FORWARD DECLARATIONS ================================================================================================

namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION IMPLEMENTATION =========================================================================================

    // CLASS IMPLEMENTATION ============================================================================================

    application::application(int argc, char* argv[]) {

        plugin_manager::load_plugins(plugin_manager::load_phase::post_platform_file_init);
        mp_window = plugin_manager::get_plugin_ref<platform::i_window_plugin>(plugin_manager::targeted_interface::window);
        ASSERT(mp_window, "", "No window plugin loaded - cannot create application window.")

        platform::window_attributes attrs;
        attrs.title = "Gluttony";
        attrs.width = 1920;
        attrs.height = 1080;
        attrs.vsync = false;
        mp_window->create(attrs);       // actually create the native window

        plugin_manager::load_plugins(plugin_manager::load_phase::post_window_creation);
        

        set_target_fps(30);             // DEBUG-ONLY - TODO: load from config
        const auto unused_handle = event_bus::subscribe<window_close_event>(
            [this](window_close_event& event) { m_running = false; });

        LOG_INIT
        plugin_manager::load_plugins(plugin_manager::load_phase::post_engine_init);
    }


    application::~application() {

        plugin_manager::load_plugins(plugin_manager::load_phase::pre_shutdown);

        mp_window->destroy();
        mp_window.reset();

        plugin_manager::load_plugins(plugin_manager::load_phase::post_shutdown);
        LOG_SHUTDOWN
    }

    // CLASS PUBLIC ====================================================================================================

    void application::run() {

        mp_window->show(true);       // show window now
        while (m_running) {

            mp_window->poll_events();        // update internal state

            // ... render, update ...
            
            m_fps_controller.limit();
        }
    }


    void application::set_target_fps(const f32 fps) {

        const u64 time = (1 / fps) * 1000000;
        m_fps_controller.set_target_interval_duration(std::chrono::microseconds(time));
    }

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

}
