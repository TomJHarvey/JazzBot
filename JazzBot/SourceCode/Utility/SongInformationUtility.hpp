//
//  SongInformationUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/07/2022.
//

#ifndef SongInformationUtility_hpp
#define SongInformationUtility_hpp

#include <stdio.h>

#include <stdio.h>
#include <juce_core/juce_core.h>
#include "../SequenceTypes.h"

class SongInformationUtility
{
public:
    static bool parseSongInformation(const juce::File& file, SongInformation& song_information);
private:
    static TimeSignature convertStringToTimeSignature(const std::string& time_signature_string);
};

#endif /* SongInformationUtility_hpp */
