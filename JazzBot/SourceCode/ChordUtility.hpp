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

enum class ChordRoot
{
    A = 0,
    Bb,
    B,
    C,
    Db,
    D,
    Eb,
    E,
    F,
    Gb,
    G,
    Ab,
    Invalid = -1
};

//struct Chord
//{
//    std::string m_chord;
//    std::size_t m_number_of_beats;
//    std::size_t m_bar_number;
//    // Position in ticks?
//};
//
//using ChordSequence = std::vector<Chord>;

using ChordsInKey = std::map<ChordRoot, std::string>;

struct ChordPosition
{
    std::string m_chord_type;
    double m_time_stamp;
};


// Might be better name midiSequenceUtility
class ChordUtility
{
public:
    static ChordsInKey getChordsInKey(const ChordRoot& key);
    static ChordRoot getKey(const std::string& key);
    static ChordRoot convertStringToChordRoot(const std::string& key, const bool& is_minor_key);
    static std::string convertChordNameToDegree(const ChordsInKey& chords_in_key, const std::string& chord); // don't really like this name
    static std::string getSimplifiedChordType(const std::string& chord_type);
};

#endif /* ChordUtility_hpp */
