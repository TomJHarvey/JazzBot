//
//  ChordParsingUtility.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 21/06/2022.
//

#ifndef ChordParsingUtility_hpp
#define ChordParsingUtility_hpp

#include <stdio.h>
#include <string>
#include <juce_core/juce_core.h>
#include "../SequenceTypes.h"

using ChordsInKey = std::map<RootNote, std::string>;

class ChordParsingUtility
{
public:
    static RootNote getKey(const std::string& key);
    static bool parseChordSequence(const juce::File& chord_changes_file,
                                   ChordSequence& chord_sequence,
                                   const TimeSignature& time_signature,
                                   const RootNote& key);
    
    
private:
    static ChordsInKey getChordsInKey(const RootNote& key);
    static std::string convertChordNameToDegree(const ChordsInKey& chords_in_key,
                                                const std::string& chord); // don't really like this name
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
    static std::string getChordSequenceAsString(const juce::File& file);
    
};

#endif /* ChordParsingUtility_hpp */
