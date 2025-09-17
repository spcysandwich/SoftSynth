# SoftSynth

SoftSynth is a minimal software synthesiser built with [JUCE](https://juce.com/). It exposes a monophonic sine oscillator as both a VST3 plugin and a standalone application, making it a compact starting point for experimenting with audio DSP and plugin development.

> **Project status:** SoftSynth is intentionally tiny right now—think of it as a learning sandbox rather than a polished instrument. The fundamentals work, but expect a utilitarian UI, a single oscillator voice, and little in the way of modulation or effects. If you're curious about JUCE or audio programming, it's a friendly codebase to explore and extend, and contributions that evolve it into something more full-featured are very welcome.

## Features

- JUCE-based audio processor configured as a MIDI-controlled synth.
- Simple monophonic signal path driven by a sine oscillator with adjustable amplitude.
- Builds as a VST3 plugin and as a standalone desktop app via JUCE's CMake helpers.

## Getting started

### Prerequisites

- CMake 3.21 or newer.
- A C++17 capable compiler toolchain (e.g., Clang, GCC, or MSVC).
- Standard build tools such as Ninja or Make.

### Clone the repository

```bash
git clone --recursive https://github.com/your-user/SoftSynth.git
cd SoftSynth
```

If you cloned without `--recursive`, initialise the JUCE submodule manually:

```bash
git submodule update --init --recursive
```

### Configure and build

Generate the project files and compile the targets:

```bash
cmake -S . -B build
cmake --build build --config Release
```

JUCE writes the compiled binaries to `build/SoftSynth_artefacts/`. From there you can
run the standalone executable or install the generated `SoftSynth.vst3` into your plugin
folder of choice.

## Repository layout

- `src/` – JUCE audio processor, voices, and helper classes.
- `SoftSynth.cpp` – Small sample translation unit that prints a greeting (not used by the JUCE plugin target).
- `CMakeLists.txt` – Project definition using JUCE's CMake API.
- `JUCE/` – JUCE framework sources (added as a Git submodule).

## License

SoftSynth is released under the terms of the [MIT License](LICENSE).
