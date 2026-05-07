#include "util/pch.h"

#include "type_deletion_queue.h"

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::util {


    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // STATIC VARIABLES ================================================================================================

    // FUNCTION IMPLEMENTATION =========================================================================================

    // CLASS IMPLEMENTATION ============================================================================================

    // CLASS PUBLIC ====================================================================================================

    void type_deletion_queue::push_func(std::function<void()>&& function) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_deletions.push_back(std::move(function));
    }

    
    void type_deletion_queue::flush() {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Execute functions in reverse order (LIFO)
        for (auto it = m_deletions.rbegin(); it != m_deletions.rend(); ++it) {
            try {
                (*it)();
            } catch (const std::exception& e) {
                // Log error but continue flushing to avoid resource leaks
                LOG(error, "Exception during deletion queue flush: {}", e.what());
            }
        }

        // Process pointers
        for (const auto& entry : m_pointers) {
            try {
                flush_pointer(entry);
            } catch (const std::exception& e) {
                LOG(error, "Exception during pointer flush: {}", e.what());
            }
        }

        // Clear the containers
        m_deletions.clear();
        m_pointers.clear();
    }


    void type_deletion_queue::clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_deletions.clear();
        m_pointers.clear();
    }


    size_t type_deletion_queue::size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_deletions.size() + m_pointers.size();
    }

    // CLASS PROTECTED =================================================================================================

    // CLASS PRIVATE ===================================================================================================

}