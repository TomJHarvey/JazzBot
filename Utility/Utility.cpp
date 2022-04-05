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
        std::cout << "Could not open the file: " << file_name << std::endl;;
    }
    return content;
}

bool
Utility::validStofConversion(const std::string& value_to_convert,
                             float& converted_value)
{
    try{
        converted_value = stof(value_to_convert);
    }
    catch(const std::exception& e){
        std::cout << "Invalid input: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool
Utility::validStoiConversion(const std::string& value_to_convert, int& converted_value)
{
    try{
        converted_value = stoi(value_to_convert);
    }
    catch(const std::exception& e){
        std::cout << "Invalid input: " << e.what() << std::endl;
        return false;
    }
    return true;
}
