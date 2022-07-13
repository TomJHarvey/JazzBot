//
//  EighthNoteSequence.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 13/07/2022.
//

#include "EighthNoteSequence.hpp"
#include "../NoteGrouping/NoteGrouping.hpp"
#include <random>

static const char* eighth_note_groupings_db_string = "eighth_note_groupings.db"; // repeated from training dataview

EighthNoteSequence::EighthNoteSequence(const ChordSequence& chord_sequence,
                           const TimeSignature& time_signature,
                           const SwingRatio& swing_ratio,
                           const EighthNoteGroupingRange& grouping_range)
    : m_chord_sequence(chord_sequence)
    , m_time_signature(time_signature)
    , m_swing_ratio(swing_ratio)
    , m_grouping_range(grouping_range)
    , m_chorus_number(0)
{
    int bar_length = (static_cast<int>(time_signature) * beat_length);
    m_sequence_length = m_chord_sequence[m_chord_sequence.size()-1].m_chord_position + bar_length;
}

MidiSequence
EighthNoteSequence::generateSequence(const std::size_t& number_of_choruses)
{
    GroupingsDatabase groupings_database(eighth_note_groupings_db_string);
    MidiSequence midi_sequence; // midi sequence to return
    m_chorus_number = 0; // maybe reset at the end?
    int last_note = 72; // used to store the last note to  find the starting note of the next grouping F5
    midi_sequence.push_back({last_note, 0, m_swing_ratio.first, m_swing_ratio.first});
    
    NotePosition note_position = std::make_pair(0, "D"); // use constant?
    while (m_chorus_number < number_of_choruses)
    {
        // prepare rows for database unaffected by grouping length
        const std::string starting_note_str = convertMidiNoteToStartingNote(last_note, note_position.first);
        const std::string current_chord_str = NoteGrouping::findChordForNote(note_position.first, m_chord_sequence, false, m_time_signature);  // this can be reused in eighth notes, just pass ina  vector of note positions. (thats whats given in the other function
        const std::string beat_type_str = note_position.second;
        std::string direction_str;
        
        if (last_note < 54)
        {
            direction_str = "up";;
        }
        else if (last_note > 80)
        {
            direction_str = "down";
        }
        
        int grouping_length = generateRandomGroupingLength();
        
        // prepare row strings for database.
        EighthNoteGroupingRows rows = calculateEighthNoteGroupingRows(note_position,
                                                                      current_chord_str,
                                                                      starting_note_str,
                                                                      beat_type_str,
                                                                      grouping_length,
                                                                      direction_str);
        
        std::string eighth_note_grouping_string = groupings_database.selectEighthNoteGroupings(rows);
        if (!eighth_note_grouping_string.empty())
        {
            appendEighthNoteGroupingToMidiSequence(eighth_note_grouping_string, midi_sequence, note_position, last_note);
        }
        else
        {
            const int new_grouping_length = 3;
            rows = calculateEighthNoteGroupingRows(note_position,
                                                   current_chord_str,
                                                   starting_note_str,
                                                   beat_type_str,
                                                   new_grouping_length,
                                                   direction_str);
            
            eighth_note_grouping_string = groupings_database.selectEighthNoteGroupings(rows);
            if (!eighth_note_grouping_string.empty())
            {
                grouping_length = new_grouping_length;
                appendEighthNoteGroupingToMidiSequence(eighth_note_grouping_string, midi_sequence, note_position, last_note);
            }
            else
            {
                std::cout << "No sequence created grouping length = " << grouping_length << std::endl;
            }
        }
        for (int i = 0; i < grouping_length - 1; i++)
        {
            incrementNotePosition(note_position); // add rests
        }
    }
    // reset all variables back to 0?
    // then i would need an init function perhaps?
    // maybe not needed
    return midi_sequence;
}

std::string
EighthNoteSequence::convertMidiNoteToStartingNote(const int& last_note,
                                            const double& note_position) const
{
    // these three steps in a row are already use, refactor this down to one function, this static one wont be needed.
    RootNote first_note = NoteGrouping::convertNoteValueToRootNote(last_note);
    RootNote chord_root_key = NoteGrouping::findRootNoteForChord(note_position, m_chord_sequence, m_time_signature);
    std::string starting_note_str = std::to_string(NoteGrouping::calculateRootNoteDifference(chord_root_key, first_note));
    return starting_note_str;
}

