#include "PluginEditor.h"

ResonantGraphSynthEditor::ResonantGraphSynthEditor(ResonantGraphSynthProcessor& p)
    : AudioProcessorEditor(&p),
      processor(p),
      graphView(p.getGraph()),
      keyboard(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 700);

    // Graph view
    addAndMakeVisible(graphView);

    // On-screen keyboard
    keyboard.setAvailableRange(48, 84);  // C3 to C6
    addAndMakeVisible(keyboard);

    // Damping slider
    dampingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(dampingSlider);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dampingLabel);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.parameters, "damping", dampingSlider);

    // Brightness slider
    brightnessSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    brightnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(brightnessSlider);
    brightnessLabel.setText("Brightness", juce::dontSendNotification);
    brightnessLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(brightnessLabel);
    brightnessAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.parameters, "brightness", brightnessSlider);

    // Coupling slider
    couplingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    couplingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(couplingSlider);
    couplingLabel.setText("Coupling", juce::dontSendNotification);
    couplingLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(couplingLabel);
    couplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.parameters, "coupling", couplingSlider);

    // Topology selector
    topologySelector.addItem("Chromatic", 1);
    topologySelector.addItem("Fifths", 2);
    topologySelector.addItem("Tonnetz", 3);
    topologySelector.addItem("Harmonic", 4);
    addAndMakeVisible(topologySelector);
    topologyLabel.setText("Topology", juce::dontSendNotification);
    topologyLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(topologyLabel);
    topologyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processor.parameters, "topology", topologySelector);

    // Start timer for visualization updates
    startTimerHz(30);
}

ResonantGraphSynthEditor::~ResonantGraphSynthEditor() {
    stopTimer();
}

void ResonantGraphSynthEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xff0f172a));  // Dark background

    // Title
    g.setColour(juce::Colours::white);
    g.setFont(24.0f);
    g.drawText("Resonant Graph Synth", 20, 10, 300, 30, juce::Justification::left);
}

void ResonantGraphSynthEditor::resized() {
    auto bounds = getLocalBounds();

    // Top area for title
    bounds.removeFromTop(50);

    // Bottom: keyboard
    keyboard.setBounds(bounds.removeFromBottom(80));

    // Above keyboard: controls
    auto controlArea = bounds.removeFromBottom(120);
    int controlWidth = controlArea.getWidth() / 4;

    // Control 1: Damping
    auto dampingArea = controlArea.removeFromLeft(controlWidth);
    dampingLabel.setBounds(dampingArea.removeFromTop(20));
    dampingSlider.setBounds(dampingArea.reduced(10));

    // Control 2: Brightness
    auto brightnessArea = controlArea.removeFromLeft(controlWidth);
    brightnessLabel.setBounds(brightnessArea.removeFromTop(20));
    brightnessSlider.setBounds(brightnessArea.reduced(10));

    // Control 3: Coupling
    auto couplingArea = controlArea.removeFromLeft(controlWidth);
    couplingLabel.setBounds(couplingArea.removeFromTop(20));
    couplingSlider.setBounds(couplingArea.reduced(10));

    // Control 4: Topology
    auto topologyArea = controlArea;
    topologyLabel.setBounds(topologyArea.removeFromTop(20));
    topologySelector.setBounds(topologyArea.reduced(20, 30));

    // Graph view takes remaining space
    graphView.setBounds(bounds.reduced(20));
}

void ResonantGraphSynthEditor::timerCallback() {
    // Process keyboard events
    juce::MidiBuffer midiBuffer;
    keyboardState.processNextMidiBuffer(midiBuffer, 0, 512, true);

    for (const auto metadata : midiBuffer) {
        auto message = metadata.getMessage();
        if (message.isNoteOn()) {
            processor.getGraph().noteOn(message.getNoteNumber(),
                                        message.getVelocity() / 127.0f);
        }
        else if (message.isNoteOff()) {
            processor.getGraph().noteOff(message.getNoteNumber());
        }
    }

    graphView.repaint();
}
