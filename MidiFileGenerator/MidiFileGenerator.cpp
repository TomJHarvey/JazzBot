//
//  MidiFileGenerator.cpp
//  MidiFileGenerator
//
//  Created by Tom Harvey on 27/03/2022.
//

#include "MidiFileGenerator.hpp"
#include "../Utility/Utility.hpp"

static const std::size_t note_information_csv_row_size = 3;
static const std::size_t bar_onset_index_onset = 2;
static const std::size_t note_onset_index_duration = 0;
static const std::size_t note_onset_index_onset = 1;
static const std::size_t note_onset_index_value = 2;
static const int time_sig_quarter_note = 4; // this will be retrieved from the web scraper
static const int ppqn = 480; // pulses per quarter note

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
    bool result = true;
    std::vector<std::vector<std::string>> bar_information = Utility::parseCsvFile(m_bar_onset_file_name);
    float previous_bar_position = -1.0f;
    float previous_beat_onset = -1.0f;
    
    if (!bar_information.empty())
    {
        for(std::size_t i = 0; i < bar_information.size(); i++)
        {
            if (bar_information[i].size() == note_information_csv_row_size)
            {
                if (i != 0) // check its not the first row which contains strings not integers
                {
                    float bar_onset = std::stof(bar_information[i][bar_onset_index_onset].c_str()); // TODO: no error handling
                    float quater_note_increment = 0.0f;
                    
                    if (i + 1 < bar_information.size()) // check it can access the next bar
                    {
                        float next_bar_onset =
                            std::stof(bar_information[i + 1][bar_onset_index_onset].c_str()); // TODO: no error handling
                        quater_note_increment =
                            (next_bar_onset - bar_onset) / time_sig_quarter_note;
                    }
                    else // use the previous bars spacing for the last bar as there is no next bar to make the calculation
                    {
                        float previous_bar_onset =
                            std::stof(bar_information[i - 1][bar_onset_index_onset].c_str()); // TODO: no error handling
                        quater_note_increment =
                            (bar_onset - previous_bar_onset) / time_sig_quarter_note;
                    }
                        
                    for (int quater_note_index = 0;
                         quater_note_index < time_sig_quarter_note;
                         quater_note_index ++)
                    {
                        float beat_onset = bar_onset + (quater_note_index * quater_note_increment);
                        m_beat_onsets.push_back(beat_onset);
                        if(previous_beat_onset > 0)
                        {
                            m_beat_lengths.push_back(beat_onset - previous_beat_onset);
                        }
                        previous_beat_onset = beat_onset;
                    }
                    previous_bar_position = bar_onset;
                }
            }
        }
    }
    return result; // TODO: if there are any exceptions thrown it will be false
}

void
MidiFileGenerator::initMidiFile()
{
    m_midi_file.setTicksPerQuarterNote(ppqn);
    //TODO: this needs to be in an output folder and to override a file with the same name
    juce::String file_location = m_project_path + juce::String("/test22") + juce::String(".mid");
    m_midi_file_output = file_location;
}

bool
MidiFileGenerator::setCurrentBeat(const float& note_onset)
{
    if (m_current_beat + 1 == m_beat_onsets.size())
    {
        if (note_onset > (m_beat_onsets[m_current_beat] + m_beat_lengths[m_current_beat] ))
        {
            return false;
        }
        return true; // the current bar is the last bar
    }
    else
    {
        if (m_beat_onsets[m_current_beat] < note_onset &&
            note_onset < m_beat_onsets[m_current_beat + 1])
        {
            return true; // return the beat number for the current note
        }
        else //current note is not in this beat
        {
            m_current_beat += 1;
            return setCurrentBeat(note_onset);
        }
    }
}

