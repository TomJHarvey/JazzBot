//
//  DatabaseUtility.cpp
//  JazzBotCommandLine
//
//  Created by Tom Harvey on 22/07/2022.
//

#include "DatabaseUtility.hpp"

#include <iostream>

sqlite3*
DatabaseUtility::getDatabase(const std::string& database_name, const std::string& database_directory)
{
    sqlite3* sql_database = nullptr;
    std::string database_location;
    // checks if it exists and also returns the location of the database as a string
    if (databaseExists(database_name, database_directory, database_location))
    {
        int exit = 0;
        exit = sqlite3_open(database_location.c_str(), &sql_database);
        if (exit)
        {
            std::cerr << "Error open DB " << sqlite3_errmsg(sql_database) << std::endl; // do we want it to be an error?
        }
    }
    return sql_database;
}

bool
DatabaseUtility::selectFromDatabase(const std::string& sql, sqlite3* sql_database, SQLColumns& sql_columns) // pass in an sql database pointer
{
    bool success = false;
    if (sql_database != nullptr)
    {
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(sql_database,
                                    sql.c_str(),
                                    -1,
                                    &stmt,
                                    nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "SELECT failed: " << sqlite3_errmsg(sql_database) << std::endl;
            //return ...; // or throw
            success = false;
        }
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            success = true;
            for (auto& sql_column : sql_columns)
            {
                sql_column.m_data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, sql_column.m_index)); // need to loop through the passed in array
            }
        }
        if (rc != SQLITE_DONE)
        {
            std::cerr << "SELECT failed: " << sqlite3_errmsg(sql_database) << std::endl;
            // if you return/throw here, don't forget the finalize
            success = false;
        }
        sqlite3_finalize(stmt);
    }
    return success;
}

bool
DatabaseUtility::createDatabase(const std::string& database_name, const std::string& database_directory, const std::string& sql)
{
    // check if directory exists, if not create it
    juce::File output_directory = getOutputDirectory(database_directory); // this should be passed in.
    if (!output_directory.isDirectory())
    {
        std::cout << "creating directory " << output_directory.getFileName() << std::endl;
        output_directory.createDirectory();
    }
    
    // if the database already exists then return
    std::string database_string;
    if (databaseExists(database_name, database_directory, database_string))
    {
        std::cout << "Database file " << database_string << " already exists" << std::endl;
        return true;
    }
    
    // open the database and insert the sql to cerate the tables
    sqlite3* sql_database;
    int exit = 0;
    exit = sqlite3_open(database_string.c_str(), &sql_database);
    char* messagge_error;
    exit = sqlite3_exec(sql_database, sql.c_str(), nullptr, nullptr, &messagge_error);
    if (exit != SQLITE_OK) {
        std::cout << "Error Create Table" << std::endl;
        sqlite3_free(messagge_error);
        return false;
    }
    else
    {
        std::cout << "Table created Successfully" << std::endl;
    }
    sqlite3_close(sql_database);
    return true;
}

bool
DatabaseUtility::populateDatabase(const std::string& database_name,
                                    const std::string& database_directory,
                                    const std::vector<std::string>& sql_insert_statements)
{
    sqlite3* sql_database;
    char* messagge_error;
    juce::File output_directory = getOutputDirectory(database_directory);
    std::string database_string = output_directory.getFullPathName().toStdString() + "/" + database_name; // not portable
    juce::File output_directory_file(database_string);
    if (!output_directory_file.exists())
    {
        std::cout << "Database file " << database_string << " doesn't exist" << std::endl;
        return false;
    }
    
    // check if file exists.
    int exit = sqlite3_open(database_string.c_str(), &sql_database);
    
    for (std::size_t index = 0; index < sql_insert_statements.size(); index++) // loop round amount of sql insert queries
    {
        exit = sqlite3_exec(sql_database, sql_insert_statements[index].c_str(), nullptr, nullptr, &messagge_error); // index each sql query
        if (exit != SQLITE_OK)
        {
            std::cout << "Error Insert" << std::endl;
            sqlite3_free(messagge_error);
            return false;
        }
        else
        {
            std::cout << "Records created Successfully!" << std::endl;
        }
    }
    sqlite3_close(sql_database);
    return true;
}

juce::File
DatabaseUtility::getOutputDirectory(const std::string& output_folder_name)
{
    // if directory doesn't exist, create directory
    juce::String current_working_directory =
        juce::File::getCurrentWorkingDirectory().getFullPathName();
    
    juce::String project_path =
        current_working_directory.upToLastOccurrenceOf("JazzBot/", true, false);
    
    juce::String output_directory_path = project_path + output_folder_name;
    juce::File output_directory = output_directory_path;
    return output_directory;
}

bool
DatabaseUtility::databaseExists(const std::string& database_name, const std::string& database_directory, std::string& database_location)
{
    juce::File output_directory = getOutputDirectory(database_directory);
    database_location = output_directory.getFullPathName().toStdString() + "/" + database_name; // not portable
    juce::File output_directory_file(database_location);
    return output_directory_file.exists();
}
