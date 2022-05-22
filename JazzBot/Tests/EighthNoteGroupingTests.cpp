#include <catch2/catch_test_macros.hpp>
//#include "TestFiles"
#include "../SourceCode/MidiSequenceUtility.hpp"

static juce::String test_file_dir = "../../Tests/TestFiles/";


// TODO: redo these test cases, might be better to a have a form of error handling thats more verbose.
// returning false could happen for a number of reasons, it would be better to check its false for the right reason.

TEST_CASE("EighthNoteGroupingTests", "[eighth_note_grouping_tests]")
{
    MidiSequence midi_sequence;
    MidiSequence output_sequence;
    juce::File test_file;
    
    // Evenly spaced swung 8th notes
    SECTION("Test 8 eigth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 8);
        output_sequence.clear();
    }
    SECTION("Test 8 eighth notes with offset")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_offset.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 8);
        output_sequence.clear();
    }
    SECTION("Test 3 eighth notes followed by a quarter note and 2 eigth notes") // currently failing
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_1_quarter.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 5);
        REQUIRE(output_sequence[3].note_value == 72);
        output_sequence.clear();
    }
    SECTION("Test 4 eighth notes followed by a sixteenth note triplet")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_1.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 5);
        REQUIRE(output_sequence[4].note_value == 75);
        output_sequence.clear();
    }
    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_2.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 5);
        REQUIRE(output_sequence[0].note_value == 72);
        output_sequence.clear();
    }

    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "benny_carter_just_friends_bar_9.mid");
        REQUIRE(MidiSequenceUtility::parseMidiFile(test_file, midi_sequence, true));
        output_sequence = MidiSequenceUtility::getOnlyEighthNotes(midi_sequence);
        REQUIRE(output_sequence.size() == 6);
        REQUIRE(output_sequence[0].note_value == 70);
        REQUIRE(output_sequence[5].note_value == 72);
        output_sequence.clear();
    }
    
//    SECTION ("8th Note grouping keys tests")
//    {
//        MidiSequence grouping;
//        std::size_t index = 0;
//        MidiSequenceUtility::calculateEighthNoteGroupingKeys(grouping, index);
//    }
}
