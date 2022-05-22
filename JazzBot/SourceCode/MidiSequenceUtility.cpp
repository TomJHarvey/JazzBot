//
//  MidiFileReader.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#include "MidiSequenceUtility.hpp"
#include "ChordUtility.hpp"
#include "SequenceUtility.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

static const int beat_length = 480;
static const int quaver_length = 240;
static const double dotted_quaver = 360;
static const double dotted_semi = 180;
static const double offbeat_dotted_semi = 150; // this value is more leniant if its on an offbeat. Not really a dotted semi then is it
static const int test_scaling = 10;
static const int test_divison_scaling = 2;

bool
MidiSequenceUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events, bool test)
{
    juce::MidiFile midi_file;
    juce::FileInputStream fi_stream(file);
    jassert(fi_stream.openedOk());
    midi_file.readFrom(fi_stream);
    
    // This is so if notes overlap it can parse them correctly
    std::map<int, double> current_note_ons;
    
    for (int num_tracks = 0; num_tracks < midi_file.getNumTracks(); num_tracks ++)
    {
        const juce::MidiMessageSequence* midi_message_sequence = midi_file.getTrack(num_tracks);
        for (int midi_event = 0; midi_event < midi_message_sequence->getNumEvents(); midi_event ++)
        {
            const juce::MidiMessageSequence::MidiEventHolder * midi_event_holder =
                midi_message_sequence->getEventPointer(midi_event);
            
            if (midi_event_holder->message.isNoteOn())
            {
                const double note_on_timestamp = midi_event_holder->message.getTimeStamp();
                
                // check for duplicate? Doesn't really matter, its hndled in the next else if
                current_note_ons.insert(std::make_pair(midi_event_holder->message.getNoteNumber(),
                                                       note_on_timestamp));
            }
            else if (midi_event_holder->message.isNoteOff())
            {
                int note_number = midi_event_holder->message.getNoteNumber();
                
                // retrieve the current notes corresponding note on value
                double note_on_timestamp = current_note_ons[note_number];
                
                // this is to handle instances where the note of the same value overlaps
                if (note_on_timestamp != 0 || midi_events.size() == 0)
                {
                    double note_off_timestamp = midi_event_holder->message.getTimeStamp();
                    double duration = note_off_timestamp - note_on_timestamp;
                    if (!test)
                    {
                        midi_events.push_back({note_number,
                            note_on_timestamp,
                            note_off_timestamp,
                            duration});
                    }
                    else // the test files are written in ableton where the midi tick value is different
                    {
                        midi_events.push_back({note_number,
                                               (note_on_timestamp/test_divison_scaling)*test_scaling,
                                               (note_off_timestamp/test_divison_scaling)*test_scaling,
                                               (duration/test_divison_scaling)*test_scaling});
                    }
                }
                current_note_ons.erase(note_number);
            }
        }
    }
    return !midi_events.empty();
}

std::array<BeatMarkers, 2>
MidiSequenceUtility::getBeatMarkers(const std::size_t& note_index, const MidiSequence& midi_events)
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

MidiSequence // It would be good if this also returns a sequence as i could then display this information when looking in the output. And it would further help with refactoring
MidiSequenceUtility::getOnlyEighthNotes(const MidiSequence& midi_events)
{
    std::array<BeatMarkers,2> beat_markers;
    MidiSequence eighth_note_groupings;
    for (std::size_t i = 0; i < midi_events.size(); i++)
    {
        beat_markers = getBeatMarkers(i, midi_events);
        std::size_t increment = 1;
        bool found_grouping = false;
        MidiSequence eighth_note_grouping;
        increment = findEigthNoteGrouping(increment,
                                          i,
                                          midi_events,
                                          beat_markers[0],
                                          beat_markers[1],
                                          eighth_note_grouping,
                                          true,
                                          found_grouping);

        // this and the return is what is different
        eighth_note_groupings.insert(eighth_note_groupings.end(),
                                     eighth_note_grouping.begin(),
                                     eighth_note_grouping.end());

        if (increment > 1)
        {
            i += increment - 1;
        }
    }
    return eighth_note_groupings;
}

EighthNoteGroupingData
MidiSequenceUtility::getEighthNoteGroupingKeys(const std::vector<Sequence>& sequence)
{
    EighthNoteGroupingData eighth_note_data;
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
            increment = findEigthNoteGrouping(increment,
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
                calculateEighthNoteGroupingKeys(eighth_note_grouping,
                                                index,
                                                beat_markers[0].second,
                                                sequence[sequence_index].m_chord_sequence,
                                                eighth_note_data,
                                                sequence[sequence_index].m_song_information.m_title);
            }

            if (increment > 1)
            {
                midi_sequence_index += increment - 1;
            }
        }
    }
    return eighth_note_data;
}

