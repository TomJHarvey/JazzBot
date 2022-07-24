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

/**
    @brief  This interface is designed for a specified grouping of notes to be selected from a midi file,
            by applying an algorithm to a midi sequence. The algorithm can then be applied to show only
            instances of the grouping in an entire sequence - getModifiedSequence()
            Or it can be used to extract all isntances of the grouping from a midi file and insert them
            into a database.
 */
class NoteGrouping
{
public:
    
    /**
     @brief Applies the grouping algorithm to an existing sequence and returns the modified sequence.
     @param midi_sequence to modify
     @returns The modified sequence
     */
    virtual MidiSequence getModifiedSequence(const MidiSequence& midi_sequence) const = 0;
    
    /**
     @brief Appiles the algorithm to a group of sequences, extracts the note groupings and
            populates a vector of strings which will contain the insert statements to place these groupings
            into the database.
     @param sequences to extract note groupings from
     @param sql_insert_statements to populate
     */
    virtual void getSQLInsertStatements(const std::vector<Sequence>& sequences, std::vector<std::string>& sql_insert_statements) const = 0; // Prepares the Sql to insert for all songs
    
    /**
     @brief Retrives the sql to create the database for this grouping.
     @returns The sql statement
     */
    virtual std::string getDatabaseCreationSQL() const = 0;
    
    /**
     @brief Gets the name of the database file.
     @returns The file name
     */
    virtual std::string getDatabaseName() const = 0;
    
    /**
     @brief Gets the name of the database directory.
     @returns The directory name
     */
    virtual std::string getDatabaseDirectory() const = 0;
    
    /**
     @brief Virtual destructor.
     */
    virtual ~NoteGrouping() {}
};

#endif /* NoteGrouping_hpp */
