#pragma once

#include "util/pch.h"

// FORWARD DECLARATIONS ================================================================================================


namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

	enum class event_type : u8 {
		none = 0,

        // Window events
        window_close, window_resize, window_framebuffer_resize, window_moved,
        window_focus, window_iconify, window_maximize, window_refresh,
        window_content_scale, window_drop,

        app_refresh, app_pause, app_resume,                         // Application events

        // Input events (summarized as you prefer)
        key_input,                                                  // For keyboard key presses/releases/repeats
        char_input,                                                 // For character input
        mouse,                                                      // For mouse button actions
        mouse_button,                                               // For mouse button actions
        mouse_moved,                                                // Mouse movement
        mouse_scrolled,                                             // Mouse wheel
        cursor_enter                                                // Mouse enter/leave window
	};


	enum class event_category : u8 {
		none            = 0,
		application     = BIT(0),
		window          = BIT(1),
		input			= BIT(2),
		keyboard		= BIT(3),
		mouse			= BIT(4),
		button			= BIT(5)
	};


    constexpr event_category operator|(event_category lhs, event_category rhs)      { return static_cast<event_category>(to_base<event_category>(lhs) | to_base<event_category>(rhs)); }

    constexpr bool operator&(event_category lhs, event_category rhs)                { return to_base<event_category>(lhs) & to_base<event_category>(rhs); }

    constexpr event_category operator^(event_category lhs, event_category rhs)      { return static_cast<event_category>(to_base<event_category>(lhs) ^ to_base<event_category>(rhs)); }


    template<typename T>
    using event_function = bool(*)(T&);

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

    class event {
    public:

        virtual ~event() = default;

        DEFAULT_GETTER_SETTER_C(bool,       handled)

        [[nodiscard]] virtual event_type get_type() const = 0;
        [[nodiscard]] virtual event_category get_category() const = 0;
        [[nodiscard]] virtual std::string to_string() const = 0;

        FORCE_INLINE bool is_in_category(event_category category) const {
            return (category & get_category());
        }

    private:

        bool                                m_handled = false;
    };


    // template derived class
    template<event_type type, event_category category>
    class event_template : public event {
    public:

        static event_type get_static_type()                     { return type; }
        virtual event_type get_type() const override            { return type; }
        virtual event_category get_category() const override    { return category; }
        virtual std::string to_string() const override {        // Default implementation
            return std::format("event [{}]", static_cast<int>(type));
        }
    };


    // Stream operator works with base class
    [[nodiscard]] FORCE_INLINE std::ostream& operator<<(std::ostream& os, const event& e) {
        return os << e.to_string();
    }


    class event_dispatcher {
    public:
        event_dispatcher(event& event) : m_event(event) {}

        template<typename T, typename F>
        bool dispatch(F&& func) {

            static_assert(std::is_invocable_r_v<bool, F, T&>, "Event handler must return bool (true if handled, false if not)");
            if (m_event.get_handled())
                return false;

            if (m_event.get_type() == T::get_static_type()) {
                const bool handled = std::invoke(std::forward<F>(func), static_cast<T&>(m_event));
                m_event.set_handled(handled);
                return handled;
            }
            return false;
        }


        // Convenience method for checking if event is of specific type
        template<typename T>
        bool is_type() const {
            return m_event.get_type() == T::get_static_type();
        }

        // Try to cast event to specific type (returns nullptr if wrong type)
        template<typename T>
        T* try_cast() {
            if (is_type<T>()) {
                return static_cast<T*>(&m_event);
            }
            return nullptr;
        }

    private:

        event&          m_event;

    };

}
