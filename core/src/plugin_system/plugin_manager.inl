#pragma once

#include "util/pch.h"
#include "plugin_manager.h"


// FORWARD DECLARATIONS ================================================================================================

namespace GLT::plugin_manager {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // TEMPLATE IMPLEMENTATION =========================================================================================

    template<typename T>
    FORCE_INLINE_R ref<T> get_plugin_ref(const std::string& name) {

        return std::dynamic_pointer_cast<T>(get_plugin_base(name));
    }


    template<typename T>
    FORCE_INLINE_R ref<T> get_plugin_ref(const targeted_interface targeted) {

        return std::dynamic_pointer_cast<T>(get_plugin_base(targeted));
    }


    // Weak reference with automatic downcast
    template<typename T>
    FORCE_INLINE_R weak_ref<T> get_plugin(const std::string& name) {

        return std::dynamic_pointer_cast<T>(get_plugin_base(name));
    }


    template<typename T>
    FORCE_INLINE_R weak_ref<T> get_plugin(const targeted_interface targeted) {

        return std::dynamic_pointer_cast<T>(get_plugin_base(targeted));
    }

    // TEMPLATE CLASS IMPLEMENTATION ===================================================================================

    // TEMPLATE CLASS PUBLIC ===========================================================================================

    // TEMPLATE CLASS PROTECTED ========================================================================================

    // TEMPLATE CLASS PRIVATE ==========================================================================================

}
