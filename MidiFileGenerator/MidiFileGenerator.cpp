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
static const int time_sig_quarter_note = 4; // this will be retrieved from the web scraper
static const int ppqn = 480; // pulses per quarter note
static const int note_velocity = 127;
static const int juce_message_on = 0x90;
static const int juce_message_off = 0x80;

MidiFileGenerator::MidiFileGenerator(const juce::String& project_path,
                                     const std::string& bar_onset_file_name,
                                     const std::string& note_onset_file_name)
    : m_current_beat(0)
    , m_project_path(project_path)
    , m_bar_onset_file_name(bar_onset_file_name)
    , m_note_onset_file_name(note_onset_file_name)
{
}

bool
MidiFileGenerator::setBeatInformation()
{
    std::vector<std::vector<std::string>> bar_information =
        Utility::parseCsvFile(m_bar_onset_file_name);

    if (!bar_information.empty())
    {
        float previous_beat_onset = -1.0f;
        
        for(std::size_t current_bar = 0; current_bar < bar_information.size(); current_bar++)
        {
            if (bar_information[current_bar].size() == note_information_csv_row_size)
            {
                if (current_bar != 0) // check its not the first row, contains string headings
                {
                    float bar_onset = 0.0f;
                    try{
                        bar_onset =
                            std::stof(bar_information[current_bar][bar_onset_index].c_str());
                    }
                    catch (const std::exception& e){
                        std::cout << "Invalid input: " << e.what() << std::endl;
                        return false;
                    }
                    
                    // calculate the length of a quarter note in the current bar
                    float quater_note_increment = 0.0f;
                    if (!calculateQuarterNoteIncrement(quater_note_increment,
                                                       current_bar,
                                                       bar_information,
                                                       bar_onset))
                    {
                        return false;
                    }
                        
                    // set the onset and length for each quarter note in the current bar
                    for (int quater_note_index = 0;
                         quater_note_index < time_sig_quarter_note;
                         quater_note_index ++)
                    {
                        float beat_onset = bar_onset + (quater_note_index * quater_note_increment);
                        m_beat_onsets.push_back(beat_onset);
                        if (previous_beat_onset > 0)
                        {
                            m_beat_lengths.push_back(beat_onset - previous_beat_onset);
                        }
                        previous_beat_onset = beat_onset;
                    }
                }
            }
        }
    }
    return true; // no exceptions for string conversions
}

bool
MidiFileGenerator::calculateQuarterNoteIncrement(float& quarter_note_increment,
                                    const std::size_t& current_bar,
                                    const std::vector<std::vector<std::string>>& bar_information,
                                    const float& bar_onset) const
{
    if (current_bar + 1 < bar_information.size()) // check it can access the next bar
    {
        float next_bar_onset =  0.0f;
        try{
            next_bar_onset = std::stof(bar_information[current_bar + 1][bar_onset_index].c_str());
        }
        catch(const std::exception& e){
            std::cout << "Invalid input: " << e.what() << std::endl;
            return false;
        }
        quarter_note_increment = (next_bar_onset - bar_onset) / time_sig_quarter_note;
    }
    else // previous bars spacing used for the last bar, no next bar to make the calculation
    {
        float previous_bar_onset =  0.0f;
        try{
            previous_bar_onset =
                std::stof(bar_information[current_bar - 1][bar_onset_index].c_str());
        }
        catch(const std::exception& e){
            std::cout << "Invalid input: " << e.what() << std::endl;
            return false;
        }
        quarter_note_increment = (bar_onset - previous_bar_onset) / time_sig_quarter_note;
    }
    return true;
}

void
MidiFileGenerator::initMidiFile()
{
    m_midi_file.setTicksPerQuarterNote(ppqn);
    //TODO: this needs to be in an output folder and to override a file with the same name
    juce::File file_location = m_project_path + juce::String("/test38") + juce::String(".mid");
    m_midi_file_output = file_location;
}

bool
MidiFileGenerator::findBeat(const float& note_onset, std::size_t& beat) const
{
    if (beat + 1 == m_beat_onsets.size())
    {
        if (note_onset > (m_beat_onsets[beat] + m_beat_lengths[beat]))
        {
            return false; // TODO: test out of range
        }
        return true; // the current bar is the last bar
    }
    else
    {
        if (m_beat_onsets[beat] < note_onset &&
            note_onset < m_beat_onsets[beat + 1])
        {
            return true; // return the beat number for the current note
        }
        else //current note is not in this beat
        {
            beat += 1;
            return findBeat(note_onset, beat);
        }
    }
}

