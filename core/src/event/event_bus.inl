#pragma once

#include "util/pch.h"
#include "event_bus.h"


// FORWARD DECLARATIONS ================================================================================================

namespace GLT::event_bus {

    // CONSTANTS =======================================================================================================

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    struct subscription_entry {
        handle                                      id;
        std::function<void(GLT::event&)>            callback;
    };

    // STATIC VARIABLES ================================================================================================

    // Per‑type lists of subscribers.
    std::unordered_map<std::type_index, std::list<subscription_entry>>                                  s_subscribers;

    // Quick lookup for unsubscription: handle -> (type_index, list iterator)
    std::unordered_map<handle, std::pair<std::type_index, std::list<subscription_entry>::iterator>>     s_handle_map;
    
    std::atomic<handle>                                                                                 s_next_handle{1};

    // TEMPLATE IMPLEMENTATION =========================================================================================

    template<event_type T>
    std::function<void(GLT::event&)> make_wrapper(event_handler_fn<T> handler) {

        return [handler = std::move(handler)](GLT::event& event) {
            handler(static_cast<T&>(event));        // The bus only calls this when the type already matches, so the cast is safe.
        };
    }


    template<event_type T>
    [[nodiscard]] handle subscribe(event_handler_fn<T> handler) {

        auto id = s_next_handle.fetch_add(1, std::memory_order_relaxed);
        auto wrapper = make_wrapper<T>(std::move(handler));
        auto& list = s_subscribers[std::type_index(typeid(T))];
        auto it = list.emplace(list.end(), id, std::move(wrapper));
        s_handle_map[id] = {std::type_index(typeid(T)), it};
        return id;
    }


    inline void unsubscribe(handle id) {
        
        auto map_it = s_handle_map.find(id);
        if (map_it == s_handle_map.end()) return;

        auto& [type_idx, list_it] = map_it->second;
        auto sub_it = s_subscribers.find(type_idx);
        if (sub_it != s_subscribers.end()) {
            sub_it->second.erase(list_it);
            if (sub_it->second.empty())
                s_subscribers.erase(sub_it);
        }
        s_handle_map.erase(map_it);
    }


    template<event_type T>
    inline void post(T& event) {
        
        auto it = s_subscribers.find(std::type_index(typeid(event)));
        if (it == s_subscribers.end()) return;

        // Copy the callbacks to allow safe mutation during iteration.
        std::vector<std::function<void(GLT::event&)>> callbacks;
        callbacks.reserve(it->second.size());
        for (const auto& entry : it->second)
            callbacks.push_back(entry.callback);

        // Fire each callback.
        for (auto& loc_callback : callbacks)
            loc_callback(event);
    }

    // TEMPLATE CLASS IMPLEMENTATION ===================================================================================

    // TEMPLATE CLASS PUBLIC ===========================================================================================

    // TEMPLATE CLASS PROTECTED ========================================================================================

    // TEMPLATE CLASS PRIVATE ==========================================================================================

}
