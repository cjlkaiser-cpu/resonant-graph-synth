#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace rgs {

/**
 * Excitation generator for resonators
 *
 * Generates the initial impulse that sets a resonator vibrating.
 * Different excitation types produce different timbres:
 * - Pluck: Sharp attack, like a guitar pick
 * - Strike: Softer, like a hammer (piano, mallet)
 * - Bow: Sustained excitation (future)
 */
class Exciter {
public:
    enum class Type {
        Pluck,
        Strike
    };

    Exciter() = default;

    void setType(Type t) { type = t; }
    void setPosition(float p) { position = std::clamp(p, 0.1f, 0.9f); }
    void setHardness(float h) { hardness = std::clamp(h, 0.0f, 1.0f); }

    /**
     * Generate excitation buffer for a resonator
     *
     * @param buffer Output buffer to fill
     * @param numSamples Number of samples
     * @param velocity Trigger velocity (0-1)
     */
    void generate(float* buffer, int numSamples, float velocity);

private:
    float nextNoise();

    Type type = Type::Pluck;
    float position = 0.5f;   // Where on the string to excite (affects harmonics)
    float hardness = 0.5f;   // Soft (muted) to hard (bright)
    uint32_t noiseState = 54321;
};

} // namespace rgs
