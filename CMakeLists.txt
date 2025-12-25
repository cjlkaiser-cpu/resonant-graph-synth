cmake_minimum_required(VERSION 3.21)

project(ResonantGraphSynth VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Plugin/Standalone target
juce_add_plugin(ResonantGraphSynth
    COMPANY_NAME "EigenLab"
    BUNDLE_ID "com.eigenlab.resonantgraphsynth"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Eglb
    PLUGIN_CODE Rgsy
    FORMATS Standalone VST3 AU
    PRODUCT_NAME "Resonant Graph Synth"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/resources/ResonantGraphSynth.icns"
    ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/resources/ResonantGraphSynth.icns"
)

# Source files
target_sources(ResonantGraphSynth
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
        src/core/Resonator.cpp
        src/core/ResonatorGraph.cpp
        src/core/Exciter.cpp
        src/gui/GraphView.cpp
)

# Header search paths
target_include_directories(ResonantGraphSynth
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# JUCE modules
target_link_libraries(ResonantGraphSynth
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_graphics
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Compile definitions
target_compile_definitions(ResonantGraphSynth
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)
