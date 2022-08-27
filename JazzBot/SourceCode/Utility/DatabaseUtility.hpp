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

/**
    @brief Used to store accessed data from the data base. The index member is the row number and the data member is the returned
           data from the selected row.
 */
struct SQLColumn
{
    std::string m_data;
    int m_index;
};

/**
    @brief A vector of columns for multiple items returned from the select statement.
 */
using SQLColumns = std::vector<SQLColumn>;

/**
    @brief Utility class used to access a database
 */
class DatabaseUtility
{
public:
    
    /**
     @brief Parses a song information file into a SongInformation object
     @param sql select statement
     @param sql_database to select from
     @param sql_columns to populate with retrieved data
     @returns If successfully retrieved data
     */
    static bool selectFromDatabase(const std::string& sql,
                                   sqlite3* sql_database,
                                   SQLColumns& sql_columns);
    
    /**
     @brief Gets a pointer to the database
     @param database_name  as a string
     @param database_directory as a string
     @returns The pointer to access the database
     */
    static sqlite3* getDatabase(const std::string& database_name,
                                const std::string& database_directory);
    
    /**
     @brief Creates a database
     @param database_name  as a string
     @param database_directory as a string
     @param sql to create the database
     @returns If it successfully created the database
     */
    static bool createDatabase(const std::string& database_name,
                               const std::string& database_directory,
                               const std::string& sql);
    
    /**
     @brief Populates the database
     @param database_name  as a string
     @param database_directory as a string
     @param sql_insert_statements containing each statement to insert data into the database
     @returns If it successfully inserted rows
     */
    static bool populateDatabase(const std::string& database_name,
                                 const std::string& database_directory,
                                 const std::vector<std::string>& sql_insert_statements);
    
    /**
     @brief Get the output directory
     @param output_folder_name  as a string
     @returns The output directory
     */
    static juce::File getOutputDirectory(const std::string& output_folder_name);
    
    /**
     @brief Check if database exists
     @param database_name  as a string
     @param database_directory as a string
     @param database_location returned as string
     @returns If the database exists
     */
    static bool databaseExists(const std::string& database_name,
                               const std::string& database_directory,
                               std::string& database_location);
};

#endif /* DatabaseUtility_hpp */
