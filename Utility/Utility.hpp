#include <string>
#include <vector>

class Utility
{
public:
    static std::vector<std::vector<std::string>> parseCsvFile(std::string file_name);
    static bool validStofConversion(const std::string& value_to_convert, float& converted_value);
    static bool validStoiConversion(const std::string& value_to_convert, int& converted_value);
};

