


rm -rf build/; cmake -S . -B build -DCMAKE_BUILD_TYPE=debug && cmake --build build --parallel














# Logger Core implementation:
The core version of the logger should save the logger calls in a log-message-buffer variable until a real logger is bound (plugin), that logger then is responsible for loading the saved log messages and formatting them.

Need to somehow check if the logger was bound,
    if yes - everything is ok
    if not - frow away the log-message-buffer and unbind the core logger function binding, should point to nothing
