#include "MidiFileGenerator.hpp"
#include <string>


int main()
{
    juce::String current_working_directory =
        juce::File::getCurrentWorkingDirectory().getFullPathName();
    
    juce::String project_path =
        current_working_directory.upToFirstOccurrenceOf("JazzBot/MidiFileGenerator/", true, false);
    
    std::string bar_onset_file_name = project_path.toStdString() + "ChetBaker_JustFriends_Solo.csv";
    std::string note_onset_file_name =
        project_path.toStdString() + "ChetBaker_JustFriends_Solo_notes.csv"; // this wont have the notes suffix, it'll just be in notes folder
        
    MidiFileGenerator midi_file_generator(project_path, bar_onset_file_name, note_onset_file_name);
    
    if (midi_file_generator.setBeatInformation())
    {
        midi_file_generator.initMidiFile();
        if (midi_file_generator.writeSequence())
        {
            midi_file_generator.writeMidiFile();
        }
    }
    return 0;
}
