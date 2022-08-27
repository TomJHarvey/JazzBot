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

/**
    @brief Utility class for operations related to generating and reading sequenences 
 */
class SequenceUtility
{
public:
    static std::vector<Sequence> generateAllSequenceObjects();
    
    /**
     @brief Generates a Sequence object
     @param song_info_file to read
     @param chord_changes_file to read
     @param midi_file to read
     @param sequence to initalise
     @param ppq_is_480 to determine the pulses per quarter note
     @returns True if successfully generated object
     */
    static bool generateSequenceObject(const juce::File& song_info_file,
                                       const juce::File& chord_changes_file,
                                       const juce::File& midi_file,
                                       Sequence& sequence,
                                       const bool& ppq_is_480);
    
    /**
     @brief Locates the last bar in a song. This is needed as the alst bar of the song may contain two chords, so using the last
            chord wont work in that isntance.
     @param last_chord_position to check against
     @param chord_sequence to search through
     @returns The last bar number
     */
    static std::size_t findLastBar(const ChordSequence& chord_sequence,
                                   const std::size_t& last_chord_position);
   
    /**
     @brief Converts a chord letter e.g A, Bb, G# to a RootNote value
     @param key the chord letter
     @param is_minor_key if its a minor key
     @returns The converted root note
     */
    static RootNote convertStringToRootNote(const std::string& key, const bool& is_minor_key);
    
    /**
     @brief Converts a midi note value to a RootNote value
     @param note_value midi note number
     @returns The converted root note
     */
    static RootNote convertNoteValueToRootNote(const int& note_value);
    
    /**
     @brief Calculates the difference between two root notes
     @param note_1 to test
     @param note_2 to test
     @returns The difference between the two values
     */
    static int calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2);
    
    /**
     @brief Converts a chord to its root note value e.g Cjj7 to C or Bb-7 to Bb
     @param note_1 to test
     @param note_2 to test
     @returns The root note for the chord
     */
    static RootNote getChordLetter(const std::string& chord_str);
    
    /**
     @brief The chord as a string for the current note
     @param note_on timestamp of the current note
     @param chord_sequence to find the chord in
     @param next_chord if its looking for the the next chord
     @param time_signature of the current chord sequence
     @returns The chord as a string
     */
    static std::string findChordForNote(const double& note_on,
                                 const ChordSequence& chord_sequence,
                                 const bool& next_chord,
                                 const TimeSignature& time_signature);
    
    /**
     @brief The chord as a string for the current note
     @param note_on timestamp of the current note
     @param chord_sequence to find the chord in
     @param time_signature of the current chord sequence
     @returns The root note for the chord
     */
    static RootNote findRootNoteForChord(const double& note_on,
                                  const ChordSequence& chord_sequence,
                                  const TimeSignature& time_signature);
    
    /**
     @brief The bar number and the number of run throughs for the current note on
     @param note_on timestamp of the current note
     @param chord_sequence to find the chord in
     @param time_signature of the current chord sequence
     @returns The location of the note
     */
    static std::string getLocation(const double& note_on,
                            const ChordSequence& chord_sequence,
                            const TimeSignature& time_signature);
};

#endif /* SequenceUtility_hpp */
