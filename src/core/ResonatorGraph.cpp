#include "ResonatorGraph.h"
#include <cmath>
#include <algorithm>

namespace rgs {

// Interval-based coupling strengths (semitones -> coupling weight)
// Based on psychoacoustic consonance
static const float INTERVAL_COUPLING[13] = {
    1.0f,   // 0: Unison
    0.1f,   // 1: m2
    0.2f,   // 2: M2
    0.4f,   // 3: m3
    0.5f,   // 4: M3
    0.6f,   // 5: P4
    0.2f,   // 6: Tritone
    0.8f,   // 7: P5
    0.5f,   // 8: m6
    0.4f,   // 9: M6
    0.3f,   // 10: m7
    0.2f,   // 11: M7
    0.9f    // 12: Octave
};

ResonatorGraph::ResonatorGraph() {
    // Initialize coupling matrix to zero
    for (auto& row : coupling) {
        row.fill(0.0f);
    }

    // Set frequencies for each node (C4 to B4)
    for (int i = 0; i < NUM_NODES; i++) {
        float freq = BASE_FREQ * std::pow(2.0f, i / 12.0f);
        nodes[i].setFrequency(freq);
    }

    buildTopology(Topology::Fifths);
}

void ResonatorGraph::prepare(double sr) {
    sampleRate = sr;
    for (auto& node : nodes) {
        node.prepare(sr);
    }
}

void ResonatorGraph::setTopology(Topology topo) {
    currentTopology = topo;
    buildTopology(topo);
}

void ResonatorGraph::buildTopology(Topology topo) {
    // Clear existing connections
    for (auto& row : coupling) {
        row.fill(0.0f);
    }

    switch (topo) {
        case Topology::Chromatic:
            // Connect each node to neighbors +-1 semitone
            for (int i = 0; i < NUM_NODES; i++) {
                int prev = (i + NUM_NODES - 1) % NUM_NODES;
                int next = (i + 1) % NUM_NODES;
                coupling[i][prev] = 0.3f;
                coupling[i][next] = 0.3f;
            }
            break;

        case Topology::Fifths:
            // Circle of fifths: each node connects to +7 and -7 semitones
            for (int i = 0; i < NUM_NODES; i++) {
                int fifth = (i + 7) % NUM_NODES;
                int fourth = (i + 5) % NUM_NODES;  // -7 = +5
                coupling[i][fifth] = 0.6f;
                coupling[i][fourth] = 0.6f;
            }
            break;

        case Topology::Tonnetz:
            // Neo-Riemannian: triangular lattice with M3, m3, P5
            for (int i = 0; i < NUM_NODES; i++) {
                int M3 = (i + 4) % NUM_NODES;   // Major third
                int m3 = (i + 3) % NUM_NODES;   // Minor third
                int P5 = (i + 7) % NUM_NODES;   // Perfect fifth

                coupling[i][M3] = 0.5f;
                coupling[i][m3] = 0.4f;
                coupling[i][P5] = 0.7f;
            }
            break;

        case Topology::Harmonic:
            // Harmonic series connections
            // Connect to nodes that are harmonics (octave, fifth, etc.)
            for (int i = 0; i < NUM_NODES; i++) {
                // Octave above (if exists in our range, wraps)
                coupling[i][(i + 12) % NUM_NODES] = 0.8f;
                // Fifth
                coupling[i][(i + 7) % NUM_NODES] = 0.6f;
                // Major third
                coupling[i][(i + 4) % NUM_NODES] = 0.4f;
            }
            break;

        case Topology::Custom:
            // Leave empty, user will set manually
            break;
    }
}

void ResonatorGraph::setCoupling(int from, int to, float weight) {
    if (from >= 0 && from < NUM_NODES && to >= 0 && to < NUM_NODES) {
        coupling[from][to] = std::clamp(weight, 0.0f, 1.0f);
    }
}

void ResonatorGraph::setGlobalCoupling(float amount) {
    globalCoupling = std::clamp(amount, 0.0f, 1.0f);
}

void ResonatorGraph::noteOn(int midiNote, float velocity) {
    int node = midiToNode(midiNote);
    if (node >= 0 && node < NUM_NODES) {
        // Update frequency to match exact MIDI note
        nodes[node].setFrequency(midiToFreq(midiNote));
        nodes[node].excite(velocity);
    }
}

void ResonatorGraph::noteOff(int midiNote) {
    // In physical model, note-off just means no more excitation
    // The string naturally decays
    // Could add damper behavior here
}

void ResonatorGraph::processBlock(float* leftOut, float* rightOut, int numSamples) {
    // Coupling scale: audible but controlled
    // globalCoupling 0.3 = subtle, 0.7 = obvious, 1.0 = dramatic
    float couplingScale = globalCoupling * 0.08f;

    for (int s = 0; s < numSamples; s++) {
        // Calculate sympathetic coupling for this sample
        std::array<float, NUM_NODES> excitations{};
        excitations.fill(0.0f);

        // Gather energy from coupled nodes
        for (int src = 0; src < NUM_NODES; src++) {
            float srcEnergy = nodes[src].getEnergy();
            if (srcEnergy < 0.005f) continue;  // Gate: skip quiet nodes

            float srcOutput = nodes[src].getOutput();

            for (int tgt = 0; tgt < NUM_NODES; tgt++) {
                if (src != tgt && coupling[src][tgt] > 0.0f) {
                    excitations[tgt] += srcOutput * coupling[src][tgt] * couplingScale;
                }
            }
        }

        // Limit per-node excitation to prevent runaway feedback
        for (int i = 0; i < NUM_NODES; i++) {
            excitations[i] = std::tanh(excitations[i] * 5.0f) * 0.1f;
        }

        // Process each node with sympathetic excitation
        float left = 0.0f;
        float right = 0.0f;

        for (int i = 0; i < NUM_NODES; i++) {
            float sample = nodes[i].process(excitations[i]);

            // Stereo panning: spread nodes across stereo field
            float pan = static_cast<float>(i) / (NUM_NODES - 1);  // 0 to 1
            left += sample * (1.0f - pan);
            right += sample * pan;
        }

        // Normalize and soft limit
        float gain = 0.15f;  // Master gain
        left *= gain;
        right *= gain;

        // Soft clip
        leftOut[s] = std::tanh(left * 2.0f) * 0.8f;
        rightOut[s] = std::tanh(right * 2.0f) * 0.8f;
    }
}

std::array<float, ResonatorGraph::NUM_NODES> ResonatorGraph::getEnergies() const {
    std::array<float, NUM_NODES> energies;
    for (int i = 0; i < NUM_NODES; i++) {
        energies[i] = nodes[i].getEnergy();
    }
    return energies;
}

float ResonatorGraph::getCoupling(int from, int to) const {
    if (from >= 0 && from < NUM_NODES && to >= 0 && to < NUM_NODES) {
        return coupling[from][to];
    }
    return 0.0f;
}

void ResonatorGraph::setDamping(float d) {
    damping = std::clamp(d, 0.9f, 0.9999f);
    for (auto& node : nodes) {
        node.setDamping(damping);
    }
}

void ResonatorGraph::setBrightness(float b) {
    brightness = std::clamp(b, 0.0f, 1.0f);
    for (auto& node : nodes) {
        node.setBrightness(brightness);
    }
}

void ResonatorGraph::reset() {
    for (auto& node : nodes) {
        node.reset();
    }
}

float ResonatorGraph::midiToFreq(int midiNote) const {
    return 440.0f * std::pow(2.0f, (midiNote - 69) / 12.0f);
}

int ResonatorGraph::midiToNode(int midiNote) const {
    // Map MIDI note to node index (0-11)
    return midiNote % NUM_NODES;
}

} // namespace rgs
