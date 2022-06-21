//
//  EighthNoteGroupingGenerationTests.cpp
//  tests
//
//  Created by Tom Harvey on 21/06/2022.
//

#include <stdio.h>

#include <catch2/catch_test_macros.hpp>
//#include "TestFiles"
#include "../SourceCode/NoteGrouping/EighthNotes.hpp"
#include "../SourceCode/Utility/SequenceUtility.hpp"

static juce::String song_information_dir = "../../Tests/TestFiles/EighthNoteGeneration/SongInformation/";
static juce::String chord_sequence_dir = "../../Tests/TestFiles/EighthNoteGeneration/ChordSequence/";
static juce::String midi_file_dir = "../../Tests/TestFiles/EighthNoteGeneration/MidiFiles/";

TEST_CASE("EighthNoteGroupingGenerationTests", "[eighth_note_grouping_generation_tests]")
{
    SECTION("Sequence Tests") // these need moving to their own place
    {
        juce::File song_info_file;
        juce::File chord_changes_file;
        juce::File midi_file;
        EighthNotes eighth_notes;
        SECTION("Test real sequence")
        {
            song_info_file = juce::File::getCurrentWorkingDirectory().getChildFile(song_information_dir + "BennyCarter_JustFriends_song_information.txt");
            chord_changes_file = juce::File::getCurrentWorkingDirectory().getChildFile(chord_sequence_dir + "BennyCarter_JustFriends_chord_changes.txt");
            midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "BennyCarter_JustFriends.mid");
            Sequence sequence;
            REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, true));
            std::vector<Sequence> sequences;
            sequences.push_back(sequence);
            NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
            REQUIRE(data[0].first.m_chord == "IVj7");
            REQUIRE(data[0].first.m_beat == "D");
            REQUIRE(data[0].first.m_direction == "-4");
            REQUIRE(data[0].first.m_file_name == "BennyCarter_JustFriends");
            REQUIRE(data[0].first.m_group_size == "13");  // apparently its 13, investigate
            REQUIRE(data[0].first.m_next_chord == "IV-7");
            REQUIRE(data[0].first.m_starting_note == "4");
        }
        SECTION("Test 2561 chord sequence in C")
        {
            chord_changes_file = juce::File::getCurrentWorkingDirectory().getChildFile(chord_sequence_dir + "2_5_1_6_chord_changes.txt");
            song_info_file = juce::File::getCurrentWorkingDirectory().getChildFile(song_information_dir + "2_5_6_1_song_information.txt");
            SECTION("Test Start of sequence")
            {
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "start_sequence.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "II-7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "-3");
                REQUIRE(data[0].first.m_group_size == "4");
                REQUIRE(data[0].first.m_next_chord == "V7");
                REQUIRE(data[0].first.m_starting_note == "3");
            }
            SECTION("Test Start of sequence, two times around")
            {
                std::cout << "next test" << std::endl;
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "start_sequence_second_time.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "II-7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "-3");
                REQUIRE(data[0].first.m_group_size == "4");
                REQUIRE(data[0].first.m_next_chord == "V7");
                REQUIRE(data[0].first.m_starting_note == "3");
            }
            SECTION("Test middle of sequence")
            {
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "middle_of_sequence.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "Ij7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "-3");
                REQUIRE(data[0].first.m_group_size == "4");
                REQUIRE(data[0].first.m_next_chord == "VI7");
                REQUIRE(data[0].first.m_starting_note == "7");
            }
            SECTION("Test two chords middle of sequence")
            {
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "middle_of_sequence_two_chords.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "V7&Ij7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "4");
                REQUIRE(data[0].first.m_group_size == "2&2");
                REQUIRE(data[0].first.m_next_chord == "VI7");
                REQUIRE(data[0].first.m_starting_note == "9");
                REQUIRE(data[1].first.m_group_size == "1&2");
                REQUIRE(data[2].first.m_group_size == "2&1");
            }
            SECTION("Test two chords end of sequence")
            {
                std::cout << "one times tests" << std::endl;
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "end_of_sequence.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "VI7&II-7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "-1");
                REQUIRE(data[0].first.m_group_size == "2&2");
                REQUIRE(data[0].first.m_next_chord == "V7");
                REQUIRE(data[0].first.m_starting_note == "4");
                REQUIRE(data[0].first.m_group_size == "2&2");
                REQUIRE(data[1].first.m_group_size == "1&2");
                REQUIRE(data[2].first.m_group_size == "2&1");
            }
            SECTION("Test two chords end of sequence, two times around")
            {
                std::cout << "two times tests" << std::endl;
                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "end_of_sequence_second_time.mid");
                Sequence sequence;
                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
                std::vector<Sequence> sequences;
                sequences.push_back(sequence);
                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
                REQUIRE(data.size() == 3);
                REQUIRE(data[0].first.m_chord == "VI7&II-7");
                REQUIRE(data[0].first.m_beat == "O");
                REQUIRE(data[0].first.m_direction == "-1");
                REQUIRE(data[0].first.m_group_size == "2&2");
                REQUIRE(data[0].first.m_next_chord == "V7");
                REQUIRE(data[0].first.m_starting_note == "4");
                REQUIRE(data[1].first.m_group_size == "1&2");
                REQUIRE(data[2].first.m_group_size == "2&1");
            }
        }
    }
}
