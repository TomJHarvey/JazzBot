//
//  EighthNotes.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/06/2022.
//

#ifndef EighthNotes_hpp
#define EighthNotes_hpp

#include <stdio.h>
#include "NoteGrouping.hpp"

using BeatMarkers = std::pair<double, bool>;

class EighthNotes : public NoteGrouping
{
public:
    MidiSequence getModifiedSequence(const MidiSequence& midi_sequence) override;
    NoteGroupingData createDatabaseKeys(const std::vector<Sequence>& sequence) override;
    
private:
    std::size_t findEigthNoteGrouping(std::size_t& increment,
                                      const std::size_t& index,
                                      const MidiSequence& midi_events,
                                      BeatMarkers& beat_marker_1,
                                      BeatMarkers& beat_marker_2,
                                      MidiSequence& eigth_notes_midi_sequence,
                                      const bool& first_time,
                                      bool& found_grouping); // maybe a better way to do this
    
    std::array<BeatMarkers, 2> getBeatMarkers(const std::size_t& note_index,
                                              const MidiSequence& midi_events);
    
    
    void calculateNoteGroupingKeys(MidiSequence& grouping,
                                         std::size_t& index,
                                         const bool& starting_beat_type,
                                         const ChordSequence& chord_sequence,
                                         NoteGroupingData& eighth_note_data,
                                         const std::string& file_name,
                                         const TimeSignature& time_signature);
    
};

#endif /* EighthNotes_hpp */
