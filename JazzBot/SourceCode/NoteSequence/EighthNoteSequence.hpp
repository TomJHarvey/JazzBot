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

/**
    @brief Used to store the current note position, first item stores the timestamp, the second is D for down beat and O for offbeat.
 */
using NotePosition = std::pair<double, std::string>;

/**
    @brief Used to define the length of each squaver in a swung quaver pair
 */
using SwingRatio = std::pair<double, double>;

/**
    @brief Used to define the range in size that the eighth note grouping can be
 */
using EighthNoteGroupingRange = std::pair<int, int>;

/**
    @brief Contains the rows that will be extracted from the database (Even though not all are retrieved EighthNoteGrouping
           could be reused here.
 */
struct EighthNoteGroupingRows
{
    std::string m_starting_note;
    std::string m_beat;
    std::string m_chords;
    std::string m_group_size;
    std::string m_next_chord;
    std::string m_direction;
};

/**
    @brief This class is an implementation of NoteSequence. It is used to generate an eighth note sequence by retriveing data
           from the eigth note groupings database.
 */
class EighthNoteSequence : public NoteSequence
{
public:
    
    /**
        @brief Constructor
     */
    EighthNoteSequence(const ChordSequence& chord_sequence,
                       const TimeSignature& time_signature,
                       const SwingRatio& swing_ratio,
                       const EighthNoteGroupingRange& grouping_range);
    
    /** Overrides NoteGrouping  */
    MidiSequence generateSequence(const std::size_t& number_of_choruses) override;
    
private:
    
    /**
     @brief Converts the midi note to the a string containing the chord degree in relation to the chord. E.g if the midi starting note was
            60 (Which is C) over a Cmaj chord it would return 0. If it was over a Dminor chord it would return 10. If it was over a
            Bb it would return 2 etc.
     @param last_note the midi note to convert
     @param note_position the position of the note in the sequence
     @returns The starting note as a string
     */
    std::string convertMidiNoteToStartingNote(const int& last_note,const double& note_position) const;
    
    /**
     @brief Generates the SQL select query to find the eigtth note grouping for the specific situation it is in.
     @param first_chord_str the first chord that the grouping will be over
     @param starting_note_str the starting note as a degree in relation to the first chord (0-11)
     @param beat_type_str if its a down or off beat
     @param grouping_length the length of the grouping to generate
     @param direction if the direction needs to go up or down to keep it in range
     @returns The SQL select query
     */
    std::string generateSQLSelectQuery(const NotePosition& note_position,
                                       const std::string& first_chord_str,
                                       const std::string& starting_note_str,
                                       const std::string& beat_type_str,
                                       const int& grouping_length,
                                       const std::string& direction) const;
    
    /**
     @brief Adds the notes taken from the database to the MidiSequence object to be returned
     @param increment_str how much to increment the previous note value by
     @param midi_sequence to append to
     @param note_position the current note position of the last note
     @param note_value the last note inserted which will be updated
     @param reset_note_position if the note position has reached the end of the chord sequence
     */
    void addNoteToSequence(const std::string& increment_str,
                           MidiSequence& midi_sequence,
                           NotePosition& note_position,
                           int& note_value,
                           bool& reset_note_position);
    
    /**
     @brief Appends the retrived notes from the database to the midi sequence
     @param eighth_note_grouping_string note grouping defined as a series of increments and decrements retrieved from the database
     @param midi_sequence to append to
     @param note_position the current note position of the last note
     @param last_note the last note inserted which will be updated
     */
    void appendEighthNoteGroupingToMidiSequence(const std::string& eighth_note_grouping_string,
                                                MidiSequence& midi_sequence,
                                                NotePosition& note_position,
                                                int& last_note);
    
    /**
     @brief Generates a random grouping length
     @returns The random grouping length
     */
    int generateRandomGroupingLength() const;
    
    /**
     @brief Increments the note position by a swung eighth notes
     */
    void incrementNotePosition(NotePosition& current_note_position) const;

private:
    /** The chord sequence used for the generated sequence */
    ChordSequence m_chord_sequence;
    
    /** The time signature used for the generated sequence  */
    TimeSignature m_time_signature;
    
    /** The length of the sequence */
    double m_sequence_length;
    
    /** The swing ratio for the eighth notes */
    SwingRatio m_swing_ratio;
    
    /** The min and max size of the groupings  */
    EighthNoteGroupingRange m_grouping_range;
    
    /** The current chorus number */
    std::size_t m_chorus_number;
};

#endif /* EighthNoteSequence_hpp */
