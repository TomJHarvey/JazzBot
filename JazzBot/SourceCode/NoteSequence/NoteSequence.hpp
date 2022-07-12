//
//  EighthNoteSequence.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/06/2022.
//

#ifndef EighthNoteSequence_hpp
#define EighthNoteSequence_hpp

#include "../SequenceTypes.h"
#include "../NoteGrouping/GroupingsDatabase.hpp"

#include <stdio.h>

using NotePosition = std::pair<double, std::string>;
using SwingRatio = std::pair<double, double>;
using EighthNoteGroupingRange = std::pair<int, int>;

class NoteSequence
{
public:
    NoteSequence(const ChordSequence& chord_sequence,
                 const TimeSignature& time_signature,
                 const SwingRatio& swing_ratio,
                 const EighthNoteGroupingRange& grouping_range);
    
    MidiSequence generateEighthNoteSequence(const std::size_t& number_of_choruses);
    std::string convertMidiNoteToStartingNote(const int& last_note,const double& note_position) const;
    
private:
    EighthNoteGroupingRows calculateEighthNoteGroupingRows(const NotePosition& note_position,
                                                           const std::string& first_chord_str,
                                                           const std::string& starting_note_str,
                                                           const std::string& beat_type_str,
                                                           const int& grouping_length) const;
    
    void addNoteToSequence(const std::string& increment_str,
                           MidiSequence& midi_sequence,
                           NotePosition& note_position,
                           int& note_value,
                           bool& reset_note_position);
    
    
    void appendEighthNoteGroupingToMidiSequence(const std::string& eighth_note_grouping_string,
                                                MidiSequence& midi_sequence,
                                                NotePosition& note_position,
                                                int& last_note);
    
    int generateRandomGroupingLength() const;
    void incrementNotePosition(NotePosition& current_note_position) const;

private:
    ChordSequence m_chord_sequence;
    TimeSignature m_time_signature;
    double m_sequence_length;
    SwingRatio m_swing_ratio;
    EighthNoteGroupingRange m_grouping_range;
    std::size_t m_chorus_number;
};

#endif /* EighthNoteSequence_hpp */
