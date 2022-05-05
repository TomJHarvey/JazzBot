#include <catch2/catch_test_macros.hpp>
//#include "TestFiles"
#include "../SourceCode/MidiFileUtility.hpp"

static juce::String test_file_dir = "../../Tests/TestFiles/";


// TODO: redo these test cases, might be better to a have a form of error handling thats more verbose.
// returning false could happen for a number of reasons, it would be better to check its false for the right reason.

TEST_CASE("MidiFileGeneratorTests", "[midi_file_generator_tests]")
{
    
    bool bl = true;
    REQUIRE(bl);
    
    MidiSequence midi_sequence;
    MidiSequence output_sequence;
    juce::File test_file;
    
    // Evenly spaced swung 8th notes
    SECTION("Test 8 eigth notes")
    {
        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes.mid");
        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, true));
        
        std::cout << midi_sequence.size() << std::endl;
//        for (auto& notes : midi_sequence)
//        {
//            std::cout << notes.note_on << " : " << notes.note_value << std::endl;
//        }
        output_sequence = MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
        //REQUIRE(output_sequence.size() == 8);
        std::cout << midi_sequence.size() << std::endl;
//        for (auto& notes : output_sequence)
//        {
//            std::cout << notes.note_on << " : " << notes.note_value << std::endl;
//        }
        output_sequence.clear();
    }
        // Evenly spaced swung 8th notes
//        SECTION("Test 8 quarter notes")
//        {
//            test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "quarter_note_test.mid");
//            REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence, true));
//
//            std::cout << midi_sequence.size() << std::endl;
//
//            for (auto& notes : midi_sequence)
//            {
//                std::cout << notes.note_on << " : " << notes.note_value << std::endl;
//
//            }
//
//            output_sequence = MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
//            REQUIRE(output_sequence.size() == 8);
//            output_sequence.clear();
//        }
    
//        
//    SECTION("Test 8 eighth notes with offset")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_offset.mid");
//        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence));
//        MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
//        REQUIRE(midi_sequence.size() == 8);
//    }
//    
//    SECTION("Test 3 eighth notes followed by a quarter note and 2 eigth notes")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "eight_notes_1_quarter.mid");
//        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence));
//
//        output_sequence = MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
//        REQUIRE(output_sequence.size() == 5);
//        REQUIRE(output_sequence[3].note_value == 72);
//        output_sequence.clear();
//    }
    
//    SECTION("Test 4 eighth notes followed by a sixteenth note triplet")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_1.mid");
//        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence));
//        MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
//        REQUIRE(midi_sequence.size() == 4);
//    }
    
//    SECTION("Test a sixteenth note triplet followed by 5 eighth notes")
//    {
//        test_file = juce::File::getCurrentWorkingDirectory().getChildFile(test_file_dir + "sixteenth_trip_1.mid");
//        REQUIRE(MidiFileUtility::parseMidiFile(test_file, midi_sequence));
//        MidiFileUtility::getOnlyEigthNoteGroupings(midi_sequence);
//        REQUIRE(midi_sequence.size() == 5);
//    }
    
    

    // Evenly spaced 8th notes with offset
}


