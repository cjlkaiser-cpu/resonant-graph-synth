#include "PluginProcessor.h"
#include "PluginEditor.h"

ResonantGraphSynthProcessor::ResonantGraphSynthProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

ResonantGraphSynthProcessor::~ResonantGraphSynthProcessor() {
}

juce::AudioProcessorValueTreeState::ParameterLayout
ResonantGraphSynthProcessor::createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "damping", "Damping",
        juce::NormalisableRange<float>(0.9f, 0.9999f, 0.0001f, 0.5f),
        0.997f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "brightness", "Brightness",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.7f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "coupling", "Coupling",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.3f
    ));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "topology", "Topology",
        juce::StringArray{"Chromatic", "Fifths", "Tonnetz", "Harmonic"},
        1  // Default: Fifths
    ));

    return {params.begin(), params.end()};
}

const juce::String ResonantGraphSynthProcessor::getName() const {
    return "Resonant Graph Synth";
}

bool ResonantGraphSynthProcessor::acceptsMidi() const { return true; }
bool ResonantGraphSynthProcessor::producesMidi() const { return false; }
bool ResonantGraphSynthProcessor::isMidiEffect() const { return false; }
double ResonantGraphSynthProcessor::getTailLengthSeconds() const { return 2.0; }

int ResonantGraphSynthProcessor::getNumPrograms() { return 1; }
int ResonantGraphSynthProcessor::getCurrentProgram() { return 0; }
void ResonantGraphSynthProcessor::setCurrentProgram(int) {}
const juce::String ResonantGraphSynthProcessor::getProgramName(int) { return {}; }
void ResonantGraphSynthProcessor::changeProgramName(int, const juce::String&) {}

void ResonantGraphSynthProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    graph.prepare(sampleRate);
}

void ResonantGraphSynthProcessor::releaseResources() {
    graph.reset();
}

bool ResonantGraphSynthProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void ResonantGraphSynthProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    // Update parameters
    float damping = *parameters.getRawParameterValue("damping");
    float brightness = *parameters.getRawParameterValue("brightness");
    float coupling = *parameters.getRawParameterValue("coupling");
    int topology = static_cast<int>(*parameters.getRawParameterValue("topology"));

    graph.setDamping(damping);
    graph.setBrightness(brightness);
    graph.setGlobalCoupling(coupling);
    graph.setTopology(static_cast<rgs::Topology>(topology));

    // Handle MIDI
    for (const auto metadata : midiMessages) {
        auto message = metadata.getMessage();

        if (message.isNoteOn()) {
            graph.noteOn(message.getNoteNumber(),
                        message.getVelocity() / 127.0f);
        }
        else if (message.isNoteOff()) {
            graph.noteOff(message.getNoteNumber());
        }
    }

    // Clear buffer
    buffer.clear();

    // Process audio
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    graph.processBlock(leftChannel, rightChannel, buffer.getNumSamples());
}

bool ResonantGraphSynthProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* ResonantGraphSynthProcessor::createEditor() {
    return new ResonantGraphSynthEditor(*this);
}

void ResonantGraphSynthProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ResonantGraphSynthProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(parameters.state.getType())) {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

// Plugin instantiation
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new ResonantGraphSynthProcessor();
}
