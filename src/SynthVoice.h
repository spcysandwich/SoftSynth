#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include "SynthSound.h"
#include "Oscillator.h"

class SynthVoice : public juce::SynthesiserVoice {
public:
    SynthVoice() {}

    // called by the processor
    void prepare (double sampleRate, int /*samplesPerBlock*/) {
        sr = sampleRate;
        osc.prepare(sr);
        adsr.setSampleRate(sr);
        reset();
    }

    // pointers to parameters (set by processor)
    struct Params {
        std::atomic<float>* wave = nullptr;   // 0..2
        std::atomic<float>* gain = nullptr;   // 0..1
        std::atomic<float>* atk = nullptr;    // sec
        std::atomic<float>* dec = nullptr;    // sec
        std::atomic<float>* sus = nullptr;    // 0..1
        std::atomic<float>* rel = nullptr;    // sec
    } params;

    void setParamsPointers (Params p) { params = p; }

    bool canPlaySound (juce::SynthesiserSound* s) override {
        return dynamic_cast<SynthSound*>(s) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override {
        const double f = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        osc.setFrequency(f);
        const float waveValue = params.wave ? params.wave->load() : 0.0f;
        osc.setWave (static_cast<Wave> (juce::jlimit (0, 2, (int) std::round (waveValue))));

        juce::ADSR::Parameters ap;
        ap.attack  = params.atk ? std::max (0.0001f, params.atk->load()) : 0.01f;
        ap.decay   = params.dec ? std::max (0.0001f, params.dec->load()) : 0.1f;
        ap.sustain = params.sus ? juce::jlimit (0.0f, 1.0f, params.sus->load()) : 0.8f;
        ap.release = params.rel ? std::max (0.0001f, params.rel->load()) : 0.5f;
        adsr.setParameters(ap);

        const float gain = params.gain ? params.gain->load() : 0.7f;
        level = juce::jlimit (0.0f, 1.0f, gain) * juce::jlimit (0.0f, 1.0f, velocity);
        adsr.noteOn();
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override {
        if (allowTailOff) adsr.noteOff();
        else { adsr.reset(); clearCurrentNote(); }
    }

    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        if (! isVoiceActive())
            return;

        for (int i = 0; i < numSamples; ++i) {
            float sample = osc.process();
            sample *= adsr.getNextSample();
            sample *= level;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                outputBuffer.addSample (channel, startSample + i, sample);
        }

        if (! adsr.isActive())
            clearCurrentNote();
    }

    void reset() {
        osc.reset();
        adsr.reset();
        level = 0.0f;
    }

private:
    double sr = 48000.0;
    SimpleOsc osc;
    juce::ADSR adsr;
    float level = 0.0f;
};