void
MidiSequenceUtility::calculateEighthNoteGroupingKeys(MidiSequence& grouping,
                                                     std::size_t& index,
                                                     const bool& starting_beat_type,
                                                     const ChordSequence& chord_sequence,
                                                     EighthNoteGroupingData& eighth_note_data,
                                                     const std::string& file_name) // think i also need chords in key.
{

//

    
    // This could be done more effeciently, by working out the key once then changing the values of the key
    // Instead it loops through multiple times to create the key.
    // Looking at how long it takes, i think it would be worth trying to change the performance of this
    
    // this and the utility functions all really need a clean up, some repeated code etc. Its nearlyt there just needs a tidy.

    // This could do with a small refactor first i should check that it actually works.
    if (index != grouping.size()) // we don't need these massive groupings, figure out how not to do it.
    {
        std::size_t new_index =  index + 1;
        calculateEighthNoteGroupingKeys(grouping, new_index, starting_beat_type, chord_sequence, eighth_note_data, file_name);
        if (index > 2)
        {
            std::cout << "New grouping with index: " << index << std::endl;
            std::size_t increment = 0;
            // get the chord degree here, they will all share that.
            bool beat_type = starting_beat_type;
            while (increment + 1 < index) // first time increment is 0, index is 6
            {
                std::cout << "New grouping " << std::endl;
                std::string beat_type_str = (beat_type ? "D" : "O");
            
    
                std::string direction_str = std::to_string(grouping[grouping.size() -1].note_value -
                                                           grouping[increment].note_value);
                // this function needs changing, it should return the root
                std::string next_chord_str = ChordUtility::findChordForNote(grouping[index-1].note_on, chord_sequence, true); // is this correct, why -1
                
                RootNote first_note = ChordUtility::convertNoteValueToRootNote(grouping[increment].note_value);
                RootNote chord_root_key = ChordUtility::findRootNoteForChord(grouping[increment].note_on, chord_sequence, false);
                
                // I need to get the root key from find chord for note.
                
                std::string starting_note_str = std::to_string(ChordUtility::calculateRootNoteDifference(chord_root_key, first_note));
                
                
                std::vector<std::string> chords;
                std::vector<int> notes;
                // Find the next chord by getting the last note and finding out the chord of the next bar
                for (std::size_t i = increment; i < index; i++) // 0-5, 1-5, 2-5, 3-5
                {
                    // Find the chord for this note
                    std::string chord_degree = ChordUtility::findChordForNote(grouping[i].note_on, chord_sequence, false);
                    chords.push_back(chord_degree);
                    if (i != increment)
                    {
                        notes.push_back(grouping[i].note_value - grouping[i-1].note_value);
                    }
                }

                std::size_t current_chord_counter = 0;
                std::string current_chord = chords[0];
                std::string chords_str;
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
                    group_size_str.pop_back();
                }

                EighthNoteGroupingKey grouping_key = {chords_str,
                                                      beat_type_str,
                                                      "starting_note_str",
                                                      group_size_str,
                                                      direction_str,
                                                      next_chord_str,
                                                      file_name};

                eighth_note_data.push_back({grouping_key, notes});

                std::cout << "C=" << chords_str << "B=" << beat_type_str << "SN=" << starting_note_str << "GS=" << group_size_str << "D=" << direction_str << "NC=" << next_chord_str << "FN=" << file_name << std::endl;
                for (auto& note : notes) {
                    std::cout << note << std::endl;
                }
                beat_type = !beat_type;
                increment ++;
            }
        }
    }
}

std::size_t
MidiSequenceUtility::findEigthNoteGrouping(std::size_t& increment,
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
            increment = findEigthNoteGrouping(increment,
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



//void
//MidiSequenceUtility::calculateEighthNoteGroupingKeys(MidiSequence& grouping,
//                                                     std::size_t& index,
//                                                     const bool& starting_beat_type,
//                                                     const ChordSequence& chord_sequence,
//                                                     EighthNoteGroupingData& eighth_note_data,
//                                                     const std::string& file_name)
//{
// bool beat_type = starting_beat_type;
//    std::string beat_type_str = (beat_type ? "D" : "O");
//    std::string starting_note_str = std::to_string(grouping[0].note_value);
//    std::string direction_str = std::to_string(grouping[grouping.size() -1].note_value -
//                                               grouping[0].note_value);
//
//    std::string next_chord_str = "";
//    next_chord_str = ChordUtility::findChordForNote(grouping[index-1].note_on, chord_sequence, true);
//    std::vector<std::string> chords;
//    std::vector<int> notes;
//
//    for (std::size_t i = 0; i < grouping.size() ; i++)
//    {
//        // Find the chord for this note
//        std::string chord_degree = ChordUtility::findChordForNote(grouping[i].note_on, chord_sequence, false);
//        chords.push_back(chord_degree);
//        if (i != 0)
//        {
//            notes.push_back(grouping[i].note_value - grouping[i-1].note_value);
//        }
//    }
//
//    std::size_t current_chord_counter = 0;
//    std::string current_chord = chords[0];
//    std::string chords_str;
//    std::string group_size_str;
//    bool found_second_chord = false;
//    for (auto& chord : chords)
//    {
//        if (chord == current_chord)
//        {
//            current_chord_counter ++;
//        }
//        else
//        {
//            found_second_chord = true;
//            chords_str += chord + "&";
//            group_size_str += std::to_string(current_chord_counter) + "&";
//            current_chord = chord;
//        }
//    }
//    if (!found_second_chord)
//    {
//        chords_str = chords[0];
//        group_size_str = std::to_string(current_chord_counter);
//    }
//    else // remove trailing &
//    {
//        chords_str.pop_back();
//        group_size_str.pop_back();
//    }
//
//    EighthNoteGroupingKey grouping_key = {chords_str,
//                                          beat_type_str,
//                                          starting_note_str,
//                                          group_size_str,
//                                          direction_str,
//                                          next_chord_str,
//                                          file_name};
//
//    eighth_note_data.push_back({grouping_key, notes});
//
//    std::cout << "C=" << chords_str << "B=" << beat_type_str << "SN=" << starting_note_str << "GS=" << group_size_str << "D=" << direction_str << "NC=" << next_chord_str << std::endl;
//    for (auto& note : notes) {
//        std::cout << note << std::endl;
//    }
//
//    // Here it would need to loop through, the things that are strings shouldn't be so they can be manipulated.
//    // For now i will stick with the slow function as it works
//}
