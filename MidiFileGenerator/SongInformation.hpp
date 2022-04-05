//
//  SongInformation.hpp
//  MIDI_FILE_GENERATOR
//
//  Created by Tom Harvey on 05/04/2022.
//

#ifndef SongInformation_hpp
#define SongInformation_hpp

#include <stdio.h>
#include <vector>
#include <string>

enum class TimeSignature
{
    four_four = 4,
    three_four = 3,
    invalid = -1
};

using SongInformationData = std::pair<std::string, TimeSignature>;

class SongInformation
{
public:
    SongInformation(const std::string& project_path);
    std::size_t numberOfElements() const;
    std::string getTitle(const std::size_t& index) const;
    TimeSignature getTimeSignature(const std::size_t& index) const;
    
private:
    void parseFiles();
    static std::string parseLine(std::string line);
    static TimeSignature convertStringToTimeSignature(std::string time_signature_string);
    
private:
    std::string m_project_path;
    std::vector<SongInformationData> m_data;
};


#endif /* SongInformation_hpp */
