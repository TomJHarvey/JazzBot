//
//  MidiFileReader.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#include "MidiFileUtility.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

static const int beat_length = 480;
static const int quaver_length = 240;
static const double dotted_quaver = 360;
static const double dotted_semi = 180;

bool
MidiFileUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events)
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
                
                // if they're overlapping but the same note, i can fix this later if i really want
                if (note_on_timestamp != 0)
                {
                    double note_off_timestamp = midi_event_holder->message.getTimeStamp();
                    double duration = note_off_timestamp - note_on_timestamp;
                    
                    midi_events.push_back({note_number, note_on_timestamp, note_off_timestamp, duration});
                    
                    // erase the note as its no longer being played
                }
                current_note_ons.erase(note_number);
            }
        }
    }
    return (!midi_events.empty());
}

MidiSequence
MidiFileUtility::getOnlyEigthNoteGroupings(const MidiSequence& midi_events)
{
    // this is a very early version of the function.
    // it will not be so rigid, it will check the notes and if they're just on the cusp of the bar, it will shift its analysis by that amount.
    // so like if it finds the note and sees its note on is jsut before a bar (say 50 away) then it will set the marker from that point until beat_marker-second - 50;
    
    BeatMarkers beat_markers[2];
    MidiSequence eigth_notes_midi_sequence;
    std::size_t midi_events_size = midi_events.size();
    
    for (std::size_t i = 0; i < midi_events_size; i++)
    {
        double note_on_timestamp = midi_events[i].note_on;
        int closest_eigth_note_multiplier = static_cast<int>(note_on_timestamp/quaver_length);
        double closet_eith_note = quaver_length * closest_eigth_note_multiplier;
        beat_markers[0].first = closet_eith_note;
        beat_markers[1].first = closet_eith_note + beat_length;
        
        // Need to make sure
        
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
        std::cout << "Finding 8th note grouping " << std::endl;
        std::size_t increment = 1;
        increment = findEigthNoteGrouping(increment, i, midi_events, beat_markers[0], beat_markers[1], eigth_notes_midi_sequence, true);
        if (increment > 1)
        {
            i += increment - 1;
        }
        
        
        std::cout << "Amount of notes analysed: " << i << std::endl;
        std::cout << "First 8th note grouping " << std::endl;
        for (auto& note : eigth_notes_midi_sequence)
        {
            std::cout << "Note on: " << note.note_value << std::endl;
//            std::cout << "Note off: " << note.note_off << std::endl;
//            std::cout << "Note value: " << note.note_value << std::endl;
        }
    }
    return eigth_notes_midi_sequence;
}

std::size_t
MidiFileUtility::findEigthNoteGrouping(std::size_t& increment, const std::size_t& index, const MidiSequence& midi_events , BeatMarkers& beat_marker_1,
                                       BeatMarkers& beat_marker_2, MidiSequence& eigth_notes_midi_sequence, const bool& first_time)
{
    if (index + increment < midi_events.size())
    {
        std::size_t first_index = index + increment - 1;
        // shouldn't need to check first marker
        
        std::cout << "If next midi event(" << midi_events[index + increment].note_value <<  ") note on: " << midi_events[index + increment].note_on << " is smaller than the second beat marker: " << beat_marker_2.first << std::endl;
        std::cout << "If current midi event(" << midi_events[first_index].note_value <<  ") duration: " << midi_events[first_index].duration << " Is smaller than 720 " << std::endl;
        //std::cout << "if next notes duration: " <<  midi_events[index + increment].duration << " is smaller than 720 " << std::endl;
        std::cout << "Next note on(" << midi_events[index + increment].note_value <<  ") minus current note on(" << midi_events[first_index].note_value <<  ") is smaller than 720: " <<  midi_events[index + increment].note_on - midi_events[first_index].note_on  << std::endl;
        
        if (midi_events[index + increment].note_on < (beat_marker_2.first+20) &&
            (midi_events[first_index].duration < dotted_quaver ) &&
            //(midi_events[index + increment].duration < dotted_quaver) &&
            midi_events[index + increment].note_on - midi_events[first_index].note_on < dotted_quaver &&
            (beat_marker_1.second == false ||
             (beat_marker_1.second == true &&
             midi_events[index + increment].note_on - midi_events[first_index].note_on > dotted_semi)))
        {
            
            // print out all these comparisons and compare with midi file.
            // shame i can't debug the actual note events, maybe restart computer...
            
            // found pair? i++
            double duration = midi_events[index+increment].duration;
            
            std::cout << "Next (" << midi_events[index+increment].note_value <<") note duration = " << duration << std::endl;
            
            if (duration > dotted_quaver)
            {
                duration = 240; // swung eigth
                double note_off = midi_events[index + increment].note_on + 240;
                
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

            std::size_t new_increment = increment + 1;
            increment = findEigthNoteGrouping(new_increment, index, midi_events, beat_marker_1, beat_marker_2, eigth_notes_midi_sequence, false);
            // eigth_notes_midi_sequence.push_front(midi_events[first_index]); // the beat_markers will be used here when setting the eigth note, so it knows whether its on offbeat or not
        }
        
        // if its in the first loop and some have been added. Then it will need to push back at the size - minus the increment
        
        if (increment > 1 && first_time)
        {
            std::cout << "First midi event = " << midi_events[index].note_value << std::endl;
            auto it = eigth_notes_midi_sequence.end() - (increment - 1);
            std::cout << "Position to increment = "<< it->note_value << std::endl;
            // check the duration here too.
            eigth_notes_midi_sequence.insert(eigth_notes_midi_sequence.end() - (increment - 1),midi_events[index] );
        }

        
        return (increment);
    }
    return 0;
}

