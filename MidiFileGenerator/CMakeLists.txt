# Build steps for MidiFileGenerator

cmake_minimum_required(VERSION 3.12)
project(MIDI_FILE_GENERATOR VERSION 0.0.1)
add_subdirectory(JUCE)
juce_add_console_app(MidiFileGenerator
    PRODUCT_NAME "Midi File Generator")

target_sources(MidiFileGenerator
    PRIVATE
        Main.cpp
        MidiFileGenerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Utility/Utility.cpp)

target_compile_definitions(MidiFileGenerator
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(MidiFileGenerator
    PRIVATE   
        juce::juce_core
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
