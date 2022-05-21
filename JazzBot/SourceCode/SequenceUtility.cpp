//
//  SequenceUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//

#include "SequenceUtility.hpp"
#include "ChordUtility.hpp"
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

//static const int beat_length = 480; // repeated definition

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
                        std::string chord_sequence_string = getChordSequenceAsString(chord_changes_file);
                        if (ChordUtility::parseChordSequence(chord_sequence_string,
                                               chord_sequence,
                                               song_information.m_time_signature,
                                               key,
                                               chord_changes_file.getFileName()) &&
                            MidiSequenceUtility::parseMidiFile(file, midi_sequence))
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

std::string
SequenceUtility::getChordSequenceAsString(const juce::File& file)
{
    //std::cout << file.getFileName() << std::endl;
    if (!file.existsAsFile())
    {
        return "";
    }
    std::string chord_sequence_str = file.loadFileAsString().toStdString();
    chord_sequence_str.erase(std::remove(chord_sequence_str.begin(),
                                         chord_sequence_str.end(),
                                         '\n'),
                            chord_sequence_str.end());
    return chord_sequence_str;
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
