//
//  SongInformationUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/07/2022.
//

#include "SongInformationUtility.hpp"
#include "ChordParsingUtility.hpp"

static const std::string four_four_string = "4/4";
static const std::string three_four_string = "3/4";
static const std::string five_four_string = "5/4";
static const std::string six_four_string = "6/4";
static const std::string nine_four_string = "9/4";
static const std::size_t num_sequence_elements = 4;

bool
SongInformationUtility::parseSongInformation(const juce::File& file, SongInformation& song_information)
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
        RootNote key = ChordParsingUtility::getKey(sequence_elements[2]);
        if (time_signature != TimeSignature::not_set &&
            key != RootNote::Invalid)
        {
            song_information = {sequence_elements[0],
                                sequence_elements[1],
                                key,
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
SongInformationUtility::convertStringToTimeSignature(const std::string& time_signature_string)
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
