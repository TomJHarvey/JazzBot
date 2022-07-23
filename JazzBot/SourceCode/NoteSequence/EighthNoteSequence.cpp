//
//  EighthNoteSequence.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 13/07/2022.
//

#include "EighthNoteSequence.hpp"
#include "../Utility/DatabaseUtility.hpp"
#include "../NoteGrouping/NoteGrouping.hpp"
#include "../DatabaseConstants.h"
#include <random>
#include <sqlite3.h>

static const int notes_index = 10;

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
    sqlite3* sql_database = DatabaseUtility::getDatabase(eighth_note_groupings_db_string, note_groupings_directory);
    MidiSequence midi_sequence; // midi sequence to return
    m_chorus_number = 0; // maybe reset at the end?
    int last_note = 72; // used to store the last note to  find the starting note of the next grouping F5
    midi_sequence.push_back({last_note, 0, m_swing_ratio.first, m_swing_ratio.first});
    
    // get the database poiinter to pass into select database
    
    SQLColumns sql_columns;
    sql_columns.push_back({"", notes_index});
    
    NotePosition note_position = std::make_pair(0, "D"); // use constant?
    while (m_chorus_number < number_of_choruses)
    {
        if (note_position.first == -160)
        {
            std::cout << "wrong" << std::endl;
        }
        
        
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
        
        std::string insert_query = generateSQLInsertQuery(note_position,
                                                          current_chord_str,
                                                          starting_note_str,
                                                          beat_type_str,
                                                          grouping_length,
                                                          direction_str);
        
        DatabaseUtility::selectFromDatabase(insert_query, sql_database, sql_columns);
        if (!sql_columns[0].m_data.empty())
        {
            appendEighthNoteGroupingToMidiSequence(sql_columns[0].m_data, midi_sequence, note_position, last_note);
        }
        else // if it can't find an entry with this size, it will try to use on of size 3
        {
            const int new_grouping_length = 3;
            insert_query = generateSQLInsertQuery(note_position,
                                                   current_chord_str,
                                                   starting_note_str,
                                                   beat_type_str,
                                                   new_grouping_length,
                                                   direction_str);
            
            DatabaseUtility::selectFromDatabase(insert_query, sql_database, sql_columns);
            if (!sql_columns[0].m_data.empty())
            {
                grouping_length = new_grouping_length; // override the grouping length to be the new one
                appendEighthNoteGroupingToMidiSequence(sql_columns[0].m_data, midi_sequence, note_position, last_note);
            }
            else // the grouping length from the original will be used to display rests
            {
                std::cout << "No sequence created grouping length = " << grouping_length << std::endl;
            }
        }
        for (int i = 0; i < grouping_length - 1; i++)
        {
            incrementNotePosition(note_position); // add rests
        }
        sql_columns[0].m_data = ""; // very important, needs to reset for next grouping
    }
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
        std::cout << "reset midi sequnece" << std::endl;
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

std::string
EighthNoteSequence::generateSQLInsertQuery(const NotePosition& note_position,
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
    
    // calculate insert query
    
    std::string direction_str;
    if (rows.m_direction == "up")
    {
        direction_str = " AND DIRECTION >0";
    }
    else if (rows.m_direction == "down")
    {
        direction_str = " AND DIRECTION <0";
    }

    std::string sql = "SELECT * FROM 'NOTEGROUPING' WHERE STARTINGNOTE='" + rows.m_starting_note + "'" +
                      " AND BEAT='"  + rows.m_beat + "'" +
                      " AND CHORD='"  + rows.m_chords + "'" +
                      " AND GROUPSIZE='"  + rows.m_group_size + "'" +
                      direction_str +
                      //" AND NEXTCHORD='"  + next_chord + "'" +
                      " order by RANDOM() LIMIT 1;";
    
    return sql;
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

