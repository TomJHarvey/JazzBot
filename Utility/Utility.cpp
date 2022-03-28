#include <iostream>
#include <fstream>
#include <sstream>

#include "Utility.hpp"
 
std::vector<std::vector<std::string>>
Utility::parseCsvFile(std::string file_name)
{
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> row;
    std::string line, word;
    std::fstream file (file_name, std::ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();
            std::stringstream str(line);
            while(getline(str, word, ','))
            {
                row.push_back(word);
            }
            content.push_back(row);
        }
    }
    else
    {
        std::cout << "Could not open the file\n" << std::endl;;
    }
    return content;
}
