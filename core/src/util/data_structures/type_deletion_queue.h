#pragma once

#include <typeindex>
#include <mutex>
#include <deque>
#include <vector>
#include <functional>

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::util {

	// CONSTANTS =======================================================================================================

	// MACROS ==========================================================================================================

	// TYPES ===========================================================================================================

	// STATIC VARIABLES ================================================================================================

	// FUNCTION DECLARATION ============================================================================================

	// TEMPLATE DECLARATION ============================================================================================

	// CLASS DECLARATION ===============================================================================================

    // @brief An extended version of a deletion queue that supports both function callbacks and typed pointers.
    //        Allows pushing pointers (smaller, faster storage) and defining custom cleanup logic in derived classes.
    //        Designed to defer deletion operations and execute them in reverse order of addition when flushed.
    //        THREAD-SAFE: All operations are protected by a mutex.
	class type_deletion_queue {
	public:

		type_deletion_queue() = default;
		~type_deletion_queue() = default;

		// @brief Adds a typed pointer to the deletion queue for deferred cleanup.
		//        The pointer is stored with its type information for type-safe deletion.
		//        Actual cleanup logic must be defined in the derived class's flush_pointer() method.
		//        THREAD-SAFE: Protected by mutex.
		// @tparam T The type of the pointer being stored.
		// @param [pointer] The pointer to be deleted when the queue is flushed.
		template<typename T>
		void push_pointer(T* pointer) {
            std::lock_guard<std::mutex> lock(m_mutex);
			m_pointers.push_back(std::pair<std::type_index, void*>{std::type_index(typeid(T*)), pointer});
        }

		// @brief Adds a function callback to the deletion queue for deferred execution.
		//        The function will be executed when the queue is flushed, typically for cleanup operations.
		//        THREAD-SAFE: Protected by mutex.
		// @param [function] The function to execute when the queue is flushed (moved into the queue).
		void push_func(std::function<void()>&& function);


		// @brief Executes all pending deletion operations in the queue.
		//        Functions are executed in reverse order of addition (LIFO).
		//        Pointers are processed by calling the virtual flush_pointer() method.
		//        Clears the queue after all operations are completed.
		//        THREAD-SAFE: Protected by mutex. Should be called from main thread.
		void flush();


		// @brief Clears the queue without executing any operations.
		//        Useful when you want to abandon pending deletions.
		//        THREAD-SAFE: Protected by mutex.
		void clear();


		// @brief Gets the number of pending deletion operations.
		//        THREAD-SAFE: Protected by mutex.
		// @return Number of pending operations (functions + pointers).
		size_t size() const;


		// @brief Pure virtual method that defines how to handle pointer deletion for specific types.
		//        Must be implemented by derived classes to provide type-specific cleanup logic.
		//        NOTE: This is called from flush() while mutex is already locked.
		// @param [pointer] A pair containing the type index and void pointer to be deleted.
		virtual void flush_pointer(std::pair<std::type_index, void*> pointer) = 0;

	protected:

		mutable std::mutex 								m_mutex;				// internal mutex for thread safety. Marked as mutable for const access in size().

	private:
		std::deque<std::function<void()>>				m_deletions{};			// Queue of function callbacks for deferred execution
		std::vector<std::pair<std::type_index, void*>>	m_pointers{};			// Collection of typed pointers awaiting deletion
	};

}