//
//  SequenceUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//


#include "ChordParsingUtility.hpp"
#include "MidiFileUtility.hpp"
#include "SequenceUtility.hpp"
#include "SongInformationUtility.hpp"
#include <juce_audio_basics/juce_audio_basics.h>
#include <algorithm>
#include <cmath>

//static const juce::File chord_changes_directory(CHORD_CHANGES_DIRECTORY);
//static const juce::File song_information_directory(SONG_INFORMATION_DIRECTORY);

// Todo: Figure out how to remove relative path from macro, why does it work with the file chooser but not here?
static const juce::String song_information_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/song_information";
static const juce::String chord_changes_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/chord_changes";
static const juce::String midi_files_directory = "/Users/tom-harvey/dev/JazzBot/midi_files";

std::vector<Sequence>
SequenceUtility::generateAllSequenceObjects()
{
    std::vector<Sequence> sequences;
    for (const auto& file : juce::File(midi_files_directory).findChildFiles (2, false))
    {
        std::cout << file.getFileName() << std::endl;
        juce::File song_info_file(song_information_directory +
                                  "/" +
                                  file.getFileName().replace(".mid", "_song_information.txt"));
        
        if (song_info_file.exists())
        {
            juce::File chord_changes_file(chord_changes_directory +
                                          "/" +
                                          file.getFileName().replace(".mid", "_chord_changes.txt"));
            
            if (chord_changes_file.exists())
            {
                Sequence sequence;
                if (generateSequenceObject(song_info_file, chord_changes_file, file, sequence, true))
                {
                    sequences.push_back(sequence);
                }
            }
        }
    }
    return sequences;
}

bool
SequenceUtility::generateSequenceObject(const juce::File& song_info_file,
                                        const juce::File& chord_changes_file,
                                        const juce::File& midi_file,
                                        Sequence& sequence,
                                        const bool& ppq_is_480)
{
    // Put this into a function as generate Seuqence object which can be tested.
    SongInformation song_information;
    ChordSequence chord_sequence;
    MidiSequence midi_sequence;

    if (SongInformationUtility::parseSongInformation(song_info_file, song_information))
    {
        if (ChordParsingUtility::parseChordSequence(chord_changes_file,
                                                    chord_sequence,
                                                    song_information.m_time_signature,
                                                    song_information.m_key) &&
            MidiFileUtility::parseMidiFile(midi_file, midi_sequence, ppq_is_480))
        {
            sequence = {song_information, chord_sequence, midi_sequence};
            return true;
        }
    }
    return false;
}

RootNote
SequenceUtility::convertStringToRootNote(const std::string& key, const bool& is_minor_key)
{
    RootNote chord_type = RootNote::Invalid;
    if (key == "A")
    {
        chord_type = RootNote::A;
    }
    else if (key == "Bb" ||
             key == "A#")
    {
        chord_type = RootNote::Bb;
    }
    else if (key == "B" ||
             key == "Cb")
    {
        chord_type = RootNote::B;
    }
    else if (key == "C")
    {
        chord_type = RootNote::C;
    }
    else if (key == "Db" ||
             key == "C#")
    {
        chord_type = RootNote::Db;
    }
    else if (key == "D")
    {
        chord_type = RootNote::D;
    }
    else if (key == "Eb" ||
             key == "D#")
    {
        chord_type = RootNote::Eb;
    }
    else if (key == "E" ||
             key == "Fb")
    {
        chord_type = RootNote::E;
    }
    else if (key == "F")
    {
        chord_type = RootNote::F;
    }
    else if (key == "Gb" ||
             key == "F#")
    {
        chord_type = RootNote::Gb;
    }
    else if (key == "G")
    {
        chord_type = RootNote::G;
    }
    else if (key == "Ab" ||
             key == "G#")
    {
        chord_type = RootNote::Ab;
    }
    else
    {
        std::cout << "key not handled: " << key << std::endl;
        chord_type = RootNote::Invalid;
    }
    
    if (is_minor_key) // add three semi tones so the relative major key is now used
    {
        int chord = static_cast<int>(chord_type) + 3;
        if (chord > 11)
        {
           chord -= 12;
        }
        chord_type = static_cast<RootNote>(chord);
    }
    return chord_type;
}

