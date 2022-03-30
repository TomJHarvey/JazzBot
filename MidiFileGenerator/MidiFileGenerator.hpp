//
//  MidiFileGenerator.hpp
//  MidiFileGenerator
//
//  Created by Tom Harvey on 27/03/2022.
//

#ifndef MidiFileGenerator_hpp
#define MidiFileGenerator_hpp

#include "Sequence.hpp"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

class MidiFileGenerator
{
public:
    MidiFileGenerator(const juce::String& project_path,
                      const std::string& bar_onset_file_name,
                      const std::string& note_onset_file_name);
    bool setBeatInformation();
    void initMidiFile();
    bool writeSequence();
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
    juce::String m_project_path;
    std::string m_bar_onset_file_name;
    std::string m_note_onset_file_name;
    juce::MidiFile m_midi_file;
    juce::MidiMessageSequence m_midi_sequence;
    juce::File m_midi_file_output;
    Sequence m_sequence;
};

#endif /* MidiFileGenerator_hpp */
