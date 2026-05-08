
#include "util/pch.h"

#include <dlfcn.h>

#include "plugin_manager.h"

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::plugin_manager {

    // CONSTANTS =======================================================================================================

    #if defined(PLATFORM_LINUX)
        #define DYNAMIC_LIB_EXTENTION           ".so"
    #else
        #define DYNAMIC_LIB_EXTENTION           ".dll"
    #endif

    // MACROS ==========================================================================================================

    // TYPES ===========================================================================================================

    // Internal handle for a loaded plugin.
    struct plugin_handle {
        std::string                             name;
        std::filesystem::path                   path;
        void*                                   module_handle = nullptr;
        std::shared_ptr<i_plugin>               instance;   // uses custom deleter
        load_phase                              phase;
        std::vector<std::string>                dependencies;
    };


    // TODO: add descriptive comment
    struct discovered_info {
        std::filesystem::path                   path;
        std::string                             name;
        load_phase                              phase;
        std::vector<std::string>                dependencies;
    };
    

    // C‑style function signatures exported by plugins.
    using create_plugin_func = i_plugin* (*)();
    

    // TODO: add descriptive comment
    using destroy_plugin_func = void (*)(i_plugin*);

    
    // Optional descriptor function: if present, it's called after dlopen to get metadata.
    using descriptor_func = const plugin_descriptor* (*)();

    // STATIC VARIABLES ================================================================================================

    static bool                                 s_shutdown = false;
    static std::vector<plugin_handle>           s_loaded_plugins {};
    static std::vector<discovered_info>         s_discovered {};

    // HELPER FUNCTIONS ===============================================================================================

    // Load a plugin described by `info`, resolve symbols, create instance, call on_load().
    static bool load_single(const discovered_info& info) {

        // Check if already loaded (by name).
        auto it = std::find_if(s_loaded_plugins.begin(), s_loaded_plugins.end(), [&](const plugin_handle& h) { return h.name == info.name; });

        if (it != s_loaded_plugins.end()) {
            return true; // already loaded
        }

        // Open library.
        void* handle = dlopen(info.path.c_str(), RTLD_NOW);
        if (!handle) {
            // Log error and fail.
            fprintf(stderr, "Plugin load failed: %s\n", dlerror());
            return false;
        }

        // Get factory functions.
        auto create = reinterpret_cast<create_plugin_func>(dlsym(handle, "create_plugin"));
        auto destroy = reinterpret_cast<destroy_plugin_func>(dlsym(handle, "destroy_plugin"));
        if (!create || !destroy) {
            fprintf(stderr, "Plugin '%s' missing required symbols.\n", info.name.c_str());
            dlclose(handle);
            return false;
        }

        // Create the plugin instance.
        i_plugin* raw_instance = create();
        if (!raw_instance) {
            dlclose(handle);
            return false;
        }

        // Wrap in shared_ptr with a custom deleter that calls destroy_plugin,
        // then (if the shared_ptr is the last owner) dlclose.
        // We need to capture the handle and keep it alive as long as the instance exists.
        struct deleter {
            void operator()(i_plugin* p) const {
                destroy_function(p);
                if (handle) dlclose(handle);
            }
            destroy_plugin_func destroy_function;
            void* handle;
        };
        auto instance = std::shared_ptr<i_plugin>(raw_instance, deleter{ destroy, handle });

        // Call the startup hook.
        instance->on_load();

        // Store the handle.
        s_loaded_plugins.push_back(plugin_handle{
            info.name,
            info.path,
            handle,
            instance,
            info.phase,
            info.dependencies
        });

        return true;
    }

    
    // Checks if all dependencies (by name) are already loaded.
    static bool dependencies_satisfied(const discovered_info& info) {

        for (const auto& dep : info.dependencies) {
            bool found = false;
            for (const auto& loaded : s_loaded_plugins) {
                if (loaded.name == dep) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        return true;
    }

    // CLASS/FUNCTION IMPLEMENTATION ==================================================================================

    // PUBLIC API =====================================================================================================

    void discover_plugins(const std::filesystem::path& pluginDir) {

        if (!std::filesystem::exists(pluginDir)) return;
        s_discovered.clear();
        for (const auto& entry : std::filesystem::recursive_directory_iterator(pluginDir)) {

            if (!entry.is_regular_file()) continue;
            
            const auto& path = entry.path();
            if (path.extension() != DYNAMIC_LIB_EXTENTION) continue;

            // Temporarily load the library to get the descriptor.
            void* handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "dlopen failed, %s\n", dlerror());
                continue;
            }

            auto desc_fn = reinterpret_cast<descriptor_func>(dlsym(handle, "gluttony_descriptor"));
            if (!desc_fn) {
                fprintf(stderr, "dlsym failed, %s\n", dlerror());
                dlclose(handle);
                continue; // descriptor is mandatory
            }

            const plugin_descriptor* desc = desc_fn();
            if (!desc) {
                dlclose(handle);
                continue;
            }

            // Copy out the data before closing the library.
            discovered_info info {
                .path = path,
                .name = desc->name ? std::string(desc->name) : path.stem().string(),
                .phase = desc->phase,
            };
            info.dependencies.reserve(desc->dependency_count);
            for (int i = 0; i < desc->dependency_count; ++i) {
                if (desc->dependency_names && desc->dependency_names[i])
                    info.dependencies.emplace_back(desc->dependency_names[i]);
            }

            s_discovered.push_back(std::move(info));
            dlclose(handle); // done with this library for now
        }
    }

    
    void load_plugins(const load_phase currentPhase) {

        if (s_shutdown) return;

        // Filter discovered plugins by phase and not yet loaded.
        std::vector<discovered_info> pending;
        for (const auto& plugin : s_discovered) {
            if (plugin.phase != currentPhase) continue;
            // Check if already loaded (shouldn't be, but safe).
            if (std::any_of(s_loaded_plugins.begin(), s_loaded_plugins.end(),
                    [&](const plugin_handle& h) { return h.name == plugin.name; })) {
                continue;
            }
            pending.push_back(plugin);
        }

        // Repeatedly load plugins whose dependencies are satisfied.
        bool progress = true;
        while (!pending.empty() && progress) {
            progress = false;
            for (auto it = pending.begin(); it != pending.end(); ) {
                if (dependencies_satisfied(*it)) {
                    if (load_single(*it)) {
                        it = pending.erase(it);
                        progress = true;
                    } else {
                        // load failed, remove from pending to avoid infinite loop, but log.
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }

        if (!pending.empty()) {
            // Some plugins could not be loaded due to missing dependencies.
            for (const auto& p : pending) {
                fprintf(stderr, "Plugin '%s' could not be loaded: unsatisfied dependencies or load error.\n",
                        p.name.c_str());
            }
        }
    }


    void shutdown() {

        if (s_shutdown) return;
        s_shutdown = true;

        // Unload in reverse order to respect dependencies.
        for (auto it = s_loaded_plugins.rbegin(); it != s_loaded_plugins.rend(); ++it) {
            if (it->instance) {
                it->instance->on_unload();
                // The shared_ptr will call its custom deleter, which calls destroy_plugin and dlclose.
                it->instance.reset(); // triggers deletion and library close
            } else if (it->module_handle) {
                dlclose(it->module_handle);
            }
        }
        s_loaded_plugins.clear();
        s_discovered.clear(); // no longer needed
    }


    std::weak_ptr<i_plugin> get_plugin(const std::string& name) {

        for (auto& h : s_loaded_plugins) {
            if (h.name == name && h.instance) {
                return std::weak_ptr<i_plugin>(h.instance);
            }
        }
        return {};
    }

}
