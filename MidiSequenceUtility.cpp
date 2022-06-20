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
#include <array>

static const int test_scaling = 10;
static const int test_divison_scaling = 2;

bool
SequenceUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events, const bool& ppq_is_480)
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
                    if (ppq_is_480)
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
