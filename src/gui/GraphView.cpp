#include "GraphView.h"
#include <cmath>

namespace rgs {

GraphView::GraphView(ResonatorGraph& g) : graph(g) {
    setOpaque(false);
}

void GraphView::resized() {
    calculateNodePositions();
}

void GraphView::calculateNodePositions() {
    auto bounds = getLocalBounds().toFloat();
    float centerX = bounds.getCentreX();
    float centerY = bounds.getCentreY();
    float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.4f;

    for (int i = 0; i < ResonatorGraph::NUM_NODES; i++) {
        // Arrange in circle, starting from top (C at 12 o'clock)
        float angle = -juce::MathConstants<float>::halfPi +
                      (static_cast<float>(i) / ResonatorGraph::NUM_NODES) *
                      juce::MathConstants<float>::twoPi;

        nodePositions[i] = {
            centerX + radius * std::cos(angle),
            centerY + radius * std::sin(angle)
        };
    }
}

void GraphView::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(juce::Colour(0xff1e293b));
    g.fillRoundedRectangle(bounds, 10.0f);

    // Get current energies
    auto energies = graph.getEnergies();

    // Draw connections first (behind nodes)
    for (int i = 0; i < ResonatorGraph::NUM_NODES; i++) {
        for (int j = i + 1; j < ResonatorGraph::NUM_NODES; j++) {
            float coupling = graph.getCoupling(i, j);
            if (coupling > 0.01f) {
                // Line thickness based on coupling strength
                float thickness = 1.0f + coupling * 3.0f;

                // Colour based on combined energy
                float combinedEnergy = (energies[i] + energies[j]) * 0.5f;
                juce::Colour lineColour = juce::Colour(0xff475569).interpolatedWith(
                    juce::Colour(0xffa855f7),  // Purple accent
                    std::min(combinedEnergy * 3.0f, 1.0f)
                );

                g.setColour(lineColour.withAlpha(0.3f + coupling * 0.5f));
                g.drawLine(nodePositions[i].x, nodePositions[i].y,
                          nodePositions[j].x, nodePositions[j].y,
                          thickness);
            }
        }
    }

    // Draw nodes
    float nodeRadius = 25.0f;

    for (int i = 0; i < ResonatorGraph::NUM_NODES; i++) {
        auto pos = nodePositions[i];
        float energy = energies[i];

        // Node glow based on energy
        if (energy > 0.01f) {
            float glowRadius = nodeRadius + energy * 30.0f;
            juce::Colour glowColour = juce::Colour(0xffa855f7).withAlpha(energy * 0.5f);
            g.setColour(glowColour);
            g.fillEllipse(pos.x - glowRadius, pos.y - glowRadius,
                         glowRadius * 2, glowRadius * 2);
        }

        // Node body
        juce::Colour nodeColour = getNodeColour(energy);
        g.setColour(nodeColour);
        g.fillEllipse(pos.x - nodeRadius, pos.y - nodeRadius,
                     nodeRadius * 2, nodeRadius * 2);

        // Node border
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawEllipse(pos.x - nodeRadius, pos.y - nodeRadius,
                     nodeRadius * 2, nodeRadius * 2, 2.0f);

        // Note name
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawText(NOTE_NAMES[i],
                  static_cast<int>(pos.x - nodeRadius),
                  static_cast<int>(pos.y - 8),
                  static_cast<int>(nodeRadius * 2),
                  16,
                  juce::Justification::centred);
    }
}

juce::Colour GraphView::getNodeColour(float energy) const {
    // Interpolate from dark (no energy) to bright purple (full energy)
    juce::Colour darkColour(0xff334155);
    juce::Colour brightColour(0xffa855f7);

    float t = std::min(energy * 2.0f, 1.0f);  // Scale energy for visibility
    return darkColour.interpolatedWith(brightColour, t);
}

} // namespace rgs
