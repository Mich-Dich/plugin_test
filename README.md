# Gluttony – my little plugin playground

**A scrappy, work‑in‑progress C++23 application skeleton that loads things as plugins because why not?**

Gluttony is my hobby project: a microkernel‑ish executable that discovers shared libraries at runtime and stitches them together into a running program. Right now it’s mostly a logger with delusions of grandeur, but the blueprint is there for windowing, input, audio, and whatever else I decide to bolt on later. It’s rough around the edges, the code still has training wheels, and you definitely shouldn’t use this for anything serious… but if you enjoy poking at half‑finished C++ experiments, pull up a chair.

> **⚠ This is very much a work in progress.**  
> The core runs, the plugin manager resolves dependencies, and the logger is actually pretty neat, but large parts are still just empty interfaces waiting for an implementation. Expect missing features, occasional jank, and comments that promise things that don’t exist yet. You’ve been warned.

---

## What’s inside the box?

Even in its half‑baked state, Gluttony already has a few moving pieces:

- **Microkernel core** – A tiny executable that discovers plugins (`*.so`) and loads them in the right order based on a *load phase* and dependencies. But unlike traditional Microkernel everything lives in one address space for speed.

- **Plugin system with training wheels**  
  Plugins export a descriptor that says “I’m ready after the render device is up” and “I need the logger”. The manager sorts it all out, calls `on_load` / `on_unload`, and even gives you a nice `get_plugin<T>()` accessor. It’s not bulletproof, but it works for the handful of plugins I’ve thrown at it.

- **Logger (plugin)**  
  Honestly, the logger is the most polished part. It formats messages with a custom string mini‑language (`$T`, `$L`, `$C`, etc.), writes to file and console, buffers low‑severity messages to avoid thrashing the disk, and even runs its own worker thread. Thread labels, runtime format switching, and a “warn” threshold that flushes the buffer – I had fun with this one.

- **Crash handler for a graceful faceplant**  
  Catches signals (Linux) or exceptions (Windows) and runs cleanup functions you’ve registered. The logger subscribes to it automatically, so if the program explodes, you still get the last few log lines.

- **A generic resource cache** (header‑only)  
  Thread‑safe `resource_manager` template with optional background cleanup. It’s there if you need it, but right now nothing uses it. Maybe I’ll plug it into a future asset pipeline.

- **Utility grab bag**  
  Platform helpers (executable path, system time, precision sleep), macro voodoo for getters/setters, copy/move deletion, etc. All the usual boilerplate that makes C++ slightly less painful.

- **CMake build with PCH**  
  Because waiting for compiles is boring. The helper script even prints a pretty list of source files with line counts and sizes – because I like seeing statistics.

---

## Quickstart (if you’re feeling brave)

**Prerequisites**  
- Linux (Windows maybe later, when I buy a new keyboard to smash)  
- C++23 compiler (GCC ≥ 11 or Clang ≥ 13)  
- CMake ≥ 3.21  

```bash
git clone https://github.com/your-username/gluttony.git
cd gluttony
cmake -B build -DCMAKE_BUILD_TYPE=debug
cmake --build build
./build/bin/debug/Gluttony
```

It’ll start up, discover any plugins in the `plugin/` directory, load the ones ready for `pre_engine_init`, fire up the logger, and then probably just sit there because the application loop isn’t written yet. But you’ll get a pretty log file.

---

## How the plugin dance works

1. **Discovery**: The core scans the `plugin/` folder for shared libraries. Each one must export a descriptor function (`gluttony_plugin_descriptor`) that returns a struct with the plugin’s name, required load phase, and dependencies.

* **Dependency resolution**: The manager loads plugins phase by phase. Within a phase, it repeatedly scans the list of candidates and loads any plugin whose dependencies are already present (from an earlier phase or from another plugin that just got loaded in the same phase). It keeps retrying until no more progress can be made. If a plugin still has unmet dependencies after all that, the manager logs a desolate error message and moves on to the next phase. So if plugin `B` needs plugin `A` and both are in the same phase, `A` gets loaded first.

3. **Lifetime**: `create_plugin()` gives the core a shiny new instance, `on_load()` lets it initialise, and later `on_unload()` tells it to clean up. When the core shuts down, plugins are unloaded in reverse order.

The plugin interface (`i_plugin`) is intentionally minimal. Subsystem interfaces (like `i_window_plugin` for a future window) inherit from it, so the core can talk to them without knowing the details.

---

## Logger – the fancy part

I spent way too much time on this. The format string is the heart of it:
So `"$B[$T:$J] $L$E $C$Z"` prints something like `[14:22:05] INFO Hello` in colour.

You can change the format on the fly, register thread names, and set a severity threshold that controls when buffered messages get flushed. It’s asynchonous, so the calling thread just drops a message and runs.

---

## Current TODOs:
- add capability to recompile the core and then static link the plugins
- create a settings system so the core can decide what plugin to use for a given task (eg: glfw_window or sdl_window for the window)


### planned plugins
- Windowing
- memory manager
- audio
- renderer

---

## Why does this exist?

Because I love the idea of a modular engine core that can be extended without recompiling the whole world. And because I enjoy the process of building scaffolding, even if I never finish the cathedral. It’s a learning project, a sandbox, and a gentle reminder that side projects don’t have to be perfect to be fun.

If you find a bug or want to add something, open an issue or a PR – I can’t promise prompt responses, but I’ll be thrilled that someone else looked at the code.

---

*Thanks for stopping by. Now go build something equally half‑baked.*
