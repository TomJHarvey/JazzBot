//
//  SequenceUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 09/05/2022.
//


#include "ChordParsingUtility.hpp"
#include "SequenceUtility.hpp"
#include <juce_audio_basics/juce_audio_basics.h>
#include <algorithm>

//static const juce::File chord_changes_directory(CHORD_CHANGES_DIRECTORY);
//static const juce::File song_information_directory(SONG_INFORMATION_DIRECTORY);

// Todo: Figure out how to remove relative path from macro, why does it work with the file chooser but not here?
static const juce::String song_information_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/song_information";
static const juce::String chord_changes_directory = "/Users/tom-harvey/dev/JazzBot/DatabaseParser/chord_changes";
static const juce::String midi_files_directory = "/Users/tom-harvey/dev/JazzBot/midi_files";

static const std::string four_four_string = "4/4";
static const std::string three_four_string = "3/4";
static const std::string five_four_string = "5/4";
static const std::string six_four_string = "6/4";
static const std::string nine_four_string = "9/4";

static const std::size_t num_sequence_elements = 4;

static const int test_scaling = 10;
static const int test_divison_scaling = 2;

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

    if (parseSongInformation(song_info_file, song_information))
    {
        if (ChordParsingUtility::parseChordSequence(chord_changes_file,
                                                    chord_sequence,
                                                    song_information.m_time_signature,
                                                    song_information.m_key) &&
            SequenceUtility::parseMidiFile(midi_file, midi_sequence, ppq_is_480))
        {
            sequence = {song_information, chord_sequence, midi_sequence};
            return true;
        }
    }
    return false;
}

bool
SequenceUtility::parseMidiFile(const juce::File& file, MidiSequence& midi_events, const bool& ppq_is_480)
{
    juce::MidiFile midi_file;
    juce::FileInputStream fi_stream(file);
    jassert(fi_stream.openedOk());
    midi_file.readFrom(fi_stream);
    
    // This is so if notes overlap it can parse them correctly
    std::map<int, double> current_note_ons;
    
    for (int num_tracks = 0; num_tracks < midi_file.getNumTracks(); num_tracks ++)
    {
        const juce::MidiMessageSequence* midi_message_sequence = midi_file.getTrack(num_tracks);
        for (int midi_event = 0; midi_event < midi_message_sequence->getNumEvents(); midi_event ++)
        {
            const juce::MidiMessageSequence::MidiEventHolder * midi_event_holder =
                midi_message_sequence->getEventPointer(midi_event);
            
            if (midi_event_holder->message.isNoteOn())
            {
                const double note_on_timestamp = midi_event_holder->message.getTimeStamp();
                
                // check for duplicate? Doesn't really matter, its hndled in the next else if
                current_note_ons.insert(std::make_pair(midi_event_holder->message.getNoteNumber(),
                                                       note_on_timestamp));
            }
            else if (midi_event_holder->message.isNoteOff())
            {
                int note_number = midi_event_holder->message.getNoteNumber();
                
                // retrieve the current notes corresponding note on value
                double note_on_timestamp = current_note_ons[note_number];
                
                // this is to handle instances where the note of the same value overlaps
                if (note_on_timestamp != 0 || midi_events.size() == 0)
                {
                    double note_off_timestamp = midi_event_holder->message.getTimeStamp();
                    double duration = note_off_timestamp - note_on_timestamp;
                    if (ppq_is_480)
                    {
                        midi_events.push_back({note_number,
                            note_on_timestamp,
                            note_off_timestamp,
                            duration});
                    }
                    else // the test files are written in ableton where the midi tick value is different
                    {
                        midi_events.push_back({note_number,
                                               (note_on_timestamp/test_divison_scaling)*test_scaling,
                                               (note_off_timestamp/test_divison_scaling)*test_scaling,
                                               (duration/test_divison_scaling)*test_scaling});
                    } 
                }
                current_note_ons.erase(note_number);
            }
        }
    }
    return !midi_events.empty();
}


bool
SequenceUtility::parseSongInformation(const juce::File& file, SongInformation& song_information)
{
    if (!file.existsAsFile())
    {
        return false;
    }
    juce::FileInputStream inputStream (file);
    if (! inputStream.openedOk())
    {
        return false;
    }
    std::vector<std::string> sequence_elements;
    while (!inputStream.isExhausted())
    {
        auto line = inputStream.readNextLine();
        sequence_elements.push_back(line.toStdString());
    }
    if (sequence_elements.size() != num_sequence_elements)
    {
        return false;
    }
    else
    {
        TimeSignature time_signature = convertStringToTimeSignature(sequence_elements[3]);
        RootNote key = ChordParsingUtility::getKey(sequence_elements[2]);
        if (time_signature != TimeSignature::not_set &&
            key != RootNote::Invalid)
        {
            song_information = {sequence_elements[0],
                                sequence_elements[1],
                                key,
                                time_signature};
            return true;
        }
        else
        {
            return false;
        }
    }
}

TimeSignature
SequenceUtility::convertStringToTimeSignature(const std::string& time_signature_string)
{
    TimeSignature time_signature = TimeSignature::not_set;
    if (time_signature_string == four_four_string)
    {
        time_signature = TimeSignature::four_four;
    }
    else if (time_signature_string == three_four_string)
    {
        time_signature = TimeSignature::three_four;
    }
    else if (time_signature_string == five_four_string)
    {
        time_signature = TimeSignature::five_four;
    }
    else if (time_signature_string == six_four_string)
    {
        time_signature = TimeSignature::six_four;
    }
    else if (time_signature_string == nine_four_string)
    {
        time_signature = TimeSignature::nine_four;
    }
    else
    {
        std::cout << "Time signature " << time_signature_string <<  " not handled " << std::endl;
    }
    return time_signature;
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
SequenceUtility::findLastBar(const ChordSequence& chord_sequence, const std::size_t& bar_number)
{
    for (std::size_t index = chord_sequence.size() -1; index >= 0; index --)
    {
        if (chord_sequence[index].m_bar_number != bar_number) // we don't want - 1, we will do that before entering function in this case
        {
            return (index +1);
        }
    }
    return 0;
}
