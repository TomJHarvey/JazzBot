//
//  EighthNoteSequence.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 13/07/2022.
//

#ifndef EighthNoteSequence_hpp
#define EighthNoteSequence_hpp

#include "NoteSequence.hpp"
#include "../SequenceTypes.h"
#include "../Utility/DatabaseUtility.hpp"

#include <stdio.h>

using NotePosition = std::pair<double, std::string>;
using SwingRatio = std::pair<double, double>;
using EighthNoteGroupingRange = std::pair<int, int>;

struct EighthNoteGroupingRows
{
    std::string m_starting_note;
    std::string m_beat;
    std::string m_chords;
    std::string m_group_size;
    std::string m_next_chord;
    std::string m_direction;
};

class EighthNoteSequence : public NoteSequence
{
public:
    EighthNoteSequence(const ChordSequence& chord_sequence,
                       const TimeSignature& time_signature,
                       const SwingRatio& swing_ratio,
                       const EighthNoteGroupingRange& grouping_range);
    
    MidiSequence generateSequence(const std::size_t& number_of_choruses) override;
    
private:
    std::string convertMidiNoteToStartingNote(const int& last_note,const double& note_position) const;
    
    std::string generateSQLInsertQuery(const NotePosition& note_position,
                                                           const std::string& first_chord_str,
                                                           const std::string& starting_note_str,
                                                           const std::string& beat_type_str,
                                                           const int& grouping_length,
                                                           const std::string& direction) const;
    
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
