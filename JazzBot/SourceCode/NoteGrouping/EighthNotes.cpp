//
//  EighthNotes.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/06/2022.
//

#include "EighthNotes.hpp"
#include "../Utility/SequenceUtility.hpp"
#include "../DatabaseConstants.h"
#include "../SequenceTypes.h"

#include <stdio.h>
#include <iostream>
#include <array>
#include <regex>

static const int quaver_length = 240;
static const double dotted_quaver = 360;
static const double dotted_semi = 180;
static const double offbeat_dotted_semi = 150; // this value is more leniant if its on an offbeat. Not really a dotted semi then is it
static const std::size_t grouping_limit = 10;

char apostrophe = '\'';

MidiSequence
EighthNotes::getModifiedSequence(const MidiSequence& midi_events) const
{
    std::array<BeatMarkers,2> beat_markers;
    MidiSequence eighth_note_groupings;
    for (std::size_t i = 0; i < midi_events.size(); i++)
    {
        beat_markers = getBeatMarkers(i, midi_events);
        std::size_t increment = 1;
        bool found_grouping = false;
        MidiSequence eighth_note_grouping;
        increment = findEighthNoteGrouping(increment,
                                           i,
                                           midi_events,
                                           beat_markers[0],
                                           beat_markers[1],
                                           eighth_note_grouping,
                                           true,
                                           found_grouping);
        if (eighth_note_grouping.size() > 2)
        {
            // this and the return is what is different
            eighth_note_groupings.insert(eighth_note_groupings.end(),
                                         eighth_note_grouping.begin(),
                                         eighth_note_grouping.end());
        }
        if (increment > 1)
        {
            i += increment - 1;
        }
    }
    return eighth_note_groupings;
}

void
EighthNotes::getSQLInsertStatements(const std::vector<Sequence>& sequences, std::vector<std::string>& sql_insert_statements) const
{
    EighthNoteGroupingData eighth_note_data;
    for (std::size_t sequence_index = 0; sequence_index < sequences.size(); sequence_index++)
    {
        std::array<BeatMarkers,2> beat_markers;
        unsigned int grouping_counter = 0;
        for (std::size_t midi_sequence_index = 0;
             midi_sequence_index < sequences[sequence_index].m_midi_sequence.size();
             midi_sequence_index++)
        {
            beat_markers = getBeatMarkers(midi_sequence_index, sequences[sequence_index].m_midi_sequence);
            std::size_t increment = 1;
            bool found_grouping = false;
            MidiSequence eighth_note_grouping;
            bool beat_type = beat_markers[0].second;
            increment = findEighthNoteGrouping(increment,
                                               midi_sequence_index,
                                               sequences[sequence_index].m_midi_sequence,
                                               beat_markers[0],
                                               beat_markers[1],
                                               eighth_note_grouping,
                                               true,
                                               found_grouping);
            std::size_t index = 0;
            if (eighth_note_grouping.size() > 2) // if its found a grouping larger that 2, then store it
            {
                calculateNoteGroupingKeys(eighth_note_grouping,
                                          index,
                                          beat_type,
                                          sequences[sequence_index].m_chord_sequence,
                                          eighth_note_data,
                                          sequences[sequence_index].m_song_information.m_title,
                                          sequences[sequence_index].m_song_information.m_time_signature,
                                          grouping_counter);
            }
            if (increment > 1)
            {
                midi_sequence_index += increment - 1;
            }
        }
    }
    constructSQLInsertStatements(sql_insert_statements, eighth_note_data);
}
    
std::string
EighthNotes::getDatabaseCreationSQL() const
{
    
    // return this overriden virtual methods from note_grouping
    std::string sql_create_db = "CREATE TABLE NOTEGROUPING("
                          "ID INT PRIMARY KEY     NOT NULL, "
                          "FILENAME         TEXT    NOT NULL, "
                          "CHORD            TEXT    NOT NULL, "
                          "STARTINGNOTE     TEXT    NOT NULL, "
                          "GROUPSIZE        TEXT    NOT NULL, "
                          "DIRECTION        TEXT    NOT NULL, "
                          "NEXTCHORD        TEXT    NOT NULL, "
                          "BEAT             TEXT    NOT NULL, "
                          "LOCATION         TEXT    NOT NULL, "
                          "GROUPINGNUM      TEXT    NOT NULL, "
                          "NOTES            TEXT    NOT NULL);";
    return sql_create_db;
}

