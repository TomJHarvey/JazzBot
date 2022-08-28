#include <catch2/catch_test_macros.hpp>
//#include "TestFiles"
#include "../SourceCode/NoteGrouping/EighthNotes.hpp"
#include "../SourceCode/Utility/MidiFileUtility.hpp"
#include "../SourceCode/Utility/SongInformationUtility.hpp"

static juce::String test_file_dir = "../../Tests/TestFiles/EighthNoteDetection/";
static juce::String ableton_file_dir = "../../Tests/TestFiles/EighthNoteDetection/Ableton/";

// TODO: redo these test cases, might be better to a have a form of error handling thats more verbose.
// returning false could happen for a number of reasons, it would be better to check its false for the right reason.

TEST_CASE("EighthNoteGroupingDetectionTests", "[eighth_note_grouping_detection_tests]")
{
    MidiSequence midi_sequence;
    MidiSequence output_sequence;
    juce::File test_file;
    EighthNotes eighth_notes;
    
    SECTION("Test parsing 1 note")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(ableton_file_dir + "midi_tick_value_test.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        REQUIRE(midi_sequence.size() == 6);
        REQUIRE(midi_sequence[0].note_on == 480);
        REQUIRE(midi_sequence[1].note_on == 960);
        REQUIRE(midi_sequence[2].note_on == 1440);
        REQUIRE(midi_sequence[3].note_on == 1920);
        REQUIRE(midi_sequence[4].note_on == 3840);
        REQUIRE(midi_sequence[5].note_on == 5760);
    }
    
    // Evenly spaced swung 8th notes
    SECTION("Test 8 eigth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        output_sequence = eighth_notes.getModifiedSequence(midi_sequence);
        REQUIRE(output_sequence.size() == 8);
        output_sequence.clear();
    }
    SECTION("Test 8 eighth notes with offset")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_offset.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        output_sequence = eighth_notes.getModifiedSequence(midi_sequence);
        REQUIRE(output_sequence.size() == 8);
        output_sequence.clear();
    }
    SECTION("Test 4 eighth notes followed by a sixteenth note triplet")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_1.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        output_sequence = eighth_notes.getModifiedSequence(midi_sequence);
        REQUIRE(output_sequence.size() == 5);
        REQUIRE(output_sequence[4].note_value == 75);
        output_sequence.clear();
    }
    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_2.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        output_sequence = eighth_notes.getModifiedSequence(midi_sequence);
        REQUIRE(output_sequence.size() == 5);
        REQUIRE(output_sequence[0].note_value == 72);
        output_sequence.clear();
    }

    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "benny_carter_just_friends_bar_9.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, false));
        output_sequence = eighth_notes.getModifiedSequence(midi_sequence);
        REQUIRE(output_sequence.size() == 6);
        REQUIRE(output_sequence[0].note_value == 70);
        REQUIRE(output_sequence[5].note_value == 72);
        output_sequence.clear();
    }
}
