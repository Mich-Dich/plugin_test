#include "util/pch.h"
#include "logger.h"
#include "util/system.h"
#include <iomanip>

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::logger {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // ---------- default (fallback) implementations --------------------------------
    static bool default_init(const std::string& /*format*/, bool /*log_to_console*/,
                             const std::filesystem::path& /*log_dir*/,
                             const std::string& /*main_log_file_name*/,
                             bool /*use_append_mode*/) {
        // minimal: do nothing, just return success
        return true;
    }

    static void default_shutdown() {
        // nothing
    }

    static void default_log_msg_internal(severity msg_sev, const char* /*file_name*/,
                                         const char* /*function_name*/, int /*line*/,
                                         std::thread::id /*thread_id*/, std::string message) {
        // fallback: write directly to stderr with a simple layout
        static const char* sev_names[] = {"TRACE","DEBUG","INFO","WARN","ERROR","FATAL"};
        std::cerr << "[" << sev_names[static_cast<int>(msg_sev)] << "] " << message << std::endl;
    }

    static std::filesystem::path default_get_log_file_location() {
        return {};
    }

    static void default_set_format(const std::string&) { }
    static void default_use_previous_format() { }
    static const std::string default_get_format() { return {}; }
    static void default_set_buffer_threshold(severity) { }
    static void default_set_buffer_size(size_t) { }
    static void default_register_label(const std::string&, std::thread::id) { }
    static void default_unregister_label(std::thread::id) { }

    // ---------- global pointer table ----------------------------------------------
    static logger_functions g_logger = {
        default_init,
        default_shutdown,
        default_log_msg_internal,
        default_get_log_file_location,
        default_set_format,
        default_use_previous_format,
        default_get_format,
        default_set_buffer_threshold,
        default_set_buffer_size,
        default_register_label,
        default_unregister_label
    };

    // INTERNAL FUNCTION ===============================================================================================

    // FUNCTION DECLARATION ============================================================================================

    // FUNCTION IMPLEMENTATION =========================================================================================

    void install_plugin_functions(const logger_functions& funcs) {
        g_logger = funcs;   // safe as long as called before any logging threads run
    }

    bool init(const std::string& format, bool log_to_console, const std::filesystem::path log_dir, const std::string& main_log_file_name, bool use_append_mode) {
        return g_logger.init(format, log_to_console, log_dir, main_log_file_name, use_append_mode);
    }

    void shutdown() {
        g_logger.shutdown();
    }

    std::filesystem::path get_log_file_location() {
        return g_logger.get_log_file_location();
    }

    void set_format(const std::string& new_format) {
        g_logger.set_format(new_format);
    }

    void use_previous_format() {
        g_logger.use_previous_format();
    }

    const std::string get_format() {
        return g_logger.get_format();
    }

    void set_buffer_threshold(severity new_threshold) {
        g_logger.set_buffer_threshold(new_threshold);
    }

    void set_buffer_size(size_t new_size) {
        g_logger.set_buffer_size(new_size);
    }

    void register_label_for_thread(const std::string& thread_label, std::thread::id thread_id) {
        g_logger.register_label_for_thread(thread_label, thread_id);
    }

    void unregister_label_for_thread(std::thread::id thread_id) {
        g_logger.unregister_label_for_thread(thread_id);
    }

    void log_msg_internal(severity msg_sev, const char* file_name, const char* function_name, int line, std::thread::id thread_id, std::string message) {
        g_logger.log_msg_internal(msg_sev, file_name, function_name, line, thread_id, std::move(message));
    }

    // CLASS IMPLEMENTATION ============================================================================================

    // CLASS PUBLIC ====================================================================================================

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

}