std::size_t
SequenceUtility::findLastBar(const ChordSequence& chord_sequence, const std::size_t& last_chord_position)
{
    for (std::size_t index = chord_sequence.size() -1; index >= 0; index --)
    {
        if (chord_sequence[index].m_bar_number != last_chord_position) // we don't want - 1, we will do that before entering function in this case
        {
            return (index +1);
        }
    }
    return 0;
}


std::string
SequenceUtility::findChordForNote(const double& note_on, const ChordSequence& chord_sequence, const bool& next_chord, const TimeSignature& time_signature)
{
    std::size_t last_bar =
        SequenceUtility::findLastBar(chord_sequence, chord_sequence[chord_sequence.size()-1].m_bar_number);
    
    const int bar_length = static_cast<int>(time_signature) * beat_length; // maybe but in function...
    double number_of_run_throughs = 0.0f;
    double sequence_positioner = 0.0f;
    double new_note_on = note_on;
    
    // if the current note is not in the first time round of the chord sequence
    if (new_note_on >= (chord_sequence[last_bar].m_chord_position) + bar_length) // changed type
    {
        // find the amount of times the sequence has gone through, then minus that from the note on so it can index the chord sequence
        
        number_of_run_throughs = std::floor(note_on/(chord_sequence[last_bar].m_chord_position + bar_length));
        sequence_positioner = number_of_run_throughs * (chord_sequence[last_bar].m_bar_number + 1) * bar_length; // maybe we want bar number + 1...
        new_note_on = new_note_on - sequence_positioner;
        //std::cout << "new note on = " << new_note_on << " note on = " << note_on << "sp = " << sequence_positioner << std::endl;
    }
    auto it = std::find_if(chord_sequence.begin(),
                           chord_sequence.end(),
                           [&new_note_on](const Chord& chord){return chord.m_chord_position > new_note_on;});
    
    //std::cout << "New note on = " << new_note_on << std::endl;
    if (!next_chord) // commented out for testing if find if is the cause of error
    {
        --it; // At this stage if its at the last chord it will be = to nothing, then it goes back, which takes us to the last bar.
    }
    if (it != chord_sequence.end())
    {
        return it->m_chord_degree;
    }
    else // if its at the end that means it should wrap around to the first chord
    {
        return chord_sequence[0].m_chord_degree;
    }
}

RootNote
SequenceUtility::findRootNoteForChord(const double& note_on, const ChordSequence& chord_sequence, const TimeSignature& time_signature) // function needs a refactor into one above
{
    std::size_t last_bar =
        SequenceUtility::findLastBar(chord_sequence, chord_sequence[chord_sequence.size()-1].m_bar_number);
    
    const int bar_length = static_cast<int>(time_signature) * beat_length; // maybe but in function...
    double new_note_on = note_on;
    
    // this should be last bar + bar length
    double last_bar_position = chord_sequence[last_bar].m_chord_position + bar_length;
    
    // if the current note is not in the first time round of the chord sequence
    if (new_note_on > chord_sequence[last_bar].m_chord_position) // changed type
    {
        // find the amount of times the sequence has gone through, then minus that from the note on so it can index the chord sequence
        double sequence_positioner = last_bar_position * std::floor(note_on/(last_bar_position));
        new_note_on -= sequence_positioner;
    }
    
    auto it = std::find_if(chord_sequence.begin(),
                           chord_sequence.end(),
                           [&new_note_on](const Chord& chord){return chord.m_chord_position > new_note_on;});
    // what happens if it reaches the end?
    it --;
    
    return getChordLetter(it->m_chord); // commented out for test.
}

// This function here will need to get the bar number
// It will first need to provide the note on for the first note in the sequence
// if it is over the last chord, then it will get the value of sequence positioner, if not it will set it to (0 or 1, need to check...)
// then it will search and get the iterators m_current_bar

