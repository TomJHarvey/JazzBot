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
#include "MidiFileUtility.hpp"

enum class TimeSignature
{
    four_four = 4,
    three_four = 3,
    five_four = 5,
    six_four = 6,
    nine_four = 9,
    not_set = -1
};

struct SongInformation
{
    std::string m_title;
    std::string m_instrument;
    std::string m_key;
    TimeSignature m_time_signature;
};

struct Chord
{
    std::string m_chord;
    int m_chord_position;
    std::size_t m_bar_number;
    // Position in ticks?
};

using ChordSequence = std::vector<Chord>;

struct Sequence
{
    SongInformation m_song_information;
    ChordSequence m_chord_sequence;
    MidiSequence m_midi_sequence;
};

class SequenceUtility
{
public:
    static std::vector<Sequence> generateAllSequenceObjects();
    static TimeSignature convertStringToTimeSignature(const std::string& time_signature_string);
    static bool parseSongInformation(const juce::File& file, SongInformation& song_information);
    static bool parseChordSequence(const juce::File& file,
                                   ChordSequence& chord_sequence,
                                   const TimeSignature& time_signature); // i'll say it again, this really needs a refactor. Some amy go in chord utility.
    static std::size_t findChord(const std::string& current_bar, std::size_t& bar_position);
};

#endif /* SongInformationUtility_hpp */
