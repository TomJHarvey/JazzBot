//
//  SongInformationUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/07/2022.
//

#ifndef SongInformationUtility_hpp
#define SongInformationUtility_hpp

#include <stdio.h>
#include <juce_core/juce_core.h>
#include "../SequenceTypes.h"

/**
    @brief Utility class used to parse song information files
 */
class SongInformationUtility
{
public:
    
    /**
     @brief Parses a song information file into a SongInformation object
     @param file to read
     @param song_information to populate
     @returns If successfully populated song information object
     */
    static bool parseSongInformation(const juce::File& file, SongInformation& song_information);

private:
    
    /**
     @brief Converts a string to a TimeSignature enum
     @param time_signature_string to convert
     @returns Converted time signature
     */
    static TimeSignature convertStringToTimeSignature(const std::string& time_signature_string);
};

#endif /* SongInformationUtility_hpp */
