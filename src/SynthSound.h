#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

struct SynthSound : public juce::SynthesiserSound {
    bool appliesToNote (int) override   { return true; }
    bool appliesToChannel (int) override{ return true; }
};
