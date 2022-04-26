//
//  MidiFileReader.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#ifndef MidiFileUtility_hpp
#define MidiFileUtility_hpp

#include <stdio.h>
#include <vector>
#include <juce_core/juce_core.h>

struct MidiNoteEvent
{
    int note_value;
    double note_on;
    double note_off;
};

using MidiSequence = std::vector<MidiNoteEvent>;

class MidiFileUtility
{
public:
    // maybe not vector...
    // have different data structures it can load it into perhaps
    static bool parseMidiFile(const juce::File& file, MidiSequence& midi_events);
};

#endif /* MidiFileUtility_hpp */
