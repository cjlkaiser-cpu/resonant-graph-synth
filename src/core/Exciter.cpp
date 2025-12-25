#include "Exciter.h"
#include <algorithm>

namespace rgs {

void Exciter::generate(float* buffer, int numSamples, float velocity) {
    velocity = std::clamp(velocity, 0.0f, 1.0f);

    switch (type) {
        case Type::Pluck: {
            // Pluck: Burst of filtered noise
            // Hardness affects the lowpass filter
            float lpfCoeff = 0.3f + hardness * 0.6f;
            float lpfState = 0.0f;

            // Position affects the excitation shape
            // Plucking near the bridge = brighter
            // Plucking near center = fuller
            float brightnessBoost = 1.0f - std::abs(position - 0.5f) * 2.0f;

            for (int i = 0; i < numSamples; i++) {
                // Envelope: quick attack, short decay
                float env = std::exp(-static_cast<float>(i) / (numSamples * 0.3f));

                float noise = nextNoise();

                // Lowpass based on hardness
                lpfState = lpfCoeff * noise + (1.0f - lpfCoeff) * lpfState;
                float sample = lpfState * env * velocity;

                // Boost harmonics based on position
                sample *= (0.7f + brightnessBoost * 0.3f);

                buffer[i] = sample;
            }
            break;
        }

        case Type::Strike: {
            // Strike: Softer attack, like a hammer
            // More sinusoidal initial shape
            float attackTime = 0.1f + (1.0f - hardness) * 0.2f;

            for (int i = 0; i < numSamples; i++) {
                float t = static_cast<float>(i) / numSamples;

                // Raised cosine envelope
                float env = 0.0f;
                if (t < attackTime) {
                    env = 0.5f * (1.0f - std::cos(3.14159f * t / attackTime));
                } else {
                    float decay = (t - attackTime) / (1.0f - attackTime);
                    env = std::exp(-decay * 5.0f);
                }

                // Mix of noise and sine for strike
                float noise = nextNoise();
                float sine = std::sin(t * 3.14159f * 4.0f);
                float mix = hardness * noise + (1.0f - hardness) * sine;

                buffer[i] = mix * env * velocity;
            }
            break;
        }
    }
}

float Exciter::nextNoise() {
    noiseState = noiseState * 1103515245 + 12345;
    return (static_cast<float>(noiseState) / static_cast<float>(UINT32_MAX)) * 2.0f - 1.0f;
}

} // namespace rgs
