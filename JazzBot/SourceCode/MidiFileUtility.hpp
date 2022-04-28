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
    double duration;
};

using MidiSequence = std::vector<MidiNoteEvent>;
using BeatMarkers = std::pair<double, bool>;

class MidiFileUtility
{
public:
    // maybe not vector...
    // have different data structures it can load it into perhaps
    static bool parseMidiFile(const juce::File& file, MidiSequence& midi_events);
    static MidiSequence getOnlyEigthNoteGroupings(const MidiSequence& midi_events);
    
private:
    static std::size_t findEigthNoteGrouping(std::size_t& increment,
                                             const std::size_t& index,
                                      const MidiSequence& midi_events,
                                      BeatMarkers& beat_marker_1,
                                      BeatMarkers& beat_marker_2,
                                      MidiSequence& eigth_notes_midi_sequence,
                                      const bool& first_time); // maybe a better way to do this
};

#endif /* MidiFileUtility_hpp */
