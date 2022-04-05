//
//  MidiFileGenerator.hpp
//  MidiFileGenerator
//
//  Created by Tom Harvey on 27/03/2022.
//

#ifndef MidiFileGenerator_hpp
#define MidiFileGenerator_hpp

#include "Sequence.hpp"

#include "SongInformation.hpp"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

using CsvTable = std::vector<std::vector<std::string>>;

// This class generates a midi sequence based off a sequence of note durations, onsets and values.

class MidiFileGenerator
{
public:
    MidiFileGenerator(const juce::String& output_directory_path,
                      const juce::String& output_file_path,
                      const TimeSignature& time_signature);
    bool setBeatInformation(const CsvTable& bar_information);
    bool writeSequence(const CsvTable& note_information);
    void initMidiFile();
    bool writeMidiFile() const;
    
private:
    bool isValidNoteOn();
    bool calculateQuarterNoteIncrement(float& quarter_note_increment,
                                       const std::size_t& current_bar,
                                       const std::vector<std::vector<std::string>>& bar_information,
                                       const float& bar_onset,
                                       float& next_bar_onset,
                                       const float& previous_bar_onset) const;
    
    static bool validStofConversion(const std::string& value_to_convert, float& converted_value);
    
private:
    juce::String m_output_directory_path;
    juce::String m_output_file_path;
    juce::MidiFile m_midi_file;
    juce::MidiMessageSequence m_midi_sequence;
    juce::File m_midi_file_output;
    Sequence m_sequence;
};

#endif /* MidiFileGenerator_hpp */
