
#pragma once

#include "util/timing/interval_controller.h"

// FORWARD DECLARATIONS ================================================================================================

namespace GLT::platform {
    class i_window_plugin;
}


namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

    class application {
    public:
        
        application(int argc, char* argv[]);
        ~application();

        void run();
        
        void set_target_fps(const f32 fps);

    private:
        
        version                                         m_version{};
        ref<GLT::platform::i_window_plugin>             mp_window{};
        bool                                            m_running = true;
        util::interval_controller                       m_fps_controller{};

    };

}
