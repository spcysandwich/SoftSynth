#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

enum class Wave : int { Sine = 0, Saw = 1, Square = 2 };

struct SimpleOsc {
    void prepare(double sampleRate) { sr = sampleRate; reset(); }
    void reset() { phase = 0.0; }
    void setFrequency(double f) { freq = f; inc = twoPi * freq / sr; }
    void setWave(Wave w) { wave = w; }

    float process() {
        float s = 0.0f;

        switch (wave) {
            case Wave::Sine:
                s = std::sin(phase);
                break;

            case Wave::Saw:
                // naive sawtooth from -1 to 1
                s = static_cast<float>((2.0 * phase / twoPi) - 1.0);
                break;

            case Wave::Square:
                s = (phase < M_PI) ? 1.0f : -1.0f;
                break;
        }

        phase += inc;
        if (phase >= twoPi) phase -= twoPi;

        return s;
    }

private:
    double sr = 48000.0;
    double freq = 440.0;
    double phase = 0.0;
    double inc = 0.0;

    static constexpr double twoPi = 2.0 * M_PI;
    Wave wave = Wave::Sine;
};
// More oscillators can be added here (e.g., FM, Wavetable, etc.)