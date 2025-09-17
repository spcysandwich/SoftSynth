#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

class SoftSynthAudioProcessor : public juce::AudioProcessor
{
public:
    SoftSynthAudioProcessor();
    ~SoftSynthAudioProcessor() override = default;

    // Core
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Layout
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // UI
    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override
    { return new juce::GenericAudioProcessorEditor (*this); }

    // Misc
    const juce::String getName() const override { return "SoftSynth"; }
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int /*index*/) override { return "Init"; }
    void changeProgramName (int, const juce::String&) override {}

    // State
    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

private:
    // simple mono oscillator state for now
    double sr_ = 44100.0, phase_ = 0.0, inc_ = 0.0;
    float  amp_ = 0.2f, currentFreq_ = 0.0f;
    bool   gate_ = false;

    void handleMidi (juce::MidiBuffer& midi);
    void updatePhase();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftSynthAudioProcessor)
};

// Factory
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter();
