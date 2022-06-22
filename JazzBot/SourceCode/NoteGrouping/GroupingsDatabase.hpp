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
#include <stdio.h>
#include <string>

class GroupingsDatabase
{
public:
    static bool createDatabase(const std::string& database_name);
    static bool populateDatabase(const std::string& database_name, const NoteGroupingData& note_grouping_data);
    static juce::File getOutputDirectory(const std::string& output_folder_name);
    static bool databaseExists(const std::string& database_name);
};

#endif /* GroupingsDatabase_hpp */
