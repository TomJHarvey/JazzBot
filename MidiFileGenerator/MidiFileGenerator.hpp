//
//  MidiFileGenerator.hpp
//  MidiFileGenerator
//
//  Created by Tom Harvey on 27/03/2022.
//

#ifndef MidiFileGenerator_hpp
#define MidiFileGenerator_hpp

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
    bool findBeat(const float& note_onset, std::size_t& beat) const;
    float getMidiTickDuration(const float& note_duration,
                              const float& note_onset) const;
    float getMidiTickOnset(const float& note_onset) const;
    bool isNoteBeforeFirstBar(const float& note_onset) const;
    
    bool calculateQuarterNoteIncrement(float& quarter_note_increment,
                                       const std::size_t& current_bar,
                                       const std::vector<std::vector<std::string>>& bar_information,
                                       const float& bar_onset) const;
    
private:
    std::vector<float> m_beat_onsets;
    std::vector<float> m_beat_lengths;
    std::size_t m_current_beat;
    juce::String m_project_path;
    std::string m_bar_onset_file_name;
    std::string m_note_onset_file_name;
    juce::MidiFile m_midi_file;
    juce::MidiMessageSequence m_midi_sequence;
    juce::File m_midi_file_output;
};

#endif /* MidiFileGenerator_hpp */
