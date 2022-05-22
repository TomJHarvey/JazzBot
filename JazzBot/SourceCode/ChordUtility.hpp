//
//  ChordUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/05/2022.
//

#ifndef ChordUtility_hpp
#define ChordUtility_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <juce_core/juce_core.h>

#include "SequenceTypes.h"

class ChordUtility
{
public:
    static ChordsInKey getChordsInKey(const RootNote& key);
    static RootNote getKey(const std::string& key);
    static std::string convertChordNameToDegree(const ChordsInKey& chords_in_key,
                                                const std::string& chord); // don't really like this name
    static bool parseChordSequence(std::string& chord_sequence_str,
                                   ChordSequence& chord_sequence,
                                   const TimeSignature& time_signature,
                                   const RootNote& key,
                                   const juce::String& file_name);
    static std::string findChordForNote(double& note_on, const ChordSequence& chord_sequence, const bool& next_chord);
    static RootNote findRootNoteForChord(double& note_on, const ChordSequence& chord_sequence, const bool& next_chord);
    static std::size_t findLastBar(const ChordSequence& chord_sequence, const std::size_t& bar_number);
    static RootNote convertNoteValueToRootNote(const int& note_value);
    static int calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2);
    
    static RootNote getChordLetter(const std::string& chord_str);
    
private:
    static RootNote convertStringToRootNote(const std::string& key, const bool& is_minor_key);
    static std::string getSimplifiedChordType(const std::string& chord_type);
    static bool setChordsInCurrentBar(const std::string& current_bar,
                                      ChordSequence& chord_sequence,
                                      const bool& found_chord,
                                      const ChordsInKey& chords_in_key,
                                      const int& bar_length,
                                      std::size_t& bar_number,
                                      int& total_beats);
    static std::size_t findChord(const std::string& current_bar, std::size_t& bar_position);
    static bool checkLastBarHasCorrectMidiTickValue(ChordSequence& chord_sequence,
                                                    const std::size_t& bar_number,
                                                    const int& bar_length);
};

#endif /* ChordUtility_hpp */
