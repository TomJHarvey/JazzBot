//
//  SequenceUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//

#include "SequenceUtility.hpp"
#include <algorithm>

//static const juce::File chord_changes_directory(CHORD_CHANGES_DIRECTORY);
//static const juce::File song_information_directory(SONG_INFORMATION_DIRECTORY);

// Todo: Figure out how to remove relative path from macro, why does it work with the file chooser but not here?
static const juce::String song_information_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/song_information";
static const juce::String chord_changes_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/chord_changes";
static const juce::String midi_files_directory = "/Users/tom-harvey/dev/JazzBot/midi_files";

static const std::string four_four_string = "4/4";
static const std::string three_four_string = "3/4";
static const std::string five_four_string = "5/4";
static const std::string six_four_string = "6/4";
static const std::string nine_four_string = "9/4";

static const std::size_t num_sequence_elements = 4;

std::vector<Sequence>
SequenceUtility::generateAllSequenceObjects()
{
    std::vector<Sequence> sequence;
    for (const auto& file : juce::File(midi_files_directory).findChildFiles (2, false))
    {
        //std::cout << file.getFileName() << std::endl;
        juce::File song_info_file(song_information_directory +
                                  "/" +
                                  file.getFileName().replace(".mid", "_song_information.txt"));
        
        if (song_info_file.exists())
        {
            juce::File chord_changes_file(chord_changes_directory +
                                          "/" +
                                          file.getFileName().replace(".mid", "_chord_changes.txt"));
            
            if (chord_changes_file.exists())
            {
                SongInformation song_information;
                ChordSequence chord_sequence;
                MidiSequence midi_sequence;
                if (parseSongInformation(song_info_file, song_information) &&
                    parseChordSequence(chord_changes_file, chord_sequence, song_information.m_time_signature) &&
                    MidiFileUtility::parseMidiFile(file, midi_sequence))
                {
                    sequence.push_back({song_information, chord_sequence, midi_sequence});
                }
            }
        }
    }
    return sequence;
}

bool
SequenceUtility::parseSongInformation(const juce::File& file, SongInformation& song_information)
{
    if (!file.existsAsFile())
    {
        return false;
    }
    
    juce::FileInputStream inputStream (file);
    if (! inputStream.openedOk())
    {
        return false;
    }
    
    std::vector<std::string> sequence_elements;
    while (!inputStream.isExhausted())
    {
        auto line = inputStream.readNextLine();
        sequence_elements.push_back(line.toStdString());
    }
    if (sequence_elements.size() != num_sequence_elements)
    {
        return false;
    }
    else
    {
        TimeSignature time_signature = convertStringToTimeSignature(sequence_elements[3]);
        if (time_signature != TimeSignature::not_set)
        {
            song_information = {sequence_elements[0],
                                sequence_elements[1],
                                sequence_elements[2],
                                time_signature};
            return true;
        }
        else
        {
            return false;
        }
    }
}

TimeSignature
SequenceUtility::convertStringToTimeSignature(const std::string& time_signature_string)
{
    TimeSignature time_signature = TimeSignature::not_set;
    if (time_signature_string == four_four_string)
    {
        time_signature = TimeSignature::four_four;
    }
    else if (time_signature_string == three_four_string)
    {
        time_signature = TimeSignature::three_four;
    }
    else if (time_signature_string == five_four_string)
    {
        time_signature = TimeSignature::five_four;
    }
    else if (time_signature_string == six_four_string)
    {
        time_signature = TimeSignature::six_four;
    }
    else if (time_signature_string == nine_four_string)
    {
        time_signature = TimeSignature::nine_four;
    }
    else
    {
        std::cout << "new time sig not handled " << std::endl;
    }
    return time_signature;
}

bool
SequenceUtility::parseChordSequence(const juce::File& file,
                                    ChordSequence& chord_sequence,
                                    const TimeSignature& time_signature)
{
    if (!file.existsAsFile())
    {
        return false;
    }
    
    std::string chord_sequence_str = file.loadFileAsString().toStdString();
    chord_sequence_str.erase(std::remove(chord_sequence_str.begin(), chord_sequence_str.end(), '\n'), chord_sequence_str.end());
    std::cout << file.getFileName() << std::endl;
    
    // Get characters between the pipes , split the string between each pipe
    std::string delimiter = "|";
    size_t pos = 0;
    std::string current_bar;
    std::size_t bar_number = 0;
    while ((pos = chord_sequence_str.find(delimiter)) != std::string::npos)
    {
        current_bar = chord_sequence_str.substr(0, pos);
        //std::cout << current_bar << std::endl;
        
        if (!current_bar.empty() &&
            std::find(current_bar.begin(), current_bar.end(), ':') == current_bar.end())
        {
            std::size_t current_chord_position = 0; // maybe not 0.
            bool found_chord = false;
            if (isupper(current_bar[0]))
            {
                found_chord = true;
            }
            else
            {
                std::cout << "Rest before chord" << std::endl;
            }
            
            // this for loop can also go in a functino
            for (std::size_t i = 0; i < current_bar.length(); i++)
            {
                if (isupper(current_bar[i]))
                {
                    if (found_chord)
                    {
                        std::size_t bar_position = i+1;
                        std::size_t number_of_beats = findChord(current_bar, bar_position);
                        std::string chord;
                        if (number_of_beats != 0)
                        {
                            chord = current_bar.substr(current_chord_position, bar_position - current_chord_position - 1);
                            chord.erase(remove(chord.begin(), chord.end(), ' '), chord.end());
                        }
                        else
                        {
                            chord = current_bar.substr(current_chord_position, bar_position - current_chord_position);
                        }

                        number_of_beats += 1;
                        chord_sequence.push_back({chord, number_of_beats, bar_number});
                        current_chord_position = bar_position;
                        i = bar_position - 1;
                    }
                    else
                    {
                        current_chord_position = i;
                        // test this separately, make sure it actually happens.
                        // count until end of chord and set, get spaces after
                    }
                }
            }
            // this can go in a function
            std::size_t total_beats = 0; // maybe this should just be an int. And just static cast for comparrisons with size
            for (int index = static_cast<int>(chord_sequence.size()) -1; index >=0; index--) // this casting is a mess, is it a problem if it reaches below 0?
            {
                if (chord_sequence[static_cast<std::size_t>(index)].m_bar_number == bar_number)
                {
                    total_beats += chord_sequence[static_cast<std::size_t>(index)].m_number_of_beats;
                }
            }
            if (total_beats != static_cast<std::size_t>(time_signature))
            {
                std::cout << "Incorrect number of beats in bar " << std::endl;
                chord_sequence.clear();
                return false;
            }
            bar_number ++;
        }
        chord_sequence_str.erase(0, pos + delimiter.length());
    }
    return true;
}

std::size_t
SequenceUtility::findChord(const std::string& current_bar, std::size_t& bar_position)
{
    std::size_t number_of_beats = 0;
    while (bar_position < current_bar.length())
    {
        if (current_bar[bar_position] == ' ')
        {
            // count the number of beats
            number_of_beats ++;
        }
        else if (isupper(current_bar[bar_position]))
        {
            return number_of_beats;
        }
        else if(current_bar[bar_position] == '/')
        {
            if (isupper(current_bar[bar_position+1]))
            {
                bar_position +=2;
                return findChord(current_bar, bar_position);
            }
            else
            {
                std::cout << "whats going on here" << std::endl;
            }
        }
        bar_position ++;
    }
    return number_of_beats;
}
