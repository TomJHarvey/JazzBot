# Build steps for MidiFileBpmCorrector

cmake_minimum_required(VERSION 3.12)
project(MIDI_FILE_BPM_Corrector VERSION 0.0.1)
add_subdirectory(JUCE)
juce_add_console_app(MidiFileBpmCorrector
    PRODUCT_NAME "Midi File Corrector")

target_sources(MidiFileBpmCorrector
    PRIVATE
        Main.cpp
        MidiFileGenerator.cpp
        Sequence.cpp
        SongInformation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Utility/Utility.cpp)

target_compile_definitions(MidiFileBpmCorrector
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(MidiFileBpmCorrector
    PRIVATE   
        juce::juce_core
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
        

# Build steps for tests

find_package(Catch2 3 REQUIRED)
add_executable(tests tests/test.cpp
                     MidiFileGenerator.cpp
                     Sequence.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/../Utility/Utility.cpp)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
        juce::juce_core
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
