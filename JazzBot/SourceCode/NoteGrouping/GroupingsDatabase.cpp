//
//  GroupingsDatabase.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 21/06/2022.
//

#include "GroupingsDatabase.hpp"
#include <sqlite3.h>
#include <iostream>
#include <regex>

static std::string database_directory = "NoteGroupings/";
char apostrophe = '\'';

bool
GroupingsDatabase::createDatabase(const std::string& database_name)
{
    juce::File output_directory = getOutputDirectory(database_directory);
    if (!output_directory.isDirectory())
    {
        std::cout << "creating directory " << output_directory.getFileName() << std::endl;
        output_directory.createDirectory();
    }
    std::string database_string = output_directory.getFullPathName().toStdString() + "/" + database_name; // not portable
    juce::File output_directory_file(database_string);
    if (output_directory_file.exists())
    {
        std::cout << "Database file " << database_string << " already exists" << std::endl;
        return true;
    }
    
    sqlite3* sql_database;
    std::string sql = "CREATE TABLE NOTEGROUPING("
                          "ID INT PRIMARY KEY     NOT NULL, "
                          "FILENAME         TEXT    NOT NULL, "
                          "CHORD            TEXT    NOT NULL, "
                          "STARTINGNOTE     TEXT    NOT NULL, "
                          "GROUPSIZE        TEXT    NOT NULL, "
                          "DIRECTION        TEXT    NOT NULL, "
                          "NEXTCHORD        TEXT    NOT NULL, "
                          "BEAT             TEXT    NOT NULL, "
                          "LOCATION         TEXT    NOT NULL, "
                          "GROUPINGNUM      TEXT    NOT NULL, "
                          "NOTES            TEXT    NOT NULL);";
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
GroupingsDatabase::populateDatabase(const std::string& database_name,
                                    const NoteGroupingData& note_grouping_data)
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
    
    for (std::size_t index = 0; index < note_grouping_data.size(); index++)
    {
        std::string file_name = note_grouping_data[index].first.m_file_name;
        if (std::count(file_name.begin(), file_name.end(), apostrophe) != 0)
        {
            file_name = std::regex_replace(file_name, std::regex("'"), "''");
        }
        
        std::string notes;
        for (auto& note : note_grouping_data[index].second)
        {
            notes += note + "&";
        }
        
        notes.pop_back();
        
        std::string sql = "INSERT INTO NOTEGROUPING VALUES(" + std::to_string(index + 1) +
                            ",'" + file_name + "'," +
                            "'" + note_grouping_data[index].first.m_chord + "'," +
                            "'" + note_grouping_data[index].first.m_starting_note + "'," +
                            "'" + note_grouping_data[index].first.m_group_size + "'," +
                            "'" + note_grouping_data[index].first.m_direction + "'," +
                            "'" + note_grouping_data[index].first.m_next_chord + "'," +
                            "'" + note_grouping_data[index].first.m_beat + "'," +
                            "'" + note_grouping_data[index].first.m_location + "'," +
                            "'" + note_grouping_data[index].first.m_grouping_number + "'," +
                            "'" + notes + "');";
        

        exit = sqlite3_exec(sql_database, sql.c_str(), nullptr, nullptr, &messagge_error);
        if (exit != SQLITE_OK) {
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
GroupingsDatabase::getOutputDirectory(const std::string& output_folder_name)
{
    // if directory doesn't exist, create directory
    // this bit of code exists in midifilegenerator, i could use the project utility class for this
    juce::String current_working_directory =
        juce::File::getCurrentWorkingDirectory().getFullPathName();
    
    juce::String project_path =
        current_working_directory.upToLastOccurrenceOf("JazzBot/", true, false);
    
    juce::String output_directory_path = project_path + output_folder_name;
    juce::File output_directory = output_directory_path;
    
    return output_directory;
}

bool
GroupingsDatabase::databaseExists(const std::string& database_name)
{
    juce::File output_directory = getOutputDirectory(database_directory);
    std::string database_string = output_directory.getFullPathName().toStdString() + "/" + database_name; // not portable
    juce::File output_directory_file(database_string);
    return output_directory_file.exists();
}
