#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../core/ResonatorGraph.h"
#include <array>

namespace rgs {

/**
 * Visual representation of the resonator graph
 *
 * Shows nodes arranged in a circle with connections between them.
 * Node brightness indicates energy level.
 */
class GraphView : public juce::Component {
public:
    explicit GraphView(ResonatorGraph& graph);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ResonatorGraph& graph;

    // Node positions (calculated on resize)
    std::array<juce::Point<float>, ResonatorGraph::NUM_NODES> nodePositions;

    // Note names for display
    static constexpr const char* NOTE_NAMES[12] = {
        "C", "C#", "D", "D#", "E", "F",
        "F#", "G", "G#", "A", "A#", "B"
    };

    void calculateNodePositions();
    juce::Colour getNodeColour(float energy) const;
};

} // namespace rgs
