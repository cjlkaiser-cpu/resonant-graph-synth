#pragma once

#include "Resonator.h"
#include <vector>
#include <array>

namespace rgs {

/**
 * Graph topology types
 */
enum class Topology {
    Chromatic,   // Each node connects to +-1 semitone
    Fifths,      // Circle of fifths connections
    Tonnetz,     // Neo-Riemannian triangular lattice
    Harmonic,    // Harmonic series from each node
    Custom       // User-defined
};

/**
 * A network of coupled resonators
 *
 * Energy flows between resonators based on the graph topology.
 * When you pluck one string, others resonate sympathetically.
 */
class ResonatorGraph {
public:
    static constexpr int NUM_NODES = 12;  // One octave for now
    static constexpr float BASE_FREQ = 261.63f;  // C4

    ResonatorGraph();

    void prepare(double sampleRate);

    // Topology
    void setTopology(Topology topo);
    void setCoupling(int from, int to, float weight);
    void setGlobalCoupling(float amount);  // 0-1 master coupling

    // Inject energy into a specific node (MIDI note)
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    // Process audio block
    void processBlock(float* leftOut, float* rightOut, int numSamples);

    // Visualization data
    std::array<float, NUM_NODES> getEnergies() const;
    float getCoupling(int from, int to) const;

    // Parameters
    void setDamping(float d);
    void setBrightness(float b);

    void reset();

private:
    void buildTopology(Topology topo);
    float midiToFreq(int midiNote) const;
    int midiToNode(int midiNote) const;

    double sampleRate = 44100.0;

    std::array<Resonator, NUM_NODES> nodes;
    std::array<std::array<float, NUM_NODES>, NUM_NODES> coupling{};

    float globalCoupling = 0.3f;
    float damping = 0.997f;
    float brightness = 0.7f;

    Topology currentTopology = Topology::Fifths;
};

} // namespace rgs
