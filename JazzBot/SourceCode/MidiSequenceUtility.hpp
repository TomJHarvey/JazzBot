//
//  MidiSequenceUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#ifndef MidiSequenceUtility_hpp
#define MidiSequenceUtility_hpp

#include <stdio.h>
#include <vector>
#include <juce_core/juce_core.h>
#include <map>

#include "SequenceTypes.h"

class MidiSequenceUtility
{
public:
    static bool parseMidiFile(const juce::File& file, MidiSequence& midi_events, bool test = false);
    static MidiSequence getOnlyEighthNotes(const MidiSequence& midi_events);
    static void calculateEighthNoteGroupingKeys(const std::vector<Sequence>& sequence);
private:
    static std::size_t findEigthNoteGrouping(std::size_t& increment,
                                             const std::size_t& index,
                                             const MidiSequence& midi_events,
                                             BeatMarkers& beat_marker_1,
                                             BeatMarkers& beat_marker_2,
                                             MidiSequence& eigth_notes_midi_sequence,
                                             const bool& first_time,
                                             bool& found_grouping); // maybe a better way to do this
    
    static std::array<BeatMarkers, 2> getBeatMarkers(const std::size_t& note_index,
                                                     const MidiSequence& midi_events);
};

#endif /* MidiFileUtility_hpp */
