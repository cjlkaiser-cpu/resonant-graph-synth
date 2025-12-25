#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "gui/GraphView.h"

class ResonantGraphSynthEditor : public juce::AudioProcessorEditor,
                                  private juce::Timer {
public:
    explicit ResonantGraphSynthEditor(ResonantGraphSynthProcessor&);
    ~ResonantGraphSynthEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    ResonantGraphSynthProcessor& processor;

    // Graph visualization
    rgs::GraphView graphView;

    // Controls
    juce::Slider dampingSlider;
    juce::Slider brightnessSlider;
    juce::Slider couplingSlider;
    juce::ComboBox topologySelector;

    juce::Label dampingLabel;
    juce::Label brightnessLabel;
    juce::Label couplingLabel;
    juce::Label topologyLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> brightnessAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> couplingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> topologyAttachment;

    // On-screen keyboard
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResonantGraphSynthEditor)
};