// for now i'll just make a new function, then i can think about refactoring. It can return a string that will be used in the database.
std::string
SequenceUtility::getLocation(const double& note_on, const ChordSequence& chord_sequence, const TimeSignature& time_signature)
{
    // 0 is first time, then 1,2,3,4
    std::size_t last_bar =
        SequenceUtility::findLastBar(chord_sequence, chord_sequence[chord_sequence.size()-1].m_bar_number);
    
    const int bar_length = static_cast<int>(time_signature) * beat_length; // maybe but in function...
    
    double number_of_run_throughs = 0.0f;
    double sequence_positioner = 0.0f;
    
    double new_note_on = note_on;
    
    // if the current note is not in the first time round of the chord sequence
    if (new_note_on >= (chord_sequence[last_bar].m_chord_position) + bar_length) // changed type
    {
        // find the amount of times the sequence has gone through, then minus that from the note on so it can index the chord sequence
        
        number_of_run_throughs = std::floor(note_on/(chord_sequence[last_bar].m_chord_position + bar_length));
        sequence_positioner = number_of_run_throughs * (chord_sequence[last_bar].m_bar_number + 1) * bar_length; // maybe we want bar number + 1...
        new_note_on = new_note_on - sequence_positioner;
        //std::cout << "new note on = " << new_note_on << " note on = " << note_on << "sp = " << sequence_positioner << std::endl;
    }
    
    auto it = std::find_if(chord_sequence.begin(),
                           chord_sequence.end(),
                           [&new_note_on](const Chord& chord){return chord.m_chord_position > new_note_on;});
    
    std::string bar_number;
    if (it != chord_sequence.end())
    {
        bar_number =  std::to_string(it->m_bar_number);
    }
    else // if its at the end that means it should wrap around to the first chord
    {
        bar_number = std::to_string(chord_sequence[0].m_bar_number);
    }
    
    return std::to_string(static_cast<int>(number_of_run_throughs)) + "&" + bar_number;
}


RootNote
SequenceUtility::convertNoteValueToRootNote(const int& note_value)
{
    float octave = (note_value / 12);
    int rounded_octave =  static_cast<int>(std::floor(octave));
    int note_degree =  note_value - (rounded_octave * 12);
    
    RootNote chord_type = RootNote::Invalid;
    switch (note_degree) {
        case 0:
            chord_type = RootNote::C;
            break;
        case 1:
            chord_type = RootNote::Db;
            break;
        case 2:
            chord_type = RootNote::D;
            break;
        case 3:
            chord_type = RootNote::Eb;
            break;
        case 4:
            chord_type = RootNote::E;
            break;
        case 5:
            chord_type = RootNote::F;
            break;
        case 6:
            chord_type = RootNote::Gb;
            break;
        case 7:
            chord_type = RootNote::G;
            break;
        case 8:
            chord_type = RootNote::Ab;
            break;
        case 9:
            chord_type = RootNote::A;
            break;
        case 10:
            chord_type = RootNote::Bb;
            break;
        case 11:
            chord_type = RootNote::B;
            break;
        default:
            break;
    }
    return chord_type;
}


int
SequenceUtility::calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2)
{
    int search_note = static_cast<int>(note_1); // root for chord
    int target_note = static_cast<int>(note_2);
    int counter = 0;
    while (search_note != target_note)
    {
        search_note ++;
        counter ++;
        if (search_note > 11)
        {
            search_note -=12;
        }
    }
    return counter;
}

RootNote
SequenceUtility::getChordLetter(const std::string& chord)
{
    std::string chord_str = chord;
    std::size_t slash_index = chord.find('/');
    if (slash_index != std::string::npos)
    {
        chord_str = chord_str.substr(0, slash_index);
    } // else exit?
    
    std::string chord_root_str;
    std::string chord_type;
    
    bool sharp_to_flat = false;
    if (chord_str.size() > 1 && chord_str[1] == 'b')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
    }
    else if (chord_str.size() > 1 && chord_str[1] == '#')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
        sharp_to_flat = true;
    }
    else
    {
        chord_root_str = chord_str.substr(0, 1);
        chord_type = chord_str.substr(1, chord_str.size()-1);
    }
    RootNote chord_root = SequenceUtility::convertStringToRootNote(chord_root_str, false);
    return chord_root;
}