void
EighthNoteSequence::appendEighthNoteGroupingToMidiSequence(const std::string& eighth_note_grouping_string,
                                                     MidiSequence& midi_sequence,
                                                     NotePosition& note_position,
                                                     int& last_note)
{
    NotePosition current_note_position = note_position;
    incrementNotePosition(current_note_position);
    int note_value = last_note;
    bool reset_note_position = false;
    size_t pos = 0;
    std::string token;
    std::string s = eighth_note_grouping_string;
    std::string delimiter = "&"; // constant?
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        addNoteToSequence(token,midi_sequence, current_note_position, note_value, reset_note_position);
        s.erase(0, pos + delimiter.length());
    }
    addNoteToSequence(s, midi_sequence, current_note_position, note_value, reset_note_position);
    
    if (reset_note_position)
    {
        note_position.first -= m_sequence_length;
    }
    
    last_note = midi_sequence[midi_sequence.size() -1].note_value;
}

void
EighthNoteSequence::addNoteToSequence(const std::string& increment_str,
                                MidiSequence& midi_sequence,
                                NotePosition& note_position,
                                int& note_value,
                                bool& reset_note_position)
{
    // put this in a function so that it increases when note position is incremented (this is sort oft he problem
    if (note_position.first >= m_sequence_length) // + bar length (get calculation from other function) // this would be a member variable when this function is no longer static
    {
        note_position.first = 0;
        m_chorus_number++;
        reset_note_position = true;
    }
    
    int increment = std::stoi(increment_str); // error check?
    note_value += increment;
    double note_on = note_position.first + (m_chorus_number * m_sequence_length);
    incrementNotePosition(note_position);
    double note_off = note_position.first + (m_chorus_number * m_sequence_length);;
    double duration = note_off - note_on;
    midi_sequence.push_back({note_value, note_on, note_off, duration});
}

EighthNoteGroupingRows
EighthNoteSequence::calculateEighthNoteGroupingRows(const NotePosition& note_position,
                                              const std::string& first_chord_str,
                                              const std::string& starting_note_str,
                                              const std::string& beat_type_str,
                                              const int& grouping_length,
                                              const std::string& direction) const
{
    NotePosition temp_note_position = note_position;
    incrementNotePosition(temp_note_position);
    int current_chord_counter = 1;
    std::string group_size_str;
    std::string current_chord_str = first_chord_str;
    std::string chords_str = current_chord_str + "&"; // constant?
    for (int index = 1; index < grouping_length; index ++)
    {
        std::string search_chord_str = NoteGrouping::findChordForNote(temp_note_position.first,
                                                                      m_chord_sequence,
                                                                      false,
                                                                      m_time_signature);
        if (search_chord_str == current_chord_str)
        {
            current_chord_counter ++;
        }
        else
        {
            group_size_str += std::to_string(current_chord_counter) + "&"; // constant?
            current_chord_str = search_chord_str;
            chords_str += current_chord_str + "&"; // constant?
            current_chord_counter = 1;
        }
        incrementNotePosition(temp_note_position); // increment beat type and change it to the next one
    }
    group_size_str += std::to_string(current_chord_counter);
    chords_str.pop_back();
    // starting_note_str = findChordForNote(note_position, chord_sequence, true, time_signature);
    EighthNoteGroupingRows rows{starting_note_str, beat_type_str, chords_str, group_size_str, "next_chord_str", direction};
    return rows;
}

int
EighthNoteSequence::generateRandomGroupingLength() const
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(m_grouping_range.first, m_grouping_range.second); // define the range
    int grouping_length = distr(gen);
    return grouping_length;
}

void
EighthNoteSequence::incrementNotePosition(NotePosition& current_note_position) const
{
    if (current_note_position.second == "D") // use constants?
    {
        current_note_position.first += m_swing_ratio.first;
        current_note_position.second = "O";
    }
    else
    {
        current_note_position.first += m_swing_ratio.second;
        current_note_position.second = "D";
    }
}

