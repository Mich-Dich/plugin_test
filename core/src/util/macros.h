
#pragma once


// FORWARD DECLARATIONS ================================================================================================

namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    #if defined(__GNUC__)

        #define IGNORE_UNUSED_VARIABLE_START            _Pragma("GCC diagnostic push")                                  \
                                                        _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")

        #define IGNORE_UNUSED_VARIABLE_STOP             _Pragma("GCC diagnostic pop")

    #elif defined(_MSC_VER)

        #define IGNORE_UNUSED_VARIABLE_START            __pragma(warning(push, 0))                                      \
                                                        __pragma(warning(disable: 4189))

        #define IGNORE_UNUSED_VARIABLE_STOP             __pragma(warning(pop))

    #else

        #define IGNORE_UNUSED_VARIABLE_START
        #define IGNORE_UNUSED_VARIABLE_STOP

    #endif

    // platform dependant ----------------------------------------------------------------------------------------------

    #if defined(PLATFORM_LINUX)

        #define CORE_API                                __attribute__((visibility("default")))

        #define DEBUG_BREAK()                           __builtin_trap()

        #ifdef CDECL
            #undef CDECL
        #endif

        // Function type macros.
        // Functions with variable arguments (not directly supported in C++)
        #define VARARGS

        // Standard C function (default in C++)
        #define CDECL

        // Standard calling convention (not directly supported in C++)
        #define STDCALL

        // Force code to be inline
        #define FORCE_INLINE                            inline __attribute__((always_inline))

        // Force code to NOT be inline
        #define FORCE_NOINLINE                          __attribute__((noinline))

        // Indicate that the function never returns nullptr.
        #define FUNCTION_NON_NULL_RETURN                __attribute__((returns_nonnull))

        // Optimization macros.
        #if defined(__clang__)
            #define PRAGMA_DISABLE_OPTIMIZATION         _Pragma("clang optimize off")
            #define PRAGMA_ENABLE_OPTIMIZATION          _Pragma("clang optimize on")
        #else
            #define PRAGMA_DISABLE_OPTIMIZATION         _Pragma("GCC optimize off")
            #define PRAGMA_ENABLE_OPTIMIZATION          _Pragma("GCC optimize on")
        #endif

    #elif defined(PLATFORM_WINDOWS)
        #error Windows not supported yet (maybe in the future)
    #endif

    // copy/move operations --------------------------------------------------------------------------------------------

    #define DELETE_COPY_CONSTRUCTOR(name)                                                                   \
        name(const name&) = delete;                                                                         \
        name& operator=(const name&) = delete;

    #define DELETE_MOVE_CONSTRUCTOR(name)                                                                   \
        name(name&&) = delete;                                                                              \
        name& operator=(name&&) = delete;

    #define DELETE_COPY_AND_MOVE_CONSTRUCTOR(name)                                                          \
        DELETE_COPY_CONSTRUCTOR(name)                                                                       \
        DELETE_MOVE_CONSTRUCTOR(name)

    #define DEFAULT_CONSTRUCTORS(name)                                                                      \
        name() = default;													                                \
        name(const name&) = default;

    #define DEFAULT_COPY_CONSTRUCTOR(name)							                                        \
        name(const name& other) = default;

    // getters ---------------------------------------------------------------------------------------------------------

    #define DEFAULT_GETTER(type, name)				        [[nodiscard]] FORCE_INLINE type get_##name() { return m_##name; }
    #define DEFAULT_GETTER_P(type, name)				    [[nodiscard]] FORCE_INLINE type* get_##name() { return m_##name.get(); }
    #define DEFAULT_GETTER_REF(type, name)			        [[nodiscard]] FORCE_INLINE type& get_##name##_ref() { return m_##name; }
    #define DEFAULT_GETTER_C(type, name)			        [[nodiscard]] FORCE_INLINE type get_##name() const { return m_##name; }
    #define DEFAULT_GETTER_CC(type, name)			        [[nodiscard]] FORCE_INLINE const type get_##name() const { return m_##name; }
    #define DEFAULT_GETTER_POINTER(type, name)		        [[nodiscard]] FORCE_INLINE type* get_##name##_pointer() { return &m_##name; }

    #define DEFAULT_GETTERS(type, name)				        DEFAULT_GETTER(type, name)					    \
                                                            DEFAULT_GETTER_REF(type, name)					\
                                                            DEFAULT_GETTER_POINTER(type, name)

    #define DEFAULT_GETTERS_C(type, name)			        DEFAULT_GETTER_C(type, name)			        \
                                                            DEFAULT_GETTER_POINTER(type, name)

    #define GETTER(type, func_name, var_name)		        [[nodiscard]] FORCE_INLINE type get_##func_name() { return var_name; }
    #define GETTER_C(type, func_name, var_name)		        [[nodiscard]] FORCE_INLINE const type get_##func_name() const { return var_name; }

    // setters ---------------------------------------------------------------------------------------------------------

    #define DEFAULT_SETTER(type, name)				        FORCE_INLINE void set_##name(const type name) { m_##name = name; }
    #define SETTER(type, func_name, var_name)               FORCE_INLINE void set_##func_name(const type value) { var_name = value; }

    // both together ---------------------------------------------------------------------------------------------------

    #define DEFAULT_GETTER_SETTER(type, name)				DEFAULT_GETTER(type, name)				        \
                                                            DEFAULT_SETTER(type, name)

    #define DEFAULT_GETTER_SETTER_C(type, name)				DEFAULT_GETTER_C(type, name)			        \
                                                            DEFAULT_SETTER(type, name)

    #define DEFAULT_GETTER_SETTER_ALL(type, name)			DEFAULT_SETTER(type, name)				        \
                                                            DEFAULT_GETTER(type, name)				        \
                                                            DEFAULT_GETTER_POINTER(type, name)

    #define GETTER_SETTER(type, func_name, var_name)		GETTER(type, func_name, var_name)		        \
                                                            SETTER(type, func_name, var_name)

    #define GETTER_SETTER_C(type, func_name, var_name)		GETTER_C(type, func_name, var_name)	            \
                                                            SETTER(type, func_name, var_name)

    // util ------------------------------------------------------------------------------------------------------------

    #define BIT(x)											(1 << x)

    #define BIND_FUNCTION(x)								std::bind(&x, this, std::placeholders::_1)

    #define ARRAY_SIZE(arr)                                 (sizeof(arr) / sizeof((arr)[0]))

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

}
