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

/**
    @brief Contains the root note and the scale degree for the chords in a key
 */
using ChordsInKey = std::map<RootNote, std::string>;

/**
    @brief Utility class for chord operations
 */
class ChordParsingUtility
{
public:
    
    /**
     @brief Generates a Sequence object
     @param key for chord sequence
     @returns The root note for a key
     */
    static RootNote getKey(const std::string& key);
    
    /**
     @brief Parses a chord sequence file into a ChordSequence object
     @param chord_changes_file to read from
     @param chord_sequence to populate
     @param time_signature of the current chord sequence
     @param key of the chord sequence
     @returns If chord sequence parsed correctyl
     */
    static bool parseChordSequence(const juce::File& chord_changes_file,
                                   ChordSequence& chord_sequence,
                                   const TimeSignature& time_signature,
                                   const RootNote& key);
    
    
private:
    /**
     @brief Gets the chords in the key
     @param key of the chord sequence
     @returns The chords in the key
     */
    static ChordsInKey getChordsInKey(const RootNote& key);
    
    /**
     @brief Converts the chord name to its chord quality its type (e.g min7, maj7, dom7 etc) +  its scale degree. (e.g I VI, VII, bII, bVII)
     @param chords_in_key for the current sequence
     @param chord to convert
     @returns The chord as its degree e.g bIIm7, VI7, IIIj7
     */
    static std::string convertChordNameToDegree(const ChordsInKey& chords_in_key,
                                                const std::string& chord); // don't really like this name
    
    /**
     @brief Converts a chord to its chord type e.g C-79 becomes C-7 Ebj69 becomes Ebj
     @param chord_type to simplify
     @returns The simplified chord type
     */
    static std::string getSimplifiedChordType(const std::string& chord_type);
    
    /**
     @brief Sets the chords in the ChordSequence object for the current bar
     @param current_bar_chords the chords in the current bar
     @param chord_sequence to append new chords to
     @param found_chord true if found a chord
     @param chords_in_key for the current sequence
     @param bar_length for the current bar
     @param bar_number of the current bar
     @param total_beats calculated through the current bar. Used to make sure its not adding extra beats in the bar
     @returns If set the chords in the bar
     */
    static bool setChordsInCurrentBar(const std::string& current_bar_chords,
                                      ChordSequence& chord_sequence,
                                      const bool& found_chord,
                                      const ChordsInKey& chords_in_key,
                                      const int& bar_length,
                                      std::size_t& bar_number,
                                      int& total_beats);
    /**
     @brief Finds the number of beats for each chord
     @param current_bar_chords the chords in the current bar
     @param bar_position the position in the bar
     @returns The number of beats for the chord
     */
    static std::size_t findNumberOfbeatsForChord(const std::string& current_bar_chords, std::size_t& bar_position);
    
    /**
     @brief Checks the last bar is the expected midi tick value. Function is called as a precatuion to ensure calculations are correct
     @param chord_sequence for current song
     @param bar_number after constructing chord sequence
     @param bar_length after constructing chord sequence
     @returns If the chord sequence is alligned correctly
     */
    static bool checkLastBarHasCorrectMidiTickValue(ChordSequence& chord_sequence,
                                                    const std::size_t& bar_number,
                                                    const int& bar_length);
    /**
     @brief Gets the chord sequence as a string
     @param file containing chord sequence
     @returns The chord sequence as  string
     */
    static std::string getChordSequenceAsString(const juce::File& file);
};

#endif /* ChordParsingUtility_hpp */
