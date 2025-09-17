cmake_minimum_required(VERSION 3.21)

project(SoftSynth VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# JUCE lives in ./JUCE (submodule)
add_subdirectory(JUCE)

juce_add_plugin(SoftSynth
    COMPANY_NAME "SoftSynth Project"
    BUNDLE_ID com.SoftSynthProject.softsynth
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES Instrument Synth
    FORMATS VST3 Standalone
    PRODUCT_NAME "SoftSynth"
)

target_sources(SoftSynth PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/SynthSound.h
    src/SynthVoice.h
    src/Oscillator.h
)

target_link_libraries(SoftSynth PRIVATE
    juce::juce_audio_processors       # <- required for AudioProcessor
    juce::juce_audio_utils
    juce::juce_audio_basics
    juce::juce_dsp
    juce::juce_core
)
