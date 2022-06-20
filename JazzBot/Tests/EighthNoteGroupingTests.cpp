#include <catch2/catch_test_macros.hpp>
//#include "TestFiles"
#include "../SourceCode/EighthNotes.hpp"
#include "../SourceCode/SequenceUtility.hpp"
#include "../SourceCode/ChordUtility.hpp"


static juce::String test_file_dir = "../../Tests/TestFiles/";
static juce::String ableton_file_dir = "../../Tests/Ableton/";

static juce::String song_information_dir = "../../Tests/ChordSequenceTestFiles/SequenceFiles/SongInformation/";
static juce::String chord_sequence_dir = "../../Tests/ChordSequenceTestFiles/SequenceFiles/ChordSequence/";
static juce::String midi_file_dir = "../../Tests/ChordSequenceTestFiles/SequenceFiles/MidiFiles/";


// TODO: redo these test cases, might be better to a have a form of error handling thats more verbose.
// returning false could happen for a number of reasons, it would be better to check its false for the right reason.

TEST_CASE("EighthNoteGroupingTests", "[eighth_note_grouping_tests]")
{
    MidiSequence midi_sequence;
    MidiSequence output_sequence;
    juce::File test_file;
    
//    SECTION("Test parsing 1 note")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(ableton_file_dir + "midi_tick_value_test.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        REQUIRE(midi_sequence.size() == 6);
//        REQUIRE(midi_sequence[0].note_on == 480);
//        REQUIRE(midi_sequence[1].note_on == 960);
//        REQUIRE(midi_sequence[2].note_on == 1440);
//        REQUIRE(midi_sequence[3].note_on == 1920);
//        REQUIRE(midi_sequence[4].note_on == 3840);
//        REQUIRE(midi_sequence[5].note_on == 5760);
//    }
//    
//    // Evenly spaced swung 8th notes
//    SECTION("Test 8 eigth notes")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        output_sequence = SequenceUtility::getOnlyEighthNotes(midi_sequence);
//        REQUIRE(output_sequence.size() == 8);
//        output_sequence.clear();
//    }
//    SECTION("Test 8 eighth notes with offset")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_offset.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        output_sequence = SequenceUtility::getOnlyEighthNotes(midi_sequence);
//        REQUIRE(output_sequence.size() == 8);
//        output_sequence.clear();
//    }
//    SECTION("Test 4 eighth notes followed by a sixteenth note triplet")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_1.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        output_sequence = SequenceUtility::getOnlyEighthNotes(midi_sequence);
//        REQUIRE(output_sequence.size() == 5);
//        REQUIRE(output_sequence[4].note_value == 75);
//        output_sequence.clear();
//    }
//    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_2.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        output_sequence = SequenceUtility::getOnlyEighthNotes(midi_sequence);
//        REQUIRE(output_sequence.size() == 5);
//        REQUIRE(output_sequence[0].note_value == 72);
//        output_sequence.clear();
//    }
//
//    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "benny_carter_just_friends_bar_9.mid");
//        REQUIRE(SequenceUtility::parseMidiFile(test_file, midi_sequence, false));
//        output_sequence = SequenceUtility::getOnlyEighthNotes(midi_sequence);
//        REQUIRE(output_sequence.size() == 6);
//        REQUIRE(output_sequence[0].note_value == 70);
//        REQUIRE(output_sequence[5].note_value == 72);
//        output_sequence.clear();
//    }
    
//    SECTION ("8th Note grouping keys tests")
//    {
//        MidiSequence grouping;
//        std::size_t index = 0;
//        SequenceUtility::calculateEighthNoteGroupingKeys(grouping, index);
//    }

    SECTION("Sequence Tests") // these need moving to their own place
    {
        juce::File song_info_file;
        juce::File chord_changes_file;
        juce::File midi_file;
        EighthNotes eighth_notes;
//        SECTION("Test real sequence")
//        {
//            song_info_file = juce::File::getCurrentWorkingDirectory().getChildFile(song_information_dir + "BennyCarter_JustFriends_song_information.txt");
//            chord_changes_file = juce::File::getCurrentWorkingDirectory().getChildFile(chord_sequence_dir + "BennyCarter_JustFriends_chord_changes.txt");
//            midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "BennyCarter_JustFriends.mid");
//            Sequence sequence;
//            REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, true));
//            std::vector<Sequence> sequences;
//            sequences.push_back(sequence);
//            NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
//            REQUIRE(data[0].first.m_chord == "IVj7");
//            REQUIRE(data[0].first.m_beat == "D");
//            REQUIRE(data[0].first.m_direction == "-4");
//            REQUIRE(data[0].first.m_file_name == "BennyCarter_JustFriends");
//            REQUIRE(data[0].first.m_group_size == "13");  // apparently its 13, investigate
//            REQUIRE(data[0].first.m_next_chord == "IV-7");
//            REQUIRE(data[0].first.m_starting_note == "4");
//        }
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
//            SECTION("Test Start of sequence, two times around")
//            {
//                std::cout << "next test" << std::endl;
//                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "start_sequence_second_time.mid");
//                Sequence sequence;
//                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
//                std::vector<Sequence> sequences;
//                sequences.push_back(sequence);
//                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
//                REQUIRE(data.size() == 3);
//                REQUIRE(data[0].first.m_chord == "II-7");
//                REQUIRE(data[0].first.m_beat == "O");
//                REQUIRE(data[0].first.m_direction == "-3");
//                REQUIRE(data[0].first.m_group_size == "4");
//                REQUIRE(data[0].first.m_next_chord == "V7");
//                REQUIRE(data[0].first.m_starting_note == "3");
//            }
//            SECTION("Test middle of sequence")
//            {
//                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "middle_of_sequence.mid");
//                Sequence sequence;
//                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
//                std::vector<Sequence> sequences;
//                sequences.push_back(sequence);
//                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
//                REQUIRE(data.size() == 3);
//                REQUIRE(data[0].first.m_chord == "Ij7");
//                REQUIRE(data[0].first.m_beat == "O");
//                REQUIRE(data[0].first.m_direction == "-3");
//                REQUIRE(data[0].first.m_group_size == "4");
//                REQUIRE(data[0].first.m_next_chord == "VI7");
//                REQUIRE(data[0].first.m_starting_note == "7");
//            }
//            SECTION("Test two chords middle of sequence")
//            {
//                midi_file = juce::File::getCurrentWorkingDirectory().getChildFile(midi_file_dir + "middle_of_sequence_two_chords.mid");
//                Sequence sequence;
//                REQUIRE(SequenceUtility::generateSequenceObject(song_info_file, chord_changes_file, midi_file, sequence, false));
//                std::vector<Sequence> sequences;
//                sequences.push_back(sequence);
//                NoteGroupingData data =  eighth_notes.createDatabaseKeys(sequences);
//                REQUIRE(data.size() == 3);
//                REQUIRE(data[0].first.m_chord == "V7&Ij7");
//                REQUIRE(data[0].first.m_beat == "O");
//                REQUIRE(data[0].first.m_direction == "4");
//                REQUIRE(data[0].first.m_group_size == "2&2");
//                REQUIRE(data[0].first.m_next_chord == "VI7");
//                REQUIRE(data[0].first.m_starting_note == "9");
//            }
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
