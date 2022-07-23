//
//  DatabaseUtility.hpp
//  JazzBotCommandLine
//
//  Created by Tom Harvey on 22/07/2022.
//

#ifndef DatabaseUtility_hpp
#define DatabaseUtility_hpp

#include <juce_core/juce_core.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>

struct SQLColumn
{
    std::string m_data;
    int m_index;
};

using SQLColumns = std::vector<SQLColumn>;


class DatabaseUtility
{
public:
    static bool selectFromDatabase(const std::string& rows, sqlite3* m_sql_database, SQLColumns& sql_columns);
    static sqlite3* getDatabase(const std::string& database_name, const std::string& database_directory);
    static bool createDatabase(const std::string& database_name, const std::string& database_directory, const std::string& sql);
    static bool populateDatabase(const std::string& database_name, const std::string& database_directory, const std::vector<std::string>& sql_insert_statements);
    static juce::File getOutputDirectory(const std::string& output_folder_name);
    static bool databaseExists(const std::string& database_name, const std::string& database_directory, std::string& database_location);
};

#endif /* DatabaseUtility_hpp */
