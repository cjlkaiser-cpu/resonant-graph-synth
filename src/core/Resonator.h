#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <cstdint>

namespace rgs {

/**
 * Karplus-Strong extended resonator
 *
 * Physical model of a plucked string with:
 * - Frequency-dependent damping
 * - Adjustable brightness (lowpass cutoff)
 * - Inharmonicity via allpass filter
 */
class Resonator {
public:
    static constexpr int MAX_DELAY = 4096;

    Resonator();

    void prepare(double sampleRate);
    void setFrequency(float freq);
    void setDamping(float damping);      // 0.9 - 0.9999
    void setBrightness(float brightness); // 0 - 1
    void setInharmonicity(float inharm);  // 0 - 0.1

    // Inject energy (from exciter or sympathetic coupling)
    void excite(float amount);

    // Process one sample, return output
    float process(float externalInput = 0.0f);

    // Get current energy level (for visualization)
    float getEnergy() const { return energy; }

    // Get last output sample
    float getOutput() const { return lastOutput; }

    // Reset state
    void reset();

private:
    float nextNoise();

    double sampleRate = 44100.0;
    float frequency = 440.0f;
    float damping = 0.998f;
    float brightness = 0.8f;
    float inharmonicity = 0.0f;

    // Delay line
    std::array<float, MAX_DELAY> delayLine{};
    int writePos = 0;
    int delayLength = 100;
    float fractionalDelay = 0.0f;

    // Filters
    float lpfState = 0.0f;       // Lowpass for damping
    float lpfCoeff = 0.5f;
    float apfState = 0.0f;       // Allpass for inharmonicity
    float apfCoeff = 0.0f;

    // State
    float energy = 0.0f;
    float lastOutput = 0.0f;
    uint32_t noiseState = 12345;
};

} // namespace rgs
