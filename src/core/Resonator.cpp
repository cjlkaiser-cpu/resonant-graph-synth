#include "Resonator.h"
#include <cmath>
#include <algorithm>

namespace rgs {

Resonator::Resonator() {
    delayLine.fill(0.0f);
}

void Resonator::prepare(double sr) {
    sampleRate = sr;
    setFrequency(frequency);
    reset();
}

void Resonator::setFrequency(float freq) {
    frequency = std::clamp(freq, 20.0f, 20000.0f);

    // Calculate delay length for this frequency
    float totalDelay = static_cast<float>(sampleRate) / frequency;

    // Account for filter delays (approximate)
    totalDelay -= 0.5f; // Lowpass filter delay
    totalDelay -= apfCoeff; // Allpass filter delay

    delayLength = static_cast<int>(totalDelay);
    fractionalDelay = totalDelay - static_cast<float>(delayLength);

    delayLength = std::clamp(delayLength, 2, MAX_DELAY - 1);
}

void Resonator::setDamping(float d) {
    damping = std::clamp(d, 0.9f, 0.9999f);
}

void Resonator::setBrightness(float b) {
    brightness = std::clamp(b, 0.0f, 1.0f);
    // Map brightness to lowpass coefficient
    // brightness=1 -> lpfCoeff=1 (no filtering)
    // brightness=0 -> lpfCoeff=0.2 (heavy filtering)
    lpfCoeff = 0.2f + brightness * 0.8f;
}

void Resonator::setInharmonicity(float inharm) {
    inharmonicity = std::clamp(inharm, 0.0f, 0.1f);
    // Allpass coefficient for slight pitch modulation
    apfCoeff = inharmonicity * 0.5f;
}

void Resonator::excite(float amount) {
    // Fill delay line with filtered noise
    for (int i = 0; i < delayLength; i++) {
        float noise = nextNoise() * amount;
        int pos = (writePos + MAX_DELAY - i) % MAX_DELAY;
        delayLine[pos] += noise;
    }
    energy = std::max(energy, std::abs(amount));
}

float Resonator::process(float externalInput) {
    // Read from delay line (with linear interpolation for fractional delay)
    int readPos = (writePos + MAX_DELAY - delayLength) % MAX_DELAY;
    int readPosNext = (readPos + MAX_DELAY - 1) % MAX_DELAY;

    float sample = delayLine[readPos] * (1.0f - fractionalDelay)
                 + delayLine[readPosNext] * fractionalDelay;

    // Lowpass filter (one-pole) for frequency-dependent damping
    lpfState = lpfCoeff * sample + (1.0f - lpfCoeff) * lpfState;
    float filtered = lpfState;

    // Allpass filter for inharmonicity
    if (inharmonicity > 0.001f) {
        float apfOut = apfCoeff * (filtered - apfState) + delayLine[readPos];
        apfState = apfOut;
        filtered = apfOut;
    }

    // Apply damping
    float feedback = filtered * damping;

    // Add external input (sympathetic resonance)
    feedback += externalInput;

    // Soft clamp to prevent blowup
    feedback = std::tanh(feedback);

    // Safety check
    if (!std::isfinite(feedback)) {
        feedback = 0.0f;
    }

    // Write to delay line
    delayLine[writePos] = feedback;

    // Advance write position
    writePos = (writePos + 1) % MAX_DELAY;

    // Update energy (exponential decay + peak tracking)
    energy = energy * 0.9995f;
    float absSample = std::abs(sample);
    if (absSample > energy) {
        energy = absSample;
    }

    lastOutput = sample;
    return sample;
}

void Resonator::reset() {
    delayLine.fill(0.0f);
    writePos = 0;
    lpfState = 0.0f;
    apfState = 0.0f;
    energy = 0.0f;
    lastOutput = 0.0f;
}

float Resonator::nextNoise() {
    // Simple LCG noise generator
    noiseState = noiseState * 1103515245 + 12345;
    return (static_cast<float>(noiseState) / static_cast<float>(UINT32_MAX)) * 2.0f - 1.0f;
}

} // namespace rgs