std::string
EighthNotes::getDatabaseName() const
{
    return eighth_note_groupings_db_string;
}

std::string
EighthNotes::getDatabaseDirectory() const
{
    return note_groupings_directory;
}

void
EighthNotes::constructSQLInsertStatements(std::vector<std::string>& sql_insert_statements, const EighthNoteGroupingData& eighth_note_data) const
{
    for (std::size_t index = 0; index < eighth_note_data.size(); index++) // this could go in a seprate function
    {
        std::string file_name = eighth_note_data[index].m_file_name;
        if (std::count(file_name.begin(), file_name.end(), apostrophe) != 0)
        {
            file_name = std::regex_replace(file_name, std::regex("'"), "''");
        }
        std::string sql = "INSERT INTO NOTEGROUPING VALUES(" + std::to_string(index + 1) + // rewrite with std::format
                            ",'" + file_name + "'," +
                            "'" + eighth_note_data[index].m_chord + "'," +
                            "'" + eighth_note_data[index].m_starting_note + "'," +
                            "'" + eighth_note_data[index].m_group_size + "'," +
                            "'" + eighth_note_data[index].m_direction + "'," +
                            "'" + eighth_note_data[index].m_next_chord + "'," +
                            "'" + eighth_note_data[index].m_beat + "'," +
                            "'" + eighth_note_data[index].m_location + "'," +
                            "'" + eighth_note_data[index].m_grouping_number + "'," +
                            "'" + eighth_note_data[index].m_notes + "');";
        sql_insert_statements.push_back(sql);
    }
}

std::array<BeatMarkers, 2>
EighthNotes::getBeatMarkers(const std::size_t& note_index, const MidiSequence& midi_events) const
{
    std::array<BeatMarkers, 2> beat_markers;
    double note_on_timestamp = midi_events[note_index].note_on;
    // early version of function, can be improed
    int closest_eigth_note_multiplier = static_cast<int>(note_on_timestamp/quaver_length);
    double closet_eith_note = quaver_length * closest_eigth_note_multiplier;
    beat_markers[0].first = closet_eith_note;
    beat_markers[1].first = closet_eith_note + beat_length;
    
    if (closest_eigth_note_multiplier % 2 == 0)
    {
        // its on an onbeat
        beat_markers[0].second = true;
        beat_markers[1].second = false;
    }
    else
    {
        // offbeat
        beat_markers[0].second = false;
        beat_markers[1].second = true;
    }
    return beat_markers;
}

std::size_t
EighthNotes::findEighthNoteGrouping(std::size_t& increment,
                                    const std::size_t& index,
                                    const MidiSequence& midi_events,
                                    BeatMarkers& beat_marker_1,
                                    BeatMarkers& beat_marker_2,
                                    MidiSequence& eigth_notes_midi_sequence,
                                    const bool& first_time,
                                    bool& found_grouping) const
{
    if (index + increment < midi_events.size())
    {
        std::size_t first_index = index + increment - 1;
        // for now these sets of conditions will have to do, i think i can always go back and edit them if i need to.
        if (increment < grouping_limit && // limit for 8th note groupings
           midi_events[index + increment].note_on < (beat_marker_2.first+20) &&
            (midi_events[first_index].duration < dotted_quaver ) &&
            midi_events[index + increment].note_on - midi_events[first_index].note_on < dotted_quaver &&
            ((midi_events[index + increment].note_on - midi_events[first_index].note_on > dotted_semi &&
            beat_marker_1.second) ||
            (midi_events[index + increment].note_on - midi_events[first_index].note_on > offbeat_dotted_semi &&
            !beat_marker_1.second)))
        {
            double duration = midi_events[index+increment].duration;
            if (duration > dotted_quaver)
            {
                duration = 240; // swung eigth
                double note_off = midi_events[index + increment].note_on + 120;
                eigth_notes_midi_sequence.push_back({midi_events[index + increment].note_value,
                                                     midi_events[index + increment].note_on,
                                                     note_off,
                                                     duration});

            }
            else
            {
                eigth_notes_midi_sequence.push_back({midi_events[index + increment]});
            }

            beat_marker_1.first += quaver_length;
            beat_marker_1.second = ! beat_marker_1.second;
            beat_marker_2.first += quaver_length;
            beat_marker_2.second = ! beat_marker_2.second;

            increment += 1;
            found_grouping = true;
            increment = findEighthNoteGrouping(increment,
                                               index,
                                               midi_events,
                                               beat_marker_1,
                                               beat_marker_2,
                                               eigth_notes_midi_sequence,
                                               false,
                                               found_grouping);
        }
        if (first_time && found_grouping)
        {
            eigth_notes_midi_sequence.insert(eigth_notes_midi_sequence.begin() ,midi_events[index]);
        }
        return increment;
    }
    return increment;
}

