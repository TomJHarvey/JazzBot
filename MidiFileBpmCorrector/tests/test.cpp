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
        SECTION ("Valid bar onsets")
        {
            // test a bar number smaller than the previous bar is invalid
            std::string bar_onset_file_name = project_path.toStdString() + "valid_bar_onsets.csv";
            
            SECTION ("Valid bar onset")
            {
                MidiFileGenerator midi_file_generator("",bar_onset_file_name, "");
                REQUIRE(midi_file_generator.setBeatInformation());
            }
//            
            // valid bar onsets are needed to test this functionality
            SECTION ("Invalid note information")
            {
                SECTION ("Invalid note onset")
                {
                    std::string note_onset_file_name = project_path.toStdString() + "invalid_note_onset.csv";
                    MidiFileGenerator midi_file_generator("",bar_onset_file_name, note_onset_file_name);
                    REQUIRE(midi_file_generator.setBeatInformation());
                    REQUIRE(!midi_file_generator.writeSequence());
                }
                SECTION ("Invalid note duration")
                {
                    std::string note_onset_file_name = project_path.toStdString() + "invalid_note_duration.csv";
                    MidiFileGenerator midi_file_generator("",bar_onset_file_name, note_onset_file_name);
                    REQUIRE(midi_file_generator.setBeatInformation());
                    REQUIRE(!midi_file_generator.writeSequence());
                }
                SECTION ("Invalid note value")
                {
                    std::string note_onset_file_name = project_path.toStdString() + "invalid_note_value.csv";
                    MidiFileGenerator midi_file_generator("",bar_onset_file_name, note_onset_file_name);
                    REQUIRE(midi_file_generator.setBeatInformation());
                    REQUIRE(!midi_file_generator.writeSequence());
                }
                SECTION ("Non sequential note onset")
                {
                    std::string note_onset_file_name = project_path.toStdString() + "non_sequential_note_onset.csv";
                    MidiFileGenerator midi_file_generator("",bar_onset_file_name, note_onset_file_name);
                    REQUIRE(midi_file_generator.setBeatInformation());
                    REQUIRE(!midi_file_generator.writeSequence());
                }
            }
        }

        // make csv file with differing gaps between notes then look at the beat lengths.
    }
}


