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

class MidiFileUtility
{
public:
    static bool writeMidiFile(const MidiSequence& midi_sequence);
};

#endif /* MidiFileUtility_hpp */