float
MidiFileGenerator::getMidiTickDuration(const float& note_duration,
                                       const float& note_onset) const
{
   float midi_tick_duration = 0.0f;
   float relative_note_duration = note_onset + note_duration;
   
   // this isn't right as it doesn't carry across the beats
   // it could check where the relative note duratino is. What beat its in.
   // if its not in the current beat, it will loop through each beat and apply the over spill to each one
   // then it wil stitch them together
   
   if (m_current_beat + 1 < m_beat_onsets.size() &&
       relative_note_duration > m_beat_onsets[m_current_beat + 1])
   {
       float under_spill = m_beat_onsets[m_current_beat + 1] - note_onset;
       float over_spill = relative_note_duration - m_beat_onsets[m_current_beat + 1];
       float under_spill_relative_duration = ppqn * (under_spill/m_beat_lengths[m_current_beat]);
       float over_spill_relative_duration = ppqn * (over_spill/m_beat_lengths[m_current_beat + 1]);
       midi_tick_duration =  under_spill_relative_duration + over_spill_relative_duration;
       
       float normal_midi_tick_duration = ppqn * (note_duration/m_beat_lengths[m_current_beat]);
       std::cout << "over spill note" << std::endl;
       std::cout << "modified time =  " << midi_tick_duration << std::endl;
       std::cout << "normal time =  " << normal_midi_tick_duration << std::endl;
   }
   else
   {
       std::cout << "normal note" << std::endl;
       midi_tick_duration = ppqn * (note_duration/m_beat_lengths[m_current_beat]);
   }
   return midi_tick_duration;
}

float
MidiFileGenerator::getMidiTickOnset(const float& note_onset) const
{
    // TODO: this needs tidying up
    return (ppqn * ((note_onset-m_beat_onsets[m_current_beat])/m_beat_lengths[m_current_beat])) + (ppqn * m_current_beat);
}

bool
MidiFileGenerator::isNoteBeforeFirstBar(const float& note_onset) const
{
    return (note_onset < m_beat_onsets[0] ? true : false);
}

void
MidiFileGenerator::writeSequence()
{
    std::vector<std::vector<std::string>> note_information = Utility::parseCsvFile(m_note_onset_file_name);
    if (!note_information.empty())
    {
        for (std::size_t i = 0; i < note_information.size(); i++)
        {
            if (i != 0) // ignore headings
            {
                if (note_information[i].size() == 3)
                {
                    // TODO: error handling for stof?
                    float note_onset = stof(note_information[i][note_onset_index_onset]);
                    
                    if (isNoteBeforeFirstBar(note_onset))
                    {
                        // TODO: store the note values before first bar, just not durations
                        std::cout << "Notes before first bar will not be included" << std::endl;
                    }
                    else
                    {
                        float note_duration = stof(note_information[i][note_onset_index_duration]);
                        (void)setCurrentBeat(note_onset); //TODO: if false handle
                        float midi_tick_onset = getMidiTickOnset(note_onset);
                        float midi_tick_duration = getMidiTickDuration(note_duration,note_onset);
                        
                        // TODO: if duration is 0, handle
                        // TODO: ensure onset is sequential, so bigger than the previous one to be safe?

                        int note_value =  stoi(note_information[i][note_onset_index_value]);
                        
                        juce::MidiMessage message_on (0x90 ,note_value, 127); // velocity 127, should have constant
                        message_on.setTimeStamp(midi_tick_onset);
                        juce::MidiMessage message_off (0x80, note_value, 0); // hex values also have constant
                        message_off.setTimeStamp(midi_tick_onset + midi_tick_duration);
                        m_midi_sequence.addEvent(message_on);
                        m_midi_sequence.addEvent(message_off);
                    }
                }
            }
        }
        m_midi_file.addTrack(m_midi_sequence);
    }
}

bool
MidiFileGenerator::writeMidiFile() const
{
    juce::FileOutputStream file_output_stream (m_midi_file_output);
    jassert(!file_output_stream.failedToOpen()); // TODO: handle, false if fail
    m_midi_file.writeTo(file_output_stream);
    return true;
}
