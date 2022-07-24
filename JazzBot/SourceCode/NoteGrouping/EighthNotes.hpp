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

/**
    @brief Used to store the contents of each row in the the eighth note grouping database
 */
struct EighthNoteGrouping
{
    std::string m_chord;
    std::string m_beat;
    std::string m_starting_note;
    std::string m_group_size;
    std::string m_direction;
    std::string m_next_chord;
    std::string m_file_name;
    std::string m_grouping_number;
    std::string m_location;
    std::string m_notes;
};

/**
    @brief Used to store a NoteGroupingKey and the elements returned from the 
 */
using EighthNoteGroupingData = std::vector<EighthNoteGrouping>;

/**
    @brief Used to keep track of whether the current note is on an on beat or an offbeat. The bool set to true is an onbeat and set to
           false is an offbeat. The first item in the pair is the timestamp of the note.
 */
using BeatMarkers = std::pair<double, bool>;

/**
    @brief This class is an implementation of NoteGrouping. It looks through a midi sequence for groups of consecutive eighth notes
           and has the functionality to either return a modified sequence showing only the modified sequence. Or to prepare the sql to
           store all the eighth note groupings in a database.
 */
class EighthNotes : public NoteGrouping
{
public:    
    /** Overrides NoteGrouping  */
    MidiSequence getModifiedSequence(const MidiSequence& midi_sequence) const override;
    
    /** Overrides NoteGrouping  */
    void getSQLInsertStatements(const std::vector<Sequence>& sequences,
                                std::vector<std::string>& sql_insert_statements) const override;
    
    /** Overrides NoteGrouping  */
    std::string getDatabaseCreationSQL() const override;
    
    /** Overrides NoteGrouping  */
    std::string getDatabaseName() const override;
    
    /** Overrides NoteGrouping  */
    std::string getDatabaseDirectory() const override;
    
private:
    /**
     @brief Constructs the SQL insert statements.
     @param sql_insert_statements to populate
     @param eighth_note_data to construct the statements from
     */
    void constructSQLInsertStatements(std::vector<std::string>& sql_insert_statements,
                                      const EighthNoteGroupingData& eighth_note_data) const;
    
    
    /**
     @brief Finds eighth note groupings from a midi sequence. Function is called recursively
     @param increment to append to the current  index of the overall midi sequence
     @param index for the position in the midi sequence
     @param midi_events to read from
     @param beat_marker_1 for the position in first beat in a quarter note range
     @param beat_marker_2 for the position in second beat in a quarter note range
     @param eigth_notes_midi_sequence to populate
     @param first_time if its the first time entering the recursive loop
     @param found_grouping if it's found a grouping
     @returns The size of the grouping
     */
    std::size_t findEighthNoteGrouping(std::size_t& increment,
                                       const std::size_t& index,
                                       const MidiSequence& midi_events,
                                       BeatMarkers& beat_marker_1,
                                       BeatMarkers& beat_marker_2,
                                       MidiSequence& eigth_notes_midi_sequence,
                                       const bool& first_time,
                                       bool& found_grouping) const; // maybe a better way to do this
    /**
     @brief Gets beat markers positioned at the start and end of a beat (This could be re used and moved elsewhere)
     @param note_index the index for the note in the current sequence
     @param midi_events the current sequence
     @returns The beat markers
     */
    std::array<BeatMarkers, 2> getBeatMarkers(const std::size_t& note_index,
                                              const MidiSequence& midi_events) const;
    
    /**
     @brief Calculate the eighth note grouping keys used to construct the insert statements for the database. Function is
            called recursively.
     @param grouping The grouping to construct keys out of
     @param index index for the current position in the grouping as the function is called recursively
     @param starting_beat_type whether the beat is an on or off beat
     @param chord_sequence for the current grouping
     @param eighth_note_data to populate
     @param file_name of the current sequence
     @param time_signature of the current sequence
     @param grouping_counter used to keep track of the number of total groupings
     */
    void calculateNoteGroupingKeys(MidiSequence& grouping,
                                   std::size_t& index,
                                   const bool& starting_beat_type,
                                   const ChordSequence& chord_sequence,
                                   EighthNoteGroupingData& eighth_note_data,
                                   const std::string& file_name,
                                   const TimeSignature& time_signature,
                                   unsigned int& grouping_counter) const;
    
};

#endif /* EighthNotes_hpp */
