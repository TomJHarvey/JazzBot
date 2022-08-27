//
//  MidiFileUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 13/07/2022.
//

#ifndef MidiFileUtility_hpp
#define MidiFileUtility_hpp

#include "../SequenceTypes.h"

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

/**
    @brief Utility class for reading and writing midi files
 */
class MidiFileUtility
{
public:

    /**
     @brief Writes a midi file
     @param midi_sequence to write to file
     @returns If successfully wrote midi file
     */
    static bool writeMidiFile(const MidiSequence& midi_sequence);
    
    /**
     @brief Parses a midi file into a MidiSequence object
     @param file to read from
     @param midi_events to populate
     @param ppq_is_480 if the pulses per quarter note is equal to 480
     @returns If successfully populated MidiSequence object.
     */
    static bool parseMidiFile(const juce::File& file, MidiSequence& midi_events, const bool& ppq_is_480);
};

#endif /* MidiFileUtility_hpp */
