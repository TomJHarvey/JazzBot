//
//  MidiFileReader.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#include "MidiFileUtility.hpp"
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
MidiFileUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events, bool test)
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

MidiSequence // It would be good if this also returns a sequence as i could then display this information when looking in the output. And it would further help with refactoring
MidiFileUtility::getOnlyEighthNotes(const MidiSequence& midi_events)
{
    BeatMarkers beat_markers[2];
    MidiSequence eighth_note_groupings;
    std::size_t midi_events_size = midi_events.size();
    
    for (std::size_t i = 0; i < midi_events_size; i++) // maybe use an iterator and check its not at the end? Indexing would be hard though.
    {
        double note_on_timestamp = midi_events[i].note_on;
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

// This is the same as the above function except for a few minor changes, if possible it should be refactored.
void
MidiFileUtility::calculateEighthNoteGroupingKeys(const std::vector<Sequence>& sequence)
{
    BeatMarkers beat_markers[2];
    
    for (std::size_t j = 0; j < sequence.size(); j++)
    {
        std::size_t midi_events_size = sequence[j].m_midi_sequence.size();
        
        for (std::size_t i = 0; i < midi_events_size; i++)
        {
            double note_on_timestamp = sequence[j].m_midi_sequence[i].note_on;
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
            // Put this above beat marker thing in a function just to tidy it up a little bit.

            std::size_t increment = 1;
            bool found_grouping = false;
            MidiSequence eighth_note_grouping;
            increment = findEigthNoteGrouping(increment,
                                              i,
                                              sequence[j].m_midi_sequence,
                                              beat_markers[0],
                                              beat_markers[1],
                                              eighth_note_grouping,
                                              true,
                                              found_grouping);
            
            // make a vector containing a pair with the eighth note grouping and its coressponding chords
            

            
            
 
            
            // it will search backwards starting from the last grouping to the first
            // so in a 6 note grouping it would be 5-4, 5-3, 5-2, 5-1, 5-0
            // this would mean the beat type will change each time and the stopping note remains consistent.
            // each time it loops through it will flip the offbeat variable
            // it will calcuate the change in semitone by doing the last note minus the first
            
            // when indexing the array it will index the pair so that it can access the chord type.
            // the group size needs to be set depending ont he chord type, so itwill need to do this calculation each time
            // or if its two chords (and &) it can just move down the (&)
            
            // once its reached the end of the loop it will remove the last element, or set a variable to have a cap on the index.
            
            // the next time it enters the loop it will count back from 4-3,4-2,4-1,4-0
            // then it will repeat the same process

            // for each grouping it will add to the main vector of pairs of keys and notes
            
            // this will be returned from the function.
            
            // a function will then input that into a database
            
            //
            if (increment > 1)
            {
                i += increment - 1;
            }
        }
    }
}
std::size_t
MidiFileUtility::findEigthNoteGrouping(std::size_t& increment,
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
