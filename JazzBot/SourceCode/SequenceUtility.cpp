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

static const int beat_length = 480; // repeated definition

std::vector<Sequence>
SequenceUtility::generateAllSequenceObjects()
{
    std::vector<Sequence> sequence;
    for (const auto& file : juce::File(midi_files_directory).findChildFiles (2, false))
    {
        std::cout << file.getFileName() << std::endl;
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

                if (parseSongInformation(song_info_file, song_information))
                {
                    ChordRoot key = ChordUtility::getKey(song_information.m_key);
                    if (key != ChordRoot::Invalid)
                    {
                        if (parseChordSequence(chord_changes_file, chord_sequence, song_information.m_time_signature, key) &&
                            MidiFileUtility::parseMidiFile(file, midi_sequence))
                        {
                            sequence.push_back({song_information, chord_sequence, midi_sequence});
                        }
                    }
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
        if (time_signature != TimeSignature::not_set &&
            sequence_elements[2] != "no_key" &&
            sequence_elements[2].find("dor") == std::string::npos &&
            sequence_elements[2].find("mix") == std::string::npos &&
            sequence_elements[2].find("-chrom") == std::string::npos &&
            sequence_elements[2].find("-blues") == std::string::npos)
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
        std::cout << "Time signature " << time_signature_string <<  " not handled " << std::endl;
    }
    return time_signature;
}

bool
SequenceUtility::parseChordSequence(const juce::File& file,
                                    ChordSequence& chord_sequence,
                                    const TimeSignature& time_signature,
                                    const ChordRoot& key) // Refactor this bad boy, its way too big.
{
    if (!file.existsAsFile())
    {
        return false;
    }
    
    
    ChordsInKey chords_in_key = ChordUtility::getChordsInKey(key); // pass this into parsechordseuqnece.
    
    std::string chord_sequence_str = file.loadFileAsString().toStdString();
    
    // the above bit should just be the parsing, and just get the string.
    // then the next bit should be split into functions for the generation generateChordSequence
    
    chord_sequence_str.erase(std::remove(chord_sequence_str.begin(), chord_sequence_str.end(), '\n'), chord_sequence_str.end());
    //std::cout << file.getFileName() << std::endl;
    
    // Get characters between the pipes , split the string between each pipe
    std::string delimiter = "|";
    size_t pos = 0;
    std::string current_bar;
    std::size_t bar_number = 0;
    const int bar_length = static_cast<int>(time_signature) * beat_length;
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
            else if (current_bar[0] == 'N')
            {
                //std::cout << file.getFileName() << ": Does not handle N.C chord at bar number " << bar_number << std::endl;
                return false;
            }
            else
            {
                std::cout << "Rest before chord" << std::endl;
            }
            
            int previous_number_of_beats = 0;
            int total_beats = 0;
            // this for loop can also go in a functino
            for (std::size_t i = 0; i < current_bar.length(); i++)
            {
                if (current_bar[i] == 'N')
                {
                    //std::cout << file.getFileName() << ": Does not handle N.C chord at bar number " << bar_number << std::endl;
                    chord_sequence.clear();
                    return false;
                }
                else if (isupper(current_bar[i]))
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
                        
                        // Chords in key will be passed into this function
                        // this will be a new struct member called m_chord_degree
                        std::string chord_degree = ChordUtility::convertChordNameToDegree(chords_in_key, chord);
                        
                        int chord_position = (bar_number * bar_length) + (previous_number_of_beats * beat_length);
                        std::cout << "Bar number = " << bar_number << " - Position = " << chord_position << " chord = " << chord << " degree = " << chord_degree << std::endl;

                        // chord position is what i should push back
                        number_of_beats += 1;
                        previous_number_of_beats += number_of_beats;
                        total_beats += number_of_beats;
                        chord_sequence.push_back({chord, chord_position, bar_number, chord_degree});
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
            if (total_beats != static_cast<int>(time_signature))
            {
                std::cout << file.getFileName() << ": Has incorrect number of beats at bar number " << bar_number << std::endl;
                chord_sequence.clear();
                return false;
            }
            bar_number ++;
        }
        chord_sequence_str.erase(0, pos + delimiter.length());
    }
    
    std::size_t last_bar_position = 0;
    if (chord_sequence.size() > 1 &&
        chord_sequence[0].m_bar_number != chord_sequence[chord_sequence.size()-1].m_bar_number)
    {
        for (std::size_t index = chord_sequence.size() -1; index >= 0; index --)
        {
            if (chord_sequence[index].m_bar_number != (bar_number - 1))
            {
                last_bar_position = index +1;
                break;
            }
        }
        
        // Checks to see if the midi tick values line up with the amount of bars.
        // There is still a chance that they're misaallihned and realligned in the middle,
        // however that is a very small chance, so this test will suffice.
        if ((chord_sequence[last_bar_position].m_chord_position != 0) &&
            (chord_sequence[last_bar_position].m_chord_position / chord_sequence[last_bar_position].m_bar_number) != bar_length) // don't use this variable.
        {
            chord_sequence.clear();
            std::cout << "Midi tick values for chords mis alligned" << std::endl;
            return false;
        }
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
                std::cout << "Invalid Slash chord format " << std::endl;
            }
        }
        bar_position ++;
    }
    return number_of_beats;
}
