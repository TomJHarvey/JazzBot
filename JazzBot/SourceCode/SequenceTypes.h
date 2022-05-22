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
    ChordRoot m_key; // this should be
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

struct EighthNoteGroupingKey
{
    std::string m_chord;
    std::string m_beat;
    std::string m_starting_note;
    std::string m_group_size;
    std::string m_direction;
    std::string m_next_chord;
    std::string m_file_name;
};

using EighthNoteGroupingData = std::vector<std::pair<EighthNoteGroupingKey, std::vector<int>>>;

#endif /* SequenceTypes_h */
