#include "PluginProcessor.h"

SoftSynthAudioProcessor::SoftSynthAudioProcessor()
: juce::AudioProcessor (BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

void SoftSynthAudioProcessor::prepareToPlay (double sampleRate, int) {
    sr_ = sampleRate;
    updatePhase();
}

bool SoftSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
    return layouts.getMainOutputChannelSet() != juce::AudioChannelSet::disabled();
}

void SoftSynthAudioProcessor::handleMidi (juce::MidiBuffer& midi) {
    for (const auto meta : midi) {
        const auto m = meta.getMessage();
        if (m.isNoteOn()) {
            currentFreq_ = juce::MidiMessage::getMidiNoteInHertz (m.getNoteNumber());
            gate_ = true;
            updatePhase();
        } else if (m.isNoteOff() || m.isAllNotesOff() || m.isAllSoundOff()) {
            gate_ = false;
        }
    }
}

void SoftSynthAudioProcessor::updatePhase() {
    inc_ = juce::MathConstants<double>::twoPi * (double) currentFreq_ / sr_;
}

void SoftSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) {
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    handleMidi(midi);

    auto* L = buffer.getWritePointer(0);
    auto* R = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        const float s = gate_ ? (float) std::sin(phase_) * amp_ : 0.0f;
        phase_ += inc_;
        if (phase_ > juce::MathConstants<double>::twoPi) phase_ -= juce::MathConstants<double>::twoPi;
        L[i] = s; if (R) R[i] = s;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new SoftSynthAudioProcessor();
}
