//
//  GenerateSequenceView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/06/2022.
//

#include "GenerateSequenceView.hpp"
#include "../Utility/ChordParsingUtility.hpp"
#include "../Utility/MidiFileUtility.hpp"
#include "../Utility/SequenceUtility.hpp"
#include "../NoteSequence/EighthNoteSequence.hpp"

static const int training_data_tool_bar_height = 50;
static const char* return_to_menu_text = "Main Menu";
static const char* load_chord_sequence = "Load Chord Sequence";
static const char* generate_melody_text = "Generate Melody";

// no idea why this wont work
//static const std::string custom_chord_changes = CUSTOM_CHORD_CHANGES_DIRECTORY;
static const std::string custom_chord_sequence_dir = "/Users/tom-harvey/dev/JazzBot/JazzBot/CustomChordChanges/ChordSequence/";
static const std::string custom_song_infromation_dir = "/Users/tom-harvey/dev/JazzBot/JazzBot/CustomChordChanges/SongInformation/";

static const std::string default_chord_sequence_file = "2_5_1_6_chord_changes.txt";
static const std::string default_song_information_file = "2_5_1_6_song_information.txt";

static const std::string just_friends_chord_sequence_file = "ChetBaker_JustFriends_chord_changes.txt";
static const std::string just_friends_song_information_file = "ChetBaker_JustFriends_song_information.txt";

static const std::string giant_steps_chord_sequence_file = "JohnColtrane_GiantSteps-1_chord_changes.txt";
static const std::string giant_steps_song_information_file = "JohnColtrane_GiantSteps-1_song_information.txt";

static const std::size_t default_number_of_choruses = 8;

GenerateSequenceView::GenerateSequenceView(Listener* listener)
    : m_midi_sequence(this)
    , m_chord_sequence_file(custom_chord_sequence_dir + giant_steps_chord_sequence_file)
    , m_song_information_file(custom_song_infromation_dir + giant_steps_song_information_file)
    , m_listener(listener)
{
    addAndMakeVisible(&m_midi_sequence);

    m_return_to_menu_button.addListener(this);
    m_return_to_menu_button.setBounds(0, 0, 80, 45); // constants when i have the spacing sorted
    m_return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(m_return_to_menu_button);
    
    m_select_chord_sequence_button.addListener(this);
    m_select_chord_sequence_button.setBounds(160, 0, 80, 45);
    m_select_chord_sequence_button.setButtonText(load_chord_sequence);
    addAndMakeVisible(m_select_chord_sequence_button);
    
    m_generate_melody_button.addListener(this);
    m_generate_melody_button.setBounds(320, 0, 80, 45);
    m_generate_melody_button.setButtonText(generate_melody_text);
    addAndMakeVisible(m_generate_melody_button);
    
    // set the chord sequence here ( for now, will be got from the button in the future)
    // set the SongInformation here (for now)
    
    // Put in function maybe...
    // Check if both files exist
    SongInformation song_information;
    ChordSequence chord_sequence;
    if (SequenceUtility::parseSongInformation(m_song_information_file, song_information))
    {
        if (ChordParsingUtility::parseChordSequence(m_chord_sequence_file,
                                                    chord_sequence,
                                                    song_information.m_time_signature,
                                                    song_information.m_key))
        {
            m_song_information = song_information;
            m_chord_sequence = chord_sequence;
        }
    }
    
    
    SwingRatio swing_ratio = std::make_pair(320, 160); // instead of fixed ratio values it would be cool to have them vary (so that would instead pass in something else
    EighthNoteGroupingRange grouping_range = std::make_pair(3, 8);
    
    m_note_sequence = std::make_unique<EighthNoteSequence>(m_chord_sequence,
                                                           m_song_information.m_time_signature,
                                                           swing_ratio,
                                                           grouping_range);
}

void
GenerateSequenceView::resized()
{
    m_midi_sequence.setBounds(0,
                              training_data_tool_bar_height,
                              getWidth(),
                              getHeight() - training_data_tool_bar_height);
}

void
GenerateSequenceView::buttonClicked(juce::Button* button)
{
    if (button == &m_return_to_menu_button)
    {
        m_listener->exitGenerateSequence();
    }
    else if (button == &m_select_chord_sequence_button)
    {
        // instead of a file chooser, i think it would be better to have a drop down menu with file names to choose from, either using songs or custom chord sequences
        // then that name will need to match for the song info and the chord sequence and from there it will load it in.
        // the code in the constructor can be wrapped into a function
        
        // remove previous ptr
        // make note_sequence point to a new sequence.
    }
    else if (button == &m_generate_melody_button)
    {
        if (!m_chord_sequence.empty())
        {
            // these two can be passed in from gui
            
            MidiSequence midi_sequence = m_note_sequence->generateSequence(default_number_of_choruses);
            MidiFileUtility::writeMidiFile(midi_sequence);
        }
    }
    // TODO: add a drop down menu to select the type of algorithm, it will change m_note_grouping
    // Also add a transpose button to change whatever the key is
}

void
GenerateSequenceView::updateViewPort(const MidiSequenceView* const changed_view, const int& x_posistion, const int& y_posistion)
{
//    if (changed_view == &m_original_sequence)
//    {
//        m_modified_sequence.updateViewPort(x_posistion, y_posistion);
//    }
//    else if (changed_view == &m_modified_sequence)
//    {
//        m_original_sequence.updateViewPort(x_posistion, y_posistion);
//    }
}

void
GenerateSequenceView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::black); // menu colour tbd
}
