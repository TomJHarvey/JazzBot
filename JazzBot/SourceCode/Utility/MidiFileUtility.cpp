//
//  MidiFileUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 13/07/2022.
//

#include "MidiFileUtility.hpp"

static const int juce_message_on = 0x90;
static const int juce_message_off = 0x80;
static const int note_velocity = 127;

bool
MidiFileUtility::writeMidiFile(const MidiSequence& midi_sequence)
{
    juce::MidiFile midi_file;
    midi_file.setTicksPerQuarterNote(480);
    juce::File output_directory = juce::File("/users/tom-harvey/test_file" + juce::String(".mid")); // write output place properly
    juce::MidiMessageSequence midi_message_sequence;
    juce::FileOutputStream file_output_stream(output_directory);
    
    for (auto& midi_event : midi_sequence)
    {
        juce::MidiMessage message_on (juce_message_on ,midi_event.note_value, note_velocity);
        message_on.setTimeStamp(midi_event.note_on);
        juce::MidiMessage message_off (juce_message_off, midi_event.note_value, 0);
        message_off.setTimeStamp(midi_event.note_on + midi_event.duration);
        midi_message_sequence.addEvent(message_on);
        midi_message_sequence.addEvent(message_off);
    }
        
    midi_file.addTrack(midi_message_sequence);
    file_output_stream.setPosition(0);
    file_output_stream.truncate();
    if(file_output_stream.failedToOpen())
    {
        std::cout << "File: " << file_output_stream.getFile().getFileName()
                  << " failed to open." << std::endl;
        return false;
    }

    midi_file.writeTo(file_output_stream);
    return true;
}
