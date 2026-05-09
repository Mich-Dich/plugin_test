
#include <iostream>
#include <plugin_system/plugin_interface.h>

#include "logger.h"                 // plugin logger implementation
#include "util/io/logger.h"         // core logger implementation

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::logger_plugin {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    static const char* dependencies[] = {  };
    static GLT::plugin_manager::plugin_descriptor descriptor = {
        .name                   = PLUGIN_NAME,
        .phase                  = GLT::plugin_manager::load_phase::pre_engine_init,
        .dependency_count       = 0,
        .dependency_names       = dependencies,
    };

    static const GLT::logger::logger_functions plugin_functions = {
        .init                           = &GLT::logger_plugin::init,
        .shutdown                       = &GLT::logger_plugin::shutdown,
        .log_msg_internal               = &GLT::logger_plugin::log_msg_internal,
        .get_log_file_location          = &GLT::logger_plugin::get_log_file_location,
        .set_format                     = &GLT::logger_plugin::set_format,
        .use_previous_format            = &GLT::logger_plugin::use_previous_format,
        .get_format                     = &GLT::logger_plugin::get_format,
        .set_buffer_threshold           = &GLT::logger_plugin::set_buffer_threshold,
        .set_buffer_size                = &GLT::logger_plugin::set_buffer_size,
        .register_label_for_thread      = &GLT::logger_plugin::register_label_for_thread,
        .unregister_label_for_thread    = &GLT::logger_plugin::unregister_label_for_thread,
    };


    // FUNCTION IMPLEMENTATION =========================================================================================

    // CLASS IMPLEMENTATION ============================================================================================

    // CLASS PUBLIC ====================================================================================================

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

    class plugin : public GLT::plugin_manager::i_plugin {
    public:

        void on_load() override {

            fprintf(stdout, "[%s] loaded\n", PLUGIN_NAME); 
            GLT::logger::install_logger_functions(plugin_functions);
        }
        
        void on_unload() override {

            fprintf(stdout, "[%s] unloaded\n", PLUGIN_NAME); 
        }

        const char* get_name() const override { return PLUGIN_NAME; };

    };

}


extern "C" {

    // Return the descriptor (mandatory for discovery)
    const GLT::plugin_manager::plugin_descriptor* gluttony_descriptor() {
        return &GLT::logger_plugin::descriptor;
    }

    // Factory to create your plugin instance
    GLT::plugin_manager::i_plugin* create_plugin() {
        return new GLT::logger_plugin::plugin();
    }

    // Factory to destroy it (called by the custom deleter)
    void destroy_plugin(GLT::plugin_manager::i_plugin* p) {
        delete p;
    }

}
