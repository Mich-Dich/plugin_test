#pragma once

#include "event.h"

// FORWARD DECLARATIONS ================================================================================================


namespace GLT {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // TEMPLATE DECLARATION ============================================================================================

    // CLASS DECLARATION ===============================================================================================

    class window_resize_event : public event_template<event_type::window_resize, event_category::window> {
    public:
        window_resize_event(u32 width, u32 height) : m_width(width), m_height(height) {}

        DEFAULT_GETTER_C(u32, width);
        DEFAULT_GETTER_C(u32, height);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window resize event [{}, {}]", m_width, m_height);
        }

    private:
        u32 m_width, m_height;
    };


    class window_framebuffer_resize_event : public event_template<event_type::window_framebuffer_resize, event_category::window> {
    public:
        window_framebuffer_resize_event(u32 width, u32 height) : m_width(width), m_height(height) {}

        DEFAULT_GETTER_C(u32, width);
        DEFAULT_GETTER_C(u32, height);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window framebuffer resize event [{}, {}]", m_width, m_height);
        }

    private:
        u32 m_width, m_height;
    };


    class window_focus_event : public event_template<event_type::window_focus, event_category::window> {
    public:
        window_focus_event(bool focused) : m_focused(focused) {}

        DEFAULT_GETTER_C(bool, focused);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window focus event [{}]", m_focused ? "focused" : "unfocused");
        }

    private:
        bool m_focused;
    };


    class window_move_event : public event_template<event_type::window_moved, event_category::window> {
    public:  // Changed from private!
        window_move_event(i32 x, i32 y) : m_x(x), m_y(y) {}

        DEFAULT_GETTER_C(i32, x)
        DEFAULT_GETTER_C(i32, y)

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window moved to [{}, {}]", m_x, m_y);
        }

    private:
        i32 m_x, m_y;
    };


    class window_iconify_event : public event_template<event_type::window_iconify, event_category::window> {
    public:
        window_iconify_event(bool iconified) : m_iconified(iconified) {}

        DEFAULT_GETTER_C(bool, iconified);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window iconify event [{}]", m_iconified ? "iconified" : "restored");
        }

    private:
        bool m_iconified;
    };


    class window_maximize_event : public event_template<event_type::window_maximize, event_category::window> {
    public:
        window_maximize_event(bool maximized) : m_maximized(maximized) {}

        DEFAULT_GETTER_C(bool, maximized);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window maximize event [{}]", m_maximized ? "maximized" : "restored");
        }

    private:
        bool m_maximized;
    };


    class window_content_scale_event : public event_template<event_type::window_content_scale, event_category::window> {
    public:
        window_content_scale_event(f32 x_scale, f32 y_scale) : m_x_scale(x_scale), m_y_scale(y_scale) {}

        DEFAULT_GETTER_C(f32, x_scale);
        DEFAULT_GETTER_C(f32, y_scale);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            return std::format("window content scale event [{:.2f}, {:.2f}]", m_x_scale, m_y_scale);
        }

    private:
        f32 m_x_scale, m_y_scale;
    };


    class window_drop_event : public event_template<event_type::window_drop, event_category::window> {
    public:
        window_drop_event(const std::vector<std::string>& paths) : m_paths(paths) {}

        DEFAULT_GETTER_C(std::vector<std::string>, paths);

        [[nodiscard]] FORCE_INLINE std::string to_string() const override {
            std::string result = "window drop event [";
            for (size_t i = 0; i < m_paths.size(); ++i) {
                result += m_paths[i];
                if (i < m_paths.size() - 1) result += ", ";
            }
            result += "]";
            return result;
        }

    private:
        std::vector<std::string> m_paths;
    };


    class window_close_event        : public event_template<event_type::window_close, event_category::window> { };
    class window_refresh_event      : public event_template<event_type::window_refresh, event_category::window> { };
    class application_refresh_event : public event_template<event_type::app_refresh, event_category::application> { };
    class application_pause_event   : public event_template<event_type::app_pause, event_category::application> { };
    class application_resume_event  : public event_template<event_type::app_resume, event_category::application> { };

}
