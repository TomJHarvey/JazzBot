//
//  SongInformationUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//

#ifndef SongInformationUtility_hpp
#define SongInformationUtility_hpp

#include <stdio.h>
#include <juce_core/juce_core.h>
#include "MidiSequenceUtility.hpp"
#include "SequenceTypes.h"

class SequenceUtility
{
public:
    static std::vector<Sequence> generateAllSequenceObjects();
    static bool parseSongInformation(const juce::File& file, SongInformation& song_information);
    static std::string getChordSequenceAsString(const juce::File& file);
    static bool generateSequenceObject(const juce::File& song_info_file,
                                       const juce::File& chord_changes_file,
                                       const juce::File& midi_file,
                                       Sequence& sequence);
private:
    static TimeSignature convertStringToTimeSignature(const std::string& time_signature_string);

};

#endif /* SongInformationUtility_hpp */
