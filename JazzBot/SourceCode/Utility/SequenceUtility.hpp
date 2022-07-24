//
//  SongInformationUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//

#ifndef SequenceUtility_hpp
#define SequenceUtility_hpp

#include <stdio.h>
#include <juce_core/juce_core.h>
#include "../SequenceTypes.h"

class SequenceUtility
{
public:
    static std::vector<Sequence> generateAllSequenceObjects();
    static std::size_t findLastBar(const ChordSequence& chord_sequence, const std::size_t& bar_number);
    static RootNote convertStringToRootNote(const std::string& key, const bool& is_minor_key);
    static RootNote convertNoteValueToRootNote(const int& note_value);
    static int calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2);
    static RootNote getChordLetter(const std::string& chord_str);
    
    static std::string findChordForNote(const double& note_on,
                                 const ChordSequence& chord_sequence,
                                 const bool& next_chord,
                                 const TimeSignature& time_signature);
    
    static RootNote findRootNoteForChord(const double& note_on,
                                  const ChordSequence& chord_sequence,
                                  const TimeSignature& time_signature);
    
    static std::string getLocation(const double& note_on,
                            const ChordSequence& chord_sequence,
                            const TimeSignature& time_signature);

    static bool generateSequenceObject(const juce::File& song_info_file,
                                       const juce::File& chord_changes_file,
                                       const juce::File& midi_file,
                                       Sequence& sequence,
                                       const bool& ppq_is_480);
};

#endif /* SequenceUtility_hpp */
