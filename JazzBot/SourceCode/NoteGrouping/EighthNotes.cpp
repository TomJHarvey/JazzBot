//
//  EighthNotes.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/06/2022.
//

#include "EighthNotes.hpp"
#include "../SequenceTypes.h"

#include <stdio.h>
#include <iostream> //why here?
#include <array>

static const int quaver_length = 240;
static const double dotted_quaver = 360;
static const double dotted_semi = 180;
static const double offbeat_dotted_semi = 150; // this value is more leniant if its on an offbeat. Not really a dotted semi then is it

MidiSequence
EighthNotes::getModifiedSequence(const MidiSequence& midi_events)
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

NoteGroupingData
EighthNotes::createDatabaseKeys(const std::vector<Sequence>& sequence)
{
    NoteGroupingData eighth_note_data;
    for (std::size_t sequence_index = 0; sequence_index < sequence.size(); sequence_index++)
    {
        std::array<BeatMarkers,2> beat_markers;
        for (std::size_t midi_sequence_index = 0;
             midi_sequence_index < sequence[sequence_index].m_midi_sequence.size();
             midi_sequence_index++)
        {
            beat_markers = getBeatMarkers(midi_sequence_index, sequence[sequence_index].m_midi_sequence);
            std::size_t increment = 1;
            bool found_grouping = false;
            MidiSequence eighth_note_grouping;
            increment = findEighthNoteGrouping(increment,
                                               midi_sequence_index,
                                               sequence[sequence_index].m_midi_sequence,
                                               beat_markers[0],
                                               beat_markers[1],
                                               eighth_note_grouping,
                                               true,
                                               found_grouping);
            std::size_t index = 0;
            if (eighth_note_grouping.size() > 2)
            {
                calculateNoteGroupingKeys(eighth_note_grouping,
                                          index,
                                          beat_markers[0].second,
                                          sequence[sequence_index].m_chord_sequence,
                                          eighth_note_data,
                                          sequence[sequence_index].m_song_information.m_title,
                                          sequence[sequence_index].m_song_information.m_time_signature);
            }
            if (increment > 1)
            {
                midi_sequence_index += increment - 1;
            }
        }
    }
    return eighth_note_data;
}

std::array<BeatMarkers, 2>
EighthNotes::getBeatMarkers(const std::size_t& note_index, const MidiSequence& midi_events)
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
                                    bool& found_grouping)
{
    if (index + increment < midi_events.size())
    {
        std::size_t first_index = index + increment - 1;
        
//        if (!(midi_events[index + increment].note_on < (beat_marker_2.first+20))) // do i need this?
//        {
//            std::cout << std::endl;
//        }
//        if (!(midi_events[first_index].duration < dotted_quaver))
//        {
//            std::cout << std::endl;
//        }
//        if (!(midi_events[index + increment].note_on - midi_events[first_index].note_on < dotted_quaver))
//        {
//            std::cout << std::endl;
//        }
//        if (!(midi_events[index + increment].note_on - midi_events[first_index].note_on > dotted_semi) && beat_marker_1.second)
//        {
//           std::cout << midi_events[index + increment].note_on << " - " << midi_events[first_index].note_on << std::endl;
//        }
//        if (!(midi_events[index + increment].note_on - midi_events[first_index].note_on > offbeat_dotted_semi) && !beat_marker_1.second)
//        {
//           std::cout << midi_events[index + increment].note_on << " - " << midi_events[first_index].note_on << std::endl;
//        }
        
        // for now these sets of conditions will have to do, i think i can always go back and edit them if i need to.

        if (midi_events[index + increment].note_on < (beat_marker_2.first+20) &&
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
            eigth_notes_midi_sequence.push_back({midi_events[index + increment]});

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
                                             NoteGroupingData& eighth_note_data,
                                             const std::string& file_name,
                                             const TimeSignature& time_signature)
{
    // This could be done more effeciently, by working out the key once then changing the values of the key
    // Instead it loops through multiple times to create the key.
    // Looking at how long it takes, i think it would be worth trying to change the performance of this
    
    // a lot of this could probably be shared by other groupings, so thats something to think about for the future

    // This could do with a small refactor first i should check that it actually works.
    if (index != grouping.size()) // we don't need these massive groupings, figure out how not to do it.
    {
        std::size_t new_index =  index + 1;
        calculateNoteGroupingKeys(grouping, new_index, starting_beat_type, chord_sequence, eighth_note_data, file_name, time_signature);
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
    
                std::string direction_str = std::to_string(grouping[grouping.size() -1].note_value -
                                                           grouping[increment].note_value);
                // this function needs changing, it should return the root
                std::string next_chord_str = findChordForNote(grouping[index-1].note_on, chord_sequence, true, time_signature); // is this correct, why -1
                
                RootNote first_note = convertNoteValueToRootNote(grouping[increment].note_value);
                RootNote chord_root_key = findRootNoteForChord(grouping[increment].note_on, chord_sequence, time_signature);
                
                // I need to get the root key from find chord for note.
                
                std::string starting_note_str = std::to_string(calculateRootNoteDifference(chord_root_key, first_note));
                
                std::vector<std::string> chords;
                std::vector<std::string> notes;
                // Find the next chord by getting the last note and finding out the chord of the next bar
                for (std::size_t i = increment; i <= index; i++) // 0-5, 1-5, 2-5, 3-5
                {
                    // Find the chord for this note
                    std::string chord_degree = findChordForNote(grouping[i].note_on, chord_sequence, false, time_signature);
                    chords.push_back(chord_degree);
                    if (i != increment)
                    {
                        notes.push_back(std::to_string(grouping[i].note_value - grouping[i-1].note_value));
                    }
                }

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
                
//                std::cout << chords_str << std::endl;
//                std::cout << group_size_str << std::endl;

                NoteGroupingKey grouping_key = {chords_str,
                                                beat_type_str,
                                                starting_note_str,
                                                group_size_str,
                                                direction_str,
                                                next_chord_str,
                                                file_name};

                eighth_note_data.push_back({grouping_key, notes});

                //std::cout << "C=" << chords_str << "B=" << beat_type_str << "SN=" << starting_note_str << "GS=" << group_size_str << "D=" << direction_str << "NC=" << next_chord_str << "FN=" << file_name << std::endl;
//                for (auto& note : notes) {
//                    std::cout << note << std::endl;
//                }
                beat_type = !beat_type;
                increment ++;
            }
        }
    }
}













