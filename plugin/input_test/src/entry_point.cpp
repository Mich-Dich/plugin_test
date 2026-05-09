
#include <iostream>
#include <plugin_system/plugin_interface.h>

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::input_test {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    static const char* dependencies[] = { "logger" };
    static GLT::plugin_manager::plugin_descriptor descriptor = {
        .name                   = GLT_MODULE_NAME,
        .phase                  = GLT::plugin_manager::load_phase::pre_engine_init,
        .dependency_count       = ARRAY_SIZE(dependencies),
        .dependency_names       = dependencies,
    };

    // FUNCTION IMPLEMENTATION =========================================================================================

    // CLASS IMPLEMENTATION ============================================================================================

    // CLASS PUBLIC ====================================================================================================

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

    class plugin : public GLT::plugin_manager::i_plugin {
    public:

        void            on_load() override          { LOG_LOADED };
        
        void            on_unload() override        { LOG_UNLOADED };

        PLUGIN_GET_NAME

    };

}

EXPORT_PLUGIN_CLASS(GLT::input_test::plugin, GLT::input_test::descriptor)
