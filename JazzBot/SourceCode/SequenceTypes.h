//
//  SequenceTypes.h
//  JAZZBOT
//
//  Created by Tom Harvey on 21/05/2022.
//

#ifndef SequenceTypes_h
#define SequenceTypes_h

#include <map>
#include <string>
#include <vector>


/**
    @brief Used to store the root note C-B as enum values 0-11
 */
enum class RootNote
{
    C = 0,
    Db,
    D,
    Eb,
    E,
    F,
    Gb,
    G,
    Ab,
    A,
    Bb,
    B,
    Invalid = -1
};

/**
    @brief Stores information about the current chord
 */
struct Chord
{
    std::string m_chord;
    int m_chord_position;
    std::size_t m_bar_number;
    std::string m_chord_degree;
};

/**
    @brief A vector  of chords
 */
using ChordSequence = std::vector<Chord>;

/**
    @brief Stores the possible time signature values
 */
enum class TimeSignature
{
    four_four = 4,
    three_four = 3,
    five_four = 5,
    six_four = 6,
    nine_four = 9,
    not_set = -1
};

/**
    @brief Stores information about the current song
 */
struct SongInformation
{
    std::string m_title;
    std::string m_instrument;
    RootNote m_key; // this should be
    TimeSignature m_time_signature;
};

/**
    @brief Stores note on and off, duration and note value for a midi event
 */
struct MidiNoteEvent
{
    int note_value;
    double note_on;
    double note_off;
    double duration;
};

/**
    @brief A vector of midi events to create a midi sequence
 */
using MidiSequence = std::vector<MidiNoteEvent>;

/**
    @brief A sequence which is  made up of three types. 1.song information 2. chord sequence 3.midi sequence
 */
struct Sequence
{
    SongInformation m_song_information;
    ChordSequence m_chord_sequence;
    MidiSequence m_midi_sequence;
};

/**
    @brief Defines the beat length
 */
static const int beat_length = 480;

#endif /* SequenceTypes_h */
