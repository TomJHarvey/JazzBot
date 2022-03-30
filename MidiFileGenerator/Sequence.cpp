//
//  Sequence.cpp
//  MIDI_FILE_GENERATOR
//
//  Created by Tom Harvey on 30/03/2022.
//

#include "Sequence.hpp"

static const int time_sig_quarter_note = 4; // this will be retrieved from the web scraper
static const int ppqn = 480; // pulses per quarter note

void
Sequence::setBeatInformation(const float& bar_onset,
                             const float& quarter_note_increment,
                             float& previous_beat_onset)
{
    // set the onset and length for each quarter note in the current bar
    for (int quater_note_index = 0;
         quater_note_index < time_sig_quarter_note;
         quater_note_index ++)
    {
        float beat_onset = bar_onset + (quater_note_index * quarter_note_increment);
        m_beat_onsets.push_back(beat_onset);
        if (previous_beat_onset > 0)
        {
            m_beat_lengths.push_back(beat_onset - previous_beat_onset);
        }
        previous_beat_onset = beat_onset;
    }
}

bool
Sequence::findBeat(const float& note_onset, std::size_t& beat) const
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
Sequence::getMidiTickDuration(const float& note_duration,
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
        
        midi_tick_duration =
            under_spill_relative_duration + last_beat_note_relative_duration + extra_beats;
    }
    else // no overlap, unless if last note where it doesn't matter the length
    {
        midi_tick_duration = ppqn * (note_duration/m_beat_lengths[m_current_beat]);
    }
    return midi_tick_duration;
}

float
Sequence::getMidiTickOnset(const float& note_onset) const
{
    float beats_elapsed = (ppqn * m_current_beat);
    float relative_note_onset = (note_onset-m_beat_onsets[m_current_beat]);
    return (ppqn * (relative_note_onset/m_beat_lengths[m_current_beat])) + beats_elapsed;
}

float
Sequence::calculateQuarterNoteIncrement(const float& next_bar_onset, const float& bar_onset)
{
    return ((next_bar_onset - bar_onset) / time_sig_quarter_note);
}

bool
Sequence::isNoteBeforeFirstBar(const float& note_onset) const
{
    return (note_onset < m_beat_onsets[0] ? true : false);
}

bool
Sequence::setCurrentBeat(const float& note_onset)
{
    return (findBeat(note_onset, m_current_beat));
}

int
Sequence::getPpqn()
{
    return ppqn;
}
