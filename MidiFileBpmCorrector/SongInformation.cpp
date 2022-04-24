//
//  SongInformation.cpp
//  MIDI_FILE_GENERATOR
//
//  Created by Tom Harvey on 05/04/2022.
//

#include "SongInformation.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

static const char* song_information_directory = "DatabaseParser/song_information";
static const unsigned short song_artist_and_title = 0;
static const unsigned short song_time_signature = 3;

static const char* four_four_string = "4/4";
static const char* three_four_string = "3/4";

SongInformation::SongInformation(const std::string& project_path)
    : m_project_path(project_path)
{
    parseFiles();
}

std::size_t
SongInformation::numberOfElements() const
{
    return m_data.size();
}

std::string
SongInformation::getTitle(const std::size_t& index) const
{
    return m_data[index].first;
}

TimeSignature
SongInformation::getTimeSignature(const std::size_t& index) const
{
    return m_data[index].second;
}

void
SongInformation::parseFiles()
{
    std::string song_information_path = m_project_path + song_information_directory;
    for (const auto& directory_entry :
         std::__fs::filesystem::recursive_directory_iterator(song_information_path))
    {
        std::fstream file (directory_entry.path(), std::ios::in);
        std::string line, title, time_signature_str;
        if(file.is_open())
        {
            unsigned short line_counter = 0;
            while(getline(file, line))
            {
                if (line_counter == song_artist_and_title)
                {
                    title = parseLine(line);
                }
                else if (line_counter == song_time_signature)
                {
                    time_signature_str = parseLine(line);;
                }
                line_counter++;
            }
            TimeSignature time_signature = convertStringToTimeSignature(time_signature_str);
            if (time_signature != TimeSignature::invalid)
            {
                m_data.push_back(std::make_pair(title, time_signature));
            }
        }
    }
}

TimeSignature
SongInformation::convertStringToTimeSignature(std::string time_signature_string)
{
    TimeSignature time_signature = TimeSignature::invalid;
    if (time_signature_string == four_four_string)
    {
        time_signature = TimeSignature::four_four;
    }
    else if (time_signature_string == three_four_string)
    {
        time_signature = TimeSignature::three_four;
    }
    return time_signature;
}

std::string
SongInformation::parseLine(std::string line)
{
    std::string word;
    std::stringstream str(line);
    getline(str, word);
    return word;
}
