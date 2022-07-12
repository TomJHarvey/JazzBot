//
//  GroupingsDatabase.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 21/06/2022.
//

#ifndef GroupingsDatabase_hpp
#define GroupingsDatabase_hpp

#include "NoteGrouping.hpp"

#include <juce_core/juce_core.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>


struct EighthNoteGroupingRows
{
    std::string m_starting_note;
    std::string m_beat;
    std::string m_chords;
    std::string m_group_size;
    std::string m_next_chord;
    std::string m_direction;
};

class GroupingsDatabase
{
public:
    
    // the non static methods are for selecting from the database
    GroupingsDatabase(const std::string& database_name); // init the db.
    ~GroupingsDatabase();
    std::string selectEighthNoteGroupings(const EighthNoteGroupingRows& rows);
    
    // These are for creating and inserting the database
    static bool createDatabase(const std::string& database_name);
    static bool populateDatabase(const std::string& database_name, const NoteGroupingData& note_grouping_data);
    static juce::File getOutputDirectory(const std::string& output_folder_name);
    static bool databaseExists(const std::string& database_name, std::string& database_location);
private:
    bool m_database_exists;
    sqlite3* m_sql_database;
    
};

#endif /* GroupingsDatabase_hpp */
