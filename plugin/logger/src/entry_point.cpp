
#include <iostream>
#include <plugin_system/plugin_interface.h>

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

    // FUNCTION IMPLEMENTATION =========================================================================================

    // CLASS IMPLEMENTATION ============================================================================================

    // CLASS PUBLIC ====================================================================================================

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

    class plugin : public GLT::plugin_manager::i_plugin {
    public:

        void            on_load() override          { fprintf(stdout, "[%s] loaded\n", PLUGIN_NAME); };
        
        void            on_unload() override        { fprintf(stdout, "[%s] unloaded\n", PLUGIN_NAME); };

        const char*     get_name() const override   { return PLUGIN_NAME; };

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
