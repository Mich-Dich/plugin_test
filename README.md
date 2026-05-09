


rm -rf build/; cmake -S . -B build -DCMAKE_BUILD_TYPE=debug && cmake --build build --parallel





# Logger Core implementation:
The core version of the logger should save the logger calls in a log-message-buffer variable until a real logger is bound (plugin), that logger then is responsible for loading the saved log messages and formatting them.

Need to somehow check if the logger was bound,
    if yes - everything is ok
    if not - frow away the log-message-buffer and unbind the core logger function binding, should point to nothing










I still get this:

[TRACE] First possible point for logging, before the logger plugin was loaded
Plugin load failed: /home/mich/playground/plugin_test/build/bin/debug/plugin/logger_plugin/liblogger_plugin.so: undefined symbol: _ZN3GLT6logger24install_logger_functionsERKNS0_16logger_functionsE
[input_test] loaded
Plugin load failed: /home/mich/playground/plugin_test/build/bin/debug/plugin/logger_plugin/liblogger_plugin.so: undefined symbol: _ZN3GLT6logger24install_logger_functionsERKNS0_16logger_functionsE
Plugin 'logger_plugin' could not be loaded: unsatisfied dependencies or load error.
[TRACE] Right after loading the logger plugin
[input_test] unloaded

The error is there twice because the loop assumes a dependency error and attempts it twice













// Near the top of logger.h, or use your project's existing export macro
#if defined(_WIN32) || defined(__CYGWIN__)
  #define CORE_API __declspec(dllexport)
#else
  #define CORE_API __attribute__((visibility("default")))
#endif

// ...

// --- inside namespace GLT::logger ---
CORE_API void install_logger_functions(const logger_functions& funcs);
