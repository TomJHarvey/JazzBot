//
//  NoteGrouping.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/06/2022.
//

#ifndef NoteGrouping_hpp
#define NoteGrouping_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "../SequenceTypes.h"

struct NoteGroupingKey // used to hold the data for various note groupings for the database entry
{
    std::string m_chord;
    std::string m_beat;
    std::string m_starting_note;
    std::string m_group_size;
    std::string m_direction;
    std::string m_next_chord;
    std::string m_file_name;
};

using NoteGroupingData = std::vector<std::pair<NoteGroupingKey, std::vector<std::string>>>;

class NoteGrouping
{
public:
    virtual MidiSequence getModifiedSequence(const MidiSequence& midi_sequence) = 0;
    virtual NoteGroupingData createDatabaseKeys(const std::vector<Sequence>& sequence) = 0;
    virtual ~NoteGrouping() {}
    
    std::string findChordForNote(const double& note_on,
                                 const ChordSequence& chord_sequence,
                                 const bool& next_chord,
                                 const TimeSignature& time_signature);
    RootNote findRootNoteForChord(const double& note_on,
                                  const ChordSequence& chord_sequence,
                                  const TimeSignature& time_signature);
    RootNote convertNoteValueToRootNote(const int& note_value);
    int calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2);
    RootNote getChordLetter(const std::string& chord_str);
    
    
};

#endif /* NoteGrouping_hpp */
