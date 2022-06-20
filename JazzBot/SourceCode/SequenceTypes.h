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

using ChordsInKey = std::map<RootNote, std::string>;


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

struct NoteGroupingKey // used to hold the data for various note groupings for the database entry
{
    std::string m_chord;
    std::string m_beat;
    std::string m_starting_note;
    std::string m_group_size;
    std::string m_direction;
    std::string m_next_chord;
    std::string m_file_name;
};

using NoteGroupingData = std::vector<std::pair<NoteGroupingKey, std::vector<int>>>;

#endif /* SequenceTypes_h */
