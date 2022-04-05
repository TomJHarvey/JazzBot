//
//  MidiFileGenerator.cpp
//  MidiFileGenerator
//
//  Created by Tom Harvey on 27/03/2022.
//

#include "MidiFileGenerator.hpp"
#include "../Utility/Utility.hpp"

static const std::size_t note_information_csv_row_size = 3;
static const std::size_t bar_onset_index = 2;
static const std::size_t note_duration_index = 0;
static const std::size_t note_onset_index = 1;
static const std::size_t note_value_index = 2;
static const int note_velocity = 127;
static const int juce_message_on = 0x90;
static const int juce_message_off = 0x80;


MidiFileGenerator::MidiFileGenerator(const juce::String& output_directory_path,
                                     const juce::String& output_file_path,
                                     const TimeSignature& time_signature)
    : m_output_directory_path(output_directory_path)
    , m_output_file_path(output_file_path)
    , m_sequence(static_cast<int>(time_signature))
{
}

bool
MidiFileGenerator::setBeatInformation(const CsvTable& bar_information)
{
    if (!bar_information.empty())
    {
        float previous_beat_onset = -1.0f;
        float next_bar_onset = -1.0f;
        float previous_bar_onset = -1.0f;
        for(std::size_t current_bar = 0; current_bar < bar_information.size(); current_bar++)
        {
            if (bar_information[current_bar].size() == note_information_csv_row_size)
            {
                if (current_bar != 0) // check its not the first row, contains string headings
                {
                    float bar_onset = 0.0f;
                    if (next_bar_onset > 0)
                    {
                        bar_onset = next_bar_onset; // use previously converted value
                    }
                    else
                    {
                        // if first time it will need to convert
                        if (!Utility::validStofConversion(bar_information[current_bar][bar_onset_index],
                                                          bar_onset))
                        {
                            return false;
                        }
                    }
                    if (previous_beat_onset > bar_onset)
                    {
                        std::cout << "Non sequential data, current bar onset: " << bar_onset
                                  << " is bigger than previous beat onset: "
                                  << previous_beat_onset << std::endl;
                        return false;
                    }
                    
                    // calculate the length of a quarter note in the current bar
                    float quarter_note_increment = 0.0f;
                    if (!calculateQuarterNoteIncrement(quarter_note_increment,
                                                       current_bar,
                                                       bar_information,
                                                       bar_onset,
                                                       next_bar_onset,
                                                       previous_bar_onset))
                    {
                        return false;
                    }
                    else // set the beat information internally for the sequence for this bar
                    {
                        m_sequence.setBeatInformation(bar_onset,
                                                      quarter_note_increment,
                                                      previous_beat_onset);
                    }
                    previous_bar_onset = bar_onset;
                }
            }
        }
    }
    else
    {
        return false; // bar information empty
    }
    return true; // no exceptions for string conversions
}

bool
MidiFileGenerator::calculateQuarterNoteIncrement(float& quarter_note_increment,
                                    const std::size_t& current_bar,
                                    const std::vector<std::vector<std::string>>& bar_information,
                                    const float& bar_onset,
                                    float& next_bar_onset,
                                    const float& previous_bar_onset) const
{
    // calculate the length of a quarter note in the current bar
    if (current_bar + 1 < bar_information.size()) // check it can access the next bar
    {
        if (!Utility::validStofConversion(bar_information[current_bar + 1][bar_onset_index],
                                          next_bar_onset))
        {
            return false;
        }
        quarter_note_increment = m_sequence.calculateQuarterNoteIncrement(next_bar_onset,
                                                                          bar_onset);
    }
    else // previous bars spacing used for the last bar, no next bar to make the calculation
    {
        quarter_note_increment = m_sequence.calculateQuarterNoteIncrement(bar_onset,
                                                                          previous_bar_onset);
    }
    return true;
}

bool
MidiFileGenerator::writeSequence(const CsvTable& note_information) // TODO: this can return false if something fails in function
{
    if (!note_information.empty())
    {
        float previous_note_onset = -1.0f;
        for (std::size_t index = 0; index < note_information.size(); index++)
        {
            if (index != 0) // ignore csv file headings
            {
                if (note_information[index].size() == 3)
                {
                    float note_onset = 0.0f;
                    if (!Utility::validStofConversion(note_information[index][note_onset_index],
                                                      note_onset))
                    {
                        return false;
                    }
                    if (m_sequence.isNoteBeforeFirstBar(note_onset))
                    {
                        // TODO: store the note values before first bar, just not durations
                        //std::cout << "Notes before first bar will not be included" << std::endl;
                    }
                    else
                    {
                        float note_duration = 0.0f;
                        if (!Utility::validStofConversion(note_information[index][note_duration_index],
                                                          note_duration))
                        {
                            return false;
                        }
                        
                        // find the beat the note is in
                        if (!m_sequence.setCurrentBeat(note_onset))
                        {
                            return true; // end of sequence
                        }
                        
                        
                        if (previous_note_onset > note_onset)
                        {
                            std::cout << "Current onset: " << note_onset
                                      << " is smaller than previous onset: "
                                      << previous_note_onset << std::endl;
                            return false;
                        }
                        
                        float midi_tick_onset = m_sequence.getMidiTickOnset(note_onset);
                        
                        float midi_tick_duration = m_sequence.getMidiTickDuration(note_duration,
                                                                                  note_onset);
                        if (midi_tick_duration <= 0) // TODO: investiate why this happens for different files each time
                        {
                            std::cout << "Invalid duration: " << midi_tick_duration << " :" << m_output_file_path << std::endl;
                            std::cout << note_duration << std::endl;
                            std::cout << note_onset << std::endl;
                            return false;
                        }

                        int note_value = -1;
                        if (!Utility::validStoiConversion(note_information[index][note_value_index], note_value))
                        {
                            std::cout << "Invalid note: " << midi_tick_duration << std::endl;
                            return false;
                        }
                        
                        juce::MidiMessage message_on (juce_message_on ,note_value, note_velocity);
                        message_on.setTimeStamp(midi_tick_onset);
                        juce::MidiMessage message_off (juce_message_off, note_value, 0);
                        message_off.setTimeStamp(midi_tick_onset + midi_tick_duration);
                        m_midi_sequence.addEvent(message_on);
                        m_midi_sequence.addEvent(message_off);
                        
                        previous_note_onset = note_onset;
                    }
                }
            }
        }
        m_midi_file.addTrack(m_midi_sequence);
    }
    return true;
}

void
MidiFileGenerator::initMidiFile()
{
    m_midi_file.setTicksPerQuarterNote(m_sequence.getPpqn());
    juce::File output_directory = m_output_directory_path;
    if (!output_directory.isDirectory())
    {
        std::cout << "creating directory " << output_directory.getFileName() << std::endl;
        output_directory.createDirectory();
    }

    juce::File file_location = m_output_directory_path + m_output_file_path + juce::String(".mid");
    m_midi_file_output = file_location;
}

bool
MidiFileGenerator::writeMidiFile() const
{
    juce::FileOutputStream file_output_stream (m_midi_file_output);
    file_output_stream.setPosition(0);
    file_output_stream.truncate();
    if(file_output_stream.failedToOpen())
    {
        std::cout << "File: " << file_output_stream.getFile().getFileName()
                  << " failed to open." << std::endl;
        return false;
    }
    m_midi_file.writeTo(file_output_stream);
    return true;
}