void
EighthNotes::calculateNoteGroupingKeys(MidiSequence& grouping,
                                       std::size_t& index,
                                       const bool& starting_beat_type,
                                       const ChordSequence& chord_sequence,
                                       EighthNoteGroupingData& eighth_note_data,
                                       const std::string& file_name,
                                       const TimeSignature& time_signature,
                                       unsigned int& grouping_counter) const
{
    // This could be done more effeciently, by working out the key once then changing the values of the key
    // Instead it loops through multiple times to create the key.
    // Looking at how long it takes, i think it would be worth trying to change the performance of this
    
    // a lot of this could probably be shared by other groupings, so thats something to think about for the future

    // This could do with a small refactor first i should check that it actually works.
    if (index != grouping.size()) // we don't need these massive groupings, figure out how not to do it.
    {
        std::size_t new_index =  index + 1;
        calculateNoteGroupingKeys(grouping, new_index, starting_beat_type, chord_sequence, eighth_note_data, file_name, time_signature, grouping_counter);
        if (index > 1)
        {
            //std::cout << "New grouping with index: " << index << std::endl;
            std::size_t increment = 0;
            // get the chord degree here, they will all share that.
            bool beat_type = starting_beat_type;
            while (increment + 1 < index) // first time increment is 0, index is 6
            {
                //std::cout << "New grouping " << std::endl;
                std::string beat_type_str = (beat_type ? "D" : "O");
    
                std::string direction_str = std::to_string(grouping[index].note_value -
                                                           grouping[increment].note_value);
                // this function needs changing, it should return the root
                std::string next_chord_str = SequenceUtility::findChordForNote(grouping[index-1].note_on, chord_sequence, true, time_signature); // is this correct, why -1
                
                RootNote first_note = SequenceUtility::convertNoteValueToRootNote(grouping[increment].note_value);
                RootNote chord_root_key = SequenceUtility::findRootNoteForChord(grouping[increment].note_on, chord_sequence, time_signature);
                std::string starting_note_str = std::to_string(SequenceUtility::calculateRootNoteDifference(chord_root_key, first_note));
                
                std::string location_str = SequenceUtility::getLocation(grouping[increment].note_on, chord_sequence, time_signature);
                grouping_counter ++;
                std::string grouping_counter_str = std::to_string(grouping_counter);

                std::vector<std::string> chords; // lines below to get groupsize and chords could be done differently, i think its unnecessary.
                std::string notes; // this can probably stay...
                
                // Find the next chord by getting the last note and finding out the chord of the next bar
                for (std::size_t i = increment; i <= index; i++) // 0-5, 1-5, 2-5, 3-5
                {
                    // Find the chord for this note
                    std::string chord_degree = SequenceUtility::findChordForNote(grouping[i].note_on, chord_sequence, false, time_signature);
                    chords.push_back(chord_degree);
                    if (i != increment)
                    {
                        notes += std::to_string(grouping[i].note_value - grouping[i-1].note_value) + "&";
                    }
                }
                notes.pop_back();

                std::size_t current_chord_counter = 0;
                std::string current_chord = chords[0];
                std::string chords_str = chords[0] + "&";
                std::string group_size_str;
                bool found_second_chord = false;
                for (auto& chord : chords)
                {
                    if (chord == current_chord)
                    {
                        current_chord_counter ++;
                    }
                    else
                    {
                        found_second_chord = true;
                        chords_str += chord + "&";
                        group_size_str += std::to_string(current_chord_counter) + "&";
                        current_chord = chord;
                        current_chord_counter = 1;
                    }
                }
                if (!found_second_chord)
                {
                    chords_str = chords[0];
                    group_size_str = std::to_string(current_chord_counter);
                }
                else // remove trailing &
                {
                    chords_str.pop_back();
                    group_size_str += std::to_string(current_chord_counter);
                }

                eighth_note_data.push_back({chords_str,
                                            beat_type_str,
                                            starting_note_str,
                                            group_size_str,
                                            direction_str,
                                            next_chord_str,
                                            file_name,
                                            grouping_counter_str,
                                            location_str,
                                            notes});
                beat_type = !beat_type;
                increment ++;
            }
        }
    }
}















