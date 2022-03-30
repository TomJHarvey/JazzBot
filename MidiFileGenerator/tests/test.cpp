#include <catch2/catch_test_macros.hpp>
#include "../MidiFileGenerator.hpp"

TEST_CASE("MidiFileGeneratorTests", "[midi_file_generator_tests]")
{
    // get file locationn
    juce::String current_working_directory =
        juce::File::getCurrentWorkingDirectory().getFullPathName();

    juce::String project_path =
        current_working_directory.upToFirstOccurrenceOf("JazzBot/MidiFileGenerator/", true, false);

    project_path += "tests/test_files/";
    
    SECTION("Test setBeatInformation")
    {
        SECTION("Empty beat info file")
        {
            // test empty file
            MidiFileGenerator midi_file_generator("","", "");
            REQUIRE(!midi_file_generator.setBeatInformation());
        }
        
        SECTION ("Invalid bar onset")
        {
            // test invalid string character for stof conversion of row
            std::string bar_onset_file_name = project_path.toStdString() + "invalid_bar_onset.csv";
            MidiFileGenerator midi_file_generator("",bar_onset_file_name, "");
            REQUIRE(!midi_file_generator.setBeatInformation());
        }
        
        SECTION ("Non sequential bar onset")
        {
            // test a bar number smaller than the previous bar is invalid
            std::string bar_onset_file_name = project_path.toStdString() + "non_sequential_bar_onset.csv";
            MidiFileGenerator midi_file_generator("",bar_onset_file_name, "");
            REQUIRE(!midi_file_generator.setBeatInformation());
        }
        
        // make csv file with differing gaps between notes then look at the beat lengths.
        
    }
    
    
    
    
   
}