float
MidiFileGenerator::getMidiTickDuration(const float& note_duration,
                                       const float& note_onset) const
{
    float midi_tick_duration = 0.0f;
    float relative_note_duration = note_onset + note_duration;
    std::size_t beat = m_current_beat;
    findBeat(relative_note_duration, beat);
    std::size_t number_of_beats = beat - m_current_beat;
   
    if (number_of_beats > 0 &&
        m_current_beat + number_of_beats < m_beat_onsets.size() &&
        relative_note_duration > m_beat_onsets[m_current_beat + number_of_beats])
    {
        // The duration of the portion of the current note in the current bar
        float under_spill = m_beat_onsets[m_current_beat + 1] - note_onset;
        float under_spill_relative_duration = ppqn * (under_spill/m_beat_lengths[m_current_beat]);
        
        // The duration of the portion of the current note in the last bar its in
        float last_beat_note_duration =
            relative_note_duration - m_beat_onsets[m_current_beat + number_of_beats];
        float last_beat_note_relative_duration =
            ppqn * (last_beat_note_duration/m_beat_lengths[m_current_beat + number_of_beats]);
        
        // If its more than one beat that the note is over, add the middle notes
        float extra_beats = ppqn * (number_of_beats - 1);
        
        midi_tick_duration = under_spill_relative_duration + last_beat_note_relative_duration + extra_beats;
    }
    else // no overlap, unless if last note where it doesn't matter the length
    {
        midi_tick_duration = ppqn * (note_duration/m_beat_lengths[m_current_beat]);
    }
    return midi_tick_duration;
}

float
MidiFileGenerator::getMidiTickOnset(const float& note_onset) const
{
    float beats_elapsed = (ppqn * m_current_beat);
    float relative_note_onset = (note_onset-m_beat_onsets[m_current_beat]);
    return (ppqn * (relative_note_onset/m_beat_lengths[m_current_beat])) + beats_elapsed;
}

bool
MidiFileGenerator::isNoteBeforeFirstBar(const float& note_onset) const
{
    return (note_onset < m_beat_onsets[0] ? true : false);
}

bool
MidiFileGenerator::writeSequence() // TODO: this can return false if something fails in function
{
    std::vector<std::vector<std::string>> note_information = Utility::parseCsvFile(m_note_onset_file_name);
    if (!note_information.empty())
    {
        float previous_midi_tick_onset = -1.0f;
        for (std::size_t i = 0; i < note_information.size(); i++)
        {
            if (i != 0) // ignore csv file headings
            {
                if (note_information[i].size() == 3)
                {
                    float note_onset = 0.0f;
                    try{
                        note_onset = stof(note_information[i][note_onset_index]);
                    }
                    catch(const std::exception& e){
                        std::cout << "Invalid input: " << e.what() << std::endl;
                        return false;
                    }
                    
                    if (isNoteBeforeFirstBar(note_onset))
                    {
                        // TODO: store the note values before first bar, just not durations
                        std::cout << "Notes before first bar will not be included" << std::endl;
                    }
                    else
                    {
                        float note_duration = 0.0f;
                        try{
                            note_duration = stof(note_information[i][note_duration_index]);;
                        }
                        catch(const std::exception& e){
                            std::cout << "Invalid input: " << e.what() << std::endl;
                            return false;
                        }
                        
                        // find the beat the note is in, exit if out of range
                        if (!findBeat(note_onset, m_current_beat))
                        {
                            return true;
                        }
                        
                        float midi_tick_onset = getMidiTickOnset(note_onset);
                        if (previous_midi_tick_onset > midi_tick_onset)
                        {
                            std::cout << "Non sequential data, current onset: " << midi_tick_onset
                                      << " is bigger than previous onset: "
                                      << previous_midi_tick_onset << std::endl;
                            return false;
                        }
                        
                        float midi_tick_duration = getMidiTickDuration(note_duration,note_onset);
                        if (midi_tick_duration <= 0)
                        {
                            std::cout << "Invalid duration: " << midi_tick_duration << std::endl;
                            return false;
                        }

                        int note_value = -1;
                        try{
                            note_value = stoi(note_information[i][note_value_index]);
                        }
                        catch(const std::exception& e){
                            std::cout << "Invalid input: " << e.what() << std::endl;
                            return false;
                        }
                        
                        juce::MidiMessage message_on (juce_message_on ,note_value, note_velocity);
                        message_on.setTimeStamp(midi_tick_onset);
                        juce::MidiMessage message_off (juce_message_off, note_value, 0);
                        message_off.setTimeStamp(midi_tick_onset + midi_tick_duration);
                        m_midi_sequence.addEvent(message_on);
                        m_midi_sequence.addEvent(message_off);
                    }
                }
            }
        }
        m_midi_file.addTrack(m_midi_sequence);
    }
    return true;
}

bool
MidiFileGenerator::writeMidiFile() const
{
    juce::FileOutputStream file_output_stream (m_midi_file_output);
    file_output_stream.setPosition(0);
    file_output_stream.truncate();
    if(file_output_stream.failedToOpen())
    {
        std::cout << "File: " << file_output_stream.getFile().getFileName() << " failed to open." << std::endl;
        return false;
    }
    m_midi_file.writeTo(file_output_stream);
    return true;
}
