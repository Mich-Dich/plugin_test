
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

        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::post_platform_file_init);
        auto weak_win = GLT::plugin_manager::get_plugin(plugin_manager::targeted_interface::window);
        if (!weak_win.expired()) {

            auto p_weak_win = weak_win.lock();
            mp_window = std::static_pointer_cast<GLT::platform::i_window_plugin>(p_weak_win);
            if (!mp_window.expired()) {
    
                auto p_window = mp_window.lock();
                GLT::platform::window_attributes attributes;
                attributes.title = "Gluttony";          // TODO: load from config, load project name prefixed with Gluttony
                attributes.width = 1920;                // TODO: load from config
                attributes.height = 1080;               // TODO: load from config
    
                p_window->create(attributes);
            }
        }

        set_target_fps(30);         // DEBUG-ONLY - TODO: load from config
        const auto unused_handle = GLT::event_bus::subscribe<GLT::window_close_event>([this](GLT::window_close_event& event) {
            m_running = false;
        });

        LOG_INIT
        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::post_engine_init);
    }


    application::~application() {

        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::pre_shutdown);

        if (!mp_window.expired()) {

            auto p_window = mp_window.lock();
            p_window->destroy();
        }

        GLT::plugin_manager::load_plugins(GLT::plugin_manager::load_phase::post_shutdown);
        LOG_SHUTDOWN
    }

    // CLASS PUBLIC ====================================================================================================

    void application::run() {

        if (!mp_window.expired())
            mp_window.lock()->show(true);       // show window now

        while (m_running) {

            if (!mp_window.expired())
                mp_window.lock()->poll_events();        // update internal state

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
