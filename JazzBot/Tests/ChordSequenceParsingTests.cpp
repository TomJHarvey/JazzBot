#include <catch2/catch_test_macros.hpp>
#include "../SourceCode/Utility/SequenceUtility.hpp"
#include "../SourceCode/Utility/ChordParsingUtility.hpp"

static juce::String test_file_dir = "../../Tests/TestFiles/ChordSequenceParsing/";

TEST_CASE("ChordSequenceParsingTests", "[chord_sequence_parsing_tests]")
{
    juce::File test_file;
    SECTION("One bar 4/4 tests")
    {
        SECTION("Test one normal chord")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "chord_first_beat.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Db, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Eb-");
            REQUIRE(chord_sequence[0].m_chord_degree == "II-7");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
        }
        SECTION("Test two normal chords")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Db, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Eb-");
            REQUIRE(chord_sequence[0].m_chord_degree == "II-7");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "Ab7");
            REQUIRE(chord_sequence[1].m_chord_degree == "V7");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
            REQUIRE(chord_sequence[1].m_bar_number == 0);
        }
        SECTION("Test one slash chord")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_chord_first_beat.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Eb, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Eb6/Bb");
            REQUIRE(chord_sequence[0].m_chord_degree == "I6");
        }
        SECTION("Test 2 slash chords")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_slash_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Eb, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Eb6/Bb");
            REQUIRE(chord_sequence[0].m_chord_degree == "I6");
            REQUIRE(chord_sequence[1].m_chord == "A-/D7b9b5");
            REQUIRE(chord_sequence[1].m_chord_degree == "bV-7");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
            REQUIRE(chord_sequence[1].m_bar_number == 0);
        }
        SECTION("Normal and slash")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "normal_and_slash.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Eb, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "I6");
            REQUIRE(chord_sequence[1].m_chord == "C/E");
            REQUIRE(chord_sequence[1].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
            REQUIRE(chord_sequence[1].m_bar_number == 0);
        }
        SECTION("Slash and normal")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_and_normal.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::Eb, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Cj7/E");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIj7");
            REQUIRE(chord_sequence[1].m_chord == "Eb-6");
            REQUIRE(chord_sequence[1].m_chord_degree == "I-6");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
            REQUIRE(chord_sequence[1].m_bar_number == 0);
        }
        SECTION("Four normal")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "four_normal.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::D, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Dbm7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[1].m_chord == "E-");
            REQUIRE(chord_sequence[1].m_chord_degree == "II-7");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7");
            REQUIRE(chord_sequence[2].m_chord_degree == "Ij7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A79b");
            REQUIRE(chord_sequence[3].m_chord_degree == "V7alt");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("Four Slash")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "four_slash.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "C/E");
            REQUIRE(chord_sequence[1].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7/G");
            REQUIRE(chord_sequence[2].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A/D");
            REQUIRE(chord_sequence[3].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("Two slash two normal")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_slash_two_normal.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "C/E");
            REQUIRE(chord_sequence[1].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7");
            REQUIRE(chord_sequence[2].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A");
            REQUIRE(chord_sequence[3].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("Two normal two slash")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_normal_two_slash.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "C");
            REQUIRE(chord_sequence[1].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7/G");
            REQUIRE(chord_sequence[2].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A/D");
            REQUIRE(chord_sequence[3].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("Slash normal alternate")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "slash_normal_alternate.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Eb6/D");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "C");
            REQUIRE(chord_sequence[1].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7/G");
            REQUIRE(chord_sequence[2].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A");
            REQUIRE(chord_sequence[3].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("Normal slash alternate")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "normal_slash_alternate.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 4);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "C/E");
            REQUIRE(chord_sequence[1].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "Dj7");
            REQUIRE(chord_sequence[2].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
            REQUIRE(chord_sequence[3].m_chord == "A/D");
            REQUIRE(chord_sequence[3].m_chord_degree == "VImaj");
            REQUIRE(chord_sequence[3].m_chord_position == 1440);
        }
        SECTION("One beat, three beats")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "one_three.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "Dj7");
            REQUIRE(chord_sequence[1].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
        }
        SECTION("Three beats, one beat")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "three_one.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Eb/A");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIIImaj");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "Dj7");
            REQUIRE(chord_sequence[1].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[1].m_chord_position == 1440);
        }
        SECTION("One beat one beat two beats")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "one_one_two.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 3);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "Dj7");
            REQUIRE(chord_sequence[1].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
            REQUIRE(chord_sequence[2].m_chord == "C/E");
            REQUIRE(chord_sequence[2].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[2].m_chord_position == 960);
        }
        SECTION("Two beats one beat one beat")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "two_one_one.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 3);
            REQUIRE(chord_sequence[0].m_chord == "Eb6");
            REQUIRE(chord_sequence[0].m_chord_degree == "bIII6");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "Dj7");
            REQUIRE(chord_sequence[1].m_chord_degree == "IIj7");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
            REQUIRE(chord_sequence[2].m_chord == "C/E");
            REQUIRE(chord_sequence[2].m_chord_degree == "Imaj");
            REQUIRE(chord_sequence[2].m_chord_position == 1440);
        }
        SECTION("Two beats one beat one beat")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "too_many_beats_1.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::C, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 0);
        }
    }
    SECTION("Whole sequences")
    {
        // Again this could be more thourough, i can come back to if there are issues.
        // Especially should test some chord positions
        SECTION("Just friends")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "just_friends_sequence.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::four_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 36);
            REQUIRE(chord_sequence[16].m_chord == "B7");
            REQUIRE(chord_sequence[16].m_chord_degree == "bV7");
            REQUIRE(chord_sequence[16].m_bar_number == 15);
            REQUIRE(chord_sequence[16].m_chord_position == 29760);
            REQUIRE(chord_sequence[20].m_chord == "Eb7");
            REQUIRE(chord_sequence[20].m_chord_degree == "bVII7");
            REQUIRE(chord_sequence[20].m_bar_number == 19);
            REQUIRE(chord_sequence[20].m_chord_position == 36480);
            REQUIRE(chord_sequence[34].m_chord == "C-7");
            REQUIRE(chord_sequence[34].m_chord_degree == "V-7");
            REQUIRE(chord_sequence[34].m_bar_number == 31);
            REQUIRE(chord_sequence[34].m_chord_position == 59520);
        }
    }
    // These tests with irregular time signatures aren't throughough, however im running out of time!!
    SECTION("6/4")
    {
        SECTION("One chord per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "six_four_one_chord.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::six_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
        }

        SECTION("Two chords per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "six_four_two_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::six_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "A7alt");
            REQUIRE(chord_sequence[1].m_chord_degree == "III7alt");
            REQUIRE(chord_sequence[1].m_chord_position == 1440);
        }
    }
    SECTION("5/4")
    {
        SECTION("One chord per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "five_four_one_chord.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::five_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
        }

        SECTION("Two chords per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "five_four_two_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::five_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "A7alt");
            REQUIRE(chord_sequence[1].m_chord_degree == "III7alt");
            REQUIRE(chord_sequence[1].m_chord_position == 960);
        }
    }
    SECTION("3/4")
    {
        SECTION("One chord per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "three_four_one_chord.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::three_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
        }

        SECTION("Two chords per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "three_four_two_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::three_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "A7alt");
            REQUIRE(chord_sequence[1].m_chord_degree == "III7alt");
            REQUIRE(chord_sequence[1].m_chord_position == 480);
        }
    }
    SECTION("9/4")
    {
        SECTION("One chord per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "nine_four_one_chord.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::nine_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 1);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
        }

        SECTION("Two chords per bar")
        {
            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "nine_four_two_chords.txt");
            std::string chord_sequence_string = SequenceUtility::getChordSequenceAsString(test_file);
            ChordSequence chord_sequence;
            ChordParsingUtility::parseChordSequence(chord_sequence_string, chord_sequence, TimeSignature::nine_four, RootNote::F, test_file.getFileName());
            REQUIRE(chord_sequence.size() == 2);
            REQUIRE(chord_sequence[0].m_chord == "Em7b5");
            REQUIRE(chord_sequence[0].m_chord_degree == "VIIm7b5");
            REQUIRE(chord_sequence[0].m_chord_position == 0);
            REQUIRE(chord_sequence[1].m_chord == "A7alt");
            REQUIRE(chord_sequence[1].m_chord_degree == "III7alt");
            REQUIRE(chord_sequence[1].m_chord_position == 1920);
        }
    }
}
