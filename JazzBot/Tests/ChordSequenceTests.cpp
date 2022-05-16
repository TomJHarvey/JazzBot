#include <catch2/catch_test_macros.hpp>
#include "../SourceCode/SequenceUtility.hpp"

static juce::String test_file_dir = "../../Tests/ChordSequenceTestFiles/";

TEST_CASE("ChordSequenceTests", "[chord_sequence_tests]")
{
    juce::File test_file;
    SECTION("One bar tests")
    {
//        SECTION("Test one normal chord")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "chord_first_beat.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 1);
//            REQUIRE(chord_sequence[0].m_chord == "Eb-");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 4);
//        }
//        SECTION("Test two normal chords")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_chords.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Eb-");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_chord == "Amaj7");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_bar_number == 0);
//        }
//        SECTION("Test one slash chord")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_chord_first_beat.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 1);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6/Bb");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 4);
//        }
//        SECTION("Test 2 slash chords")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_slash_chords.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6/Bb");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_chord == "A-/D7b9b5");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_bar_number == 0);
//        }
//        SECTION("Normal and slash")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "normal_and_slash.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_chord == "C/E");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_bar_number == 0);
//        }
//        SECTION("Slash and normal")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_and_normal.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Cmaj7/E");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_chord == "Eb-6");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_bar_number == 0);
//        }
//        SECTION("Four normal")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "four_normal.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A7b9#11");
//            REQUIRE(chord_sequence[3].m_number_of_beats == 1);
//        }
//        SECTION("Four Slash")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "four_slash.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C/E");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7/G");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A/D");
//            REQUIRE(chord_sequence[3].m_number_of_beats == 1);
//        }
//        SECTION("Two slash two normal")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_slash_two_normal.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C/E");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A");
//            REQUIRE(chord_sequence[3].m_number_of_beats == 1);
//        }
//        SECTION("Two normal two slash")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_normal_two_slash.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7/G");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A/D");
//        }
//        SECTION("Slash normal alternate")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_normal_alternate.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7/G");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A");
//            REQUIRE(chord_sequence[3].m_number_of_beats == 1);
//        }
//        SECTION("Normal slash alternate")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "normal_slash_alternate.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 4);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "C/E");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[3].m_chord == "A/D");
//            REQUIRE(chord_sequence[3].m_number_of_beats == 1);
//        }
//        SECTION("One beat, three beats")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "one_three.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 3);
//        }
//        SECTION("Three beats, one beat")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "three_one.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 2);
//            REQUIRE(chord_sequence[0].m_chord == "Eb/A");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 3);
//            REQUIRE(chord_sequence[1].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//        }
//        SECTION("One beat one beat two beats")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "one_one_two.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 3);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[1].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "C/E");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 2);
//        }
//        SECTION("Two beats one beat one beat")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_one_one.txt");
//            ChordSequence chord_sequence;
//            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
//            REQUIRE(chord_sequence.size() == 3);
//            REQUIRE(chord_sequence[0].m_chord == "Eb6");
//            REQUIRE(chord_sequence[0].m_number_of_beats == 2);
//            REQUIRE(chord_sequence[1].m_chord == "Dmaj7");
//            REQUIRE(chord_sequence[1].m_number_of_beats == 1);
//            REQUIRE(chord_sequence[2].m_chord == "C/E");
//            REQUIRE(chord_sequence[2].m_number_of_beats == 1);
//        }
        SECTION("Two beats one beat one beat")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "too_many_beats_1.txt");
            ChordSequence chord_sequence;
            SequenceUtility::parseChordSequence(test_file, chord_sequence, TimeSignature::four_four);
            REQUIRE(chord_sequence.size() == 0);
        }
    }
}
