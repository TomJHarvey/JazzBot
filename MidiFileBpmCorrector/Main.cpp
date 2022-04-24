#include "MidiFileGenerator.hpp"
#include <string>


#include "SongInformation.hpp"
#include "../Utility/Utility.hpp"

static const char* beats_directory = "DatabaseParser/beats";
static const char* notes_directory = "DatabaseParser/notes";
static const char* file_name_suffix = "_Solo.csv";

void
generateMidiFiles(const juce::String& project_path,
                  const juce::String& output_directory_path) // TODO: maybe need to find a home for this function
{
    SongInformation song_information(project_path.toStdString());
    std::cout << song_information.numberOfElements() << std::endl;
    for (std::size_t song_index = 0; song_index < song_information.numberOfElements(); song_index ++)
    {
        // slash is not platform specific, need equivalent of os.path.join in python
        std::string bar_onset_file_name = project_path.toStdString() + beats_directory + "/" + song_information.getTitle(song_index) + file_name_suffix;
        std::string note_onset_file_name = project_path.toStdString() + notes_directory + "/" + song_information.getTitle(song_index) + file_name_suffix;
        juce::String output_file_path = "/" + song_information.getTitle(song_index);
        
        MidiFileGenerator midi_file_generator(output_directory_path,
                                              output_file_path,
                                              song_information.getTimeSignature(song_index));
        
        if (midi_file_generator.setBeatInformation(Utility::parseCsvFile(bar_onset_file_name)))
        {
            midi_file_generator.initMidiFile();
            if (midi_file_generator.writeSequence(Utility::parseCsvFile(note_onset_file_name)))
            {
                midi_file_generator.writeMidiFile();
            }
            else
            {
                std::cout << "Failed to write sequence for file: " << output_file_path << std::endl;
            }
        }
        else
        {
            std::cout << "Failed to set beat information for file: " << output_file_path << std::endl;
        }
    }
}

int main()
{
    juce::String current_working_directory =
        juce::File::getCurrentWorkingDirectory().getFullPathName();
    
    juce::String project_path =
        current_working_directory.upToFirstOccurrenceOf("JazzBot/", true, false);
    
    juce::String output_directory_path = project_path + "midi_files";
    generateMidiFiles(project_path, output_directory_path);
    return 0;
}
