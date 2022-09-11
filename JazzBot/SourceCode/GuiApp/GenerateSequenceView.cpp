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
#include "../Utility/SongInformationUtility.hpp"

static const int training_data_tool_bar_height = 50;
static const char* return_to_menu_text = "Main Menu";
static const char* load_chord_sequence = "Load Chord Sequence";
static const char* generate_melody_text = "Generate Melody";

static const std::string default_chord_sequence_file = "/ChetBaker_JustFriends_chord_changes.txt";
static const std::string default_song_information_file = "/ChetBaker_JustFriends_song_information.txt";

static const int button_width = 120;
static const int button_height = 45;
static const int select_chord_sequence_x_position = 160;
static const int generate_melody_x_position = 320;
static const int current_song_label_x_position = 480;
static const int current_song_label_width = 500;

static const std::size_t default_number_of_choruses = 8;
static const double swing_ratio_first = 320;
static const double swing_ratio_second = 160;
static const int grouping_range_lower = 3;
static const int grouping_range_upper = 8;

static const std::string chord_changes_directory_str = CUSTOM_CHORD_CHANGES_DIRECTORY;
static const std::string song_information_directory_str = CUSTOM_SONG_INFORMATION_DIRECTORY;
static const juce::File chord_changes_directory(chord_changes_directory_str);

static const juce::String song_information_file_ending = "_song_information.txt";

static const std::string please_load_file_str = "Please load a song file";
static const std::string chord_changes_suffix = "_chord_changes.txt";

static const juce::String output_directory(juce::String(PROJECT_DIRECTORY) + "/GeneratedSequences"); // TODO: updated when more types of sequence are added
//static const juce::String output_directory =+ "/GeneratedSequences"

GenerateSequenceView::GenerateSequenceView(Listener* listener)
    : m_midi_sequence(this)
    , m_chord_sequence_file(chord_changes_directory_str + default_chord_sequence_file)
    , m_song_information_file(song_information_directory_str + default_song_information_file)
    , m_listener(listener)
{
    addAndMakeVisible(&m_midi_sequence);

    m_return_to_menu_button.addListener(this);
    m_return_to_menu_button.setBounds(0, 0, button_width, button_height);
    m_return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(m_return_to_menu_button);
    
    m_select_chord_sequence_button.addListener(this);
    m_select_chord_sequence_button.setBounds(select_chord_sequence_x_position, 0, button_width, button_height);
    m_select_chord_sequence_button.setButtonText(load_chord_sequence);
    addAndMakeVisible(m_select_chord_sequence_button);
    
    m_generate_melody_button.addListener(this);
    m_generate_melody_button.setBounds(generate_melody_x_position, 0, button_width, button_height);
    m_generate_melody_button.setButtonText(generate_melody_text);
    addAndMakeVisible(m_generate_melody_button);
    
    m_current_song_label.setBounds(current_song_label_x_position, 0, current_song_label_width, button_height);
    addAndMakeVisible(m_current_song_label);
    
    m_swing_ratio = std::make_pair(swing_ratio_first, swing_ratio_second); // instead of fixed ratio values it would be cool to have them vary (so that would instead pass in something else
    m_grouping_range = std::make_pair(grouping_range_lower, grouping_range_upper);
    parseDataFiles();
}

void
GenerateSequenceView::parseDataFiles()
{
    SongInformation song_information;
    ChordSequence chord_sequence;
    bool success = false;
    if (SongInformationUtility::parseSongInformation(m_song_information_file, song_information))
    {
        if (ChordParsingUtility::parseChordSequence(m_chord_sequence_file,
                                                    chord_sequence,
                                                    song_information.m_time_signature,
                                                    song_information.m_key))
        {
            m_song_information = song_information;
            m_chord_sequence = chord_sequence;
            setCurrentSongLabel(m_song_information_file.getFileName());
            // should this be reset?
            m_note_sequence = std::make_unique<EighthNoteSequence>(m_chord_sequence,
                                                                   m_song_information.m_time_signature,
                                                                   m_swing_ratio,
                                                                   m_grouping_range);
            success = true;
        }
    }
    if (!success)
    {
        setCurrentSongLabel(please_load_file_str);
        m_note_sequence.reset(); // or do i want release?
    }
}

void
GenerateSequenceView::setCurrentSongLabel(const juce::String& current_song_text)
{
    // TODO: Remove underscores and add spaces between capital letters to improve gui
    m_current_song_label.setText(current_song_text.replaceFirstOccurrenceOf(song_information_file_ending, ""), juce::NotificationType::dontSendNotification);
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
        juce::String chord_sequence_song_name = m_chord_sequence_file.getFileName().upToFirstOccurrenceOf(chord_changes_suffix, false, false);
        
        juce::FileChooser chooser{"Please load a File", chord_changes_directory};
        if (chooser.browseForFileToOpen())
        {
            auto chord_sequence_file = chooser.getResult();
            m_chord_sequence_file = chord_sequence_file;
            juce::String new_chord_sequence_song_name = m_chord_sequence_file.getFileName().upToFirstOccurrenceOf(chord_changes_suffix, false, false);
            m_song_information_file = juce::File(m_song_information_file.getFullPathName().replaceFirstOccurrenceOf(chord_sequence_song_name, new_chord_sequence_song_name));
            parseDataFiles();
        }
    }
    else if (button == &m_generate_melody_button)
    {
        if (m_note_sequence)
        {
            // these two can be passed in from gui
            MidiSequence midi_sequence = m_note_sequence->generateSequence(default_number_of_choruses);
            // check if midi sequence empty - then display something in gui.
            MidiFileUtility::writeMidiFile(midi_sequence,
                                           output_directory,
                                           m_chord_sequence_file.getFileName().upToFirstOccurrenceOf(chord_changes_suffix, false, false));
        }
    }
    // TODO: add a drop down menu to select the type of algorithm, it will change m_note_grouping
    // Also add a transpose button to change whatever the key is
}

void
GenerateSequenceView::updateViewPort(const MidiSequenceView* const changed_view, const int& x_posistion, const int& y_posistion)
{
    juce::ignoreUnused(changed_view,x_posistion, y_posistion);
}

void
GenerateSequenceView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::black); // menu colour tbd
}
