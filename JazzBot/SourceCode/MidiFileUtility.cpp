//
//  MidiFileReader.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#include "MidiFileUtility.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

bool
MidiFileUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events)
{
    juce::MidiFile midi_file;
    juce::FileInputStream fi_stream(file);
    jassert(fi_stream.openedOk());
    midi_file.readFrom(fi_stream);
    
    // This is so if notes overlap it can pars them correctly
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
                current_note_ons.insert(std::make_pair(midi_event_holder->message.getNoteNumber(),
                                                       note_on_timestamp));
            }
            else if (midi_event_holder->message.isNoteOff())
            {
                int note_number = midi_event_holder->message.getNoteNumber();
                
                // retrieve the current notes corresponding note on value
                double note_on_timestamp = current_note_ons[note_number];
                
                double note_off_timestamp = midi_event_holder->message.getTimeStamp();
                
                midi_events.push_back({note_number, note_on_timestamp, note_off_timestamp});
                
                // erase the note as its no longer being played
                current_note_ons.erase(note_number);
            }
        }
    }
    return (!midi_events.empty());
}
