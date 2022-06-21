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


struct Chord
{
    std::string m_chord;
    int m_chord_position;
    std::size_t m_bar_number;
    std::string m_chord_degree;
};

using ChordSequence = std::vector<Chord>;

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
    RootNote m_key; // this should be
    TimeSignature m_time_signature;
};

struct MidiNoteEvent
{
    int note_value;
    double note_on;
    double note_off;
    double duration;
};

using MidiSequence = std::vector<MidiNoteEvent>;

struct Sequence
{
    SongInformation m_song_information;
    ChordSequence m_chord_sequence;
    MidiSequence m_midi_sequence;
};

static const int beat_length = 480;

#endif /* SequenceTypes_h */
