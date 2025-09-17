#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
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
        osc.setWave( (Wave) (int) (params.wave->load() + 0.5f) );

        juce::ADSR::Parameters ap;
        ap.attack  = std::max(0.0001f, params.atk->load());
        ap.decay   = std::max(0.0001f, params.dec->load());
        ap.sustain = juce::jlimit (0.0f, 1.0f, params.sus->load());
        ap.release = std::max(0.0001f, params.rel->load());
        adsr.setParameters(ap);

        level = juce::jlimit (0.0f, 1.0f, params.gain->load()) * juce::jlimit (0.0f, 1.0f, velocity);
        adsr.noteOn();
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override {
        if (allowTailOff) adsr.noteOff();
        else { adsr.reset(); clearCurrentNote(); }
    }

    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
    void renderNextBlock (juce::AudioBuffer<float>& output, int startSample, int numSamples) override {
        if (! adsr.isActive()) { clearCurrentNote(); return; }
        for (int i = 0; i < numSamples; ++i) {
            float sample = osc.process();
            sample *= adsr.getNextSample();
            sample *= level;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                outputBuffer.addSample(channel, startSample + i, sample);
        }
        }
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
