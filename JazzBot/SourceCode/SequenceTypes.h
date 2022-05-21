//
//  SequenceTypes.h
//  JAZZBOT
//
//  Created by Tom Harvey on 21/05/2022.
//

#ifndef SequenceTypes_h
#define SequenceTypes_h

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

using ChordsInKey = std::map<ChordRoot, std::string>;

struct ChordPosition
{
    std::string m_chord_type;
    double m_time_stamp;
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
    std::string m_key;
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
using BeatMarkers = std::pair<double, bool>;

struct Sequence
{
    SongInformation m_song_information;
    ChordSequence m_chord_sequence;
    MidiSequence m_midi_sequence;
};

#endif /* SequenceTypes_h */
