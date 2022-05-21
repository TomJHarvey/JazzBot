//
//  TrainingDataView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataView.hpp"
#include "../SequenceUtility.hpp"

static const int training_data_tool_bar_height = 50;
static const int midi_sequence_view_height = 450;

static const char* return_to_menu_text = "Main Menu";
static const char* load_file_text = "Load file";
static const char* modified_sequence_text = "View modified Sequence"; // rename
static const char* generate_sequence_text = "Generate Sequence"; // rename

static const std::string midi_files_directory = MIDI_FILES_DIRECTORY;
// static const std::string midi_file_extension = "mid";

static const juce::File default_file_path(midi_files_directory);


TrainingDataView::TrainingDataView(Listener* listener)
    : m_original_sequence(this)
    , m_modified_sequence(this)
    , m_listener(listener)
{
    addAndMakeVisible(&m_original_sequence);
    addAndMakeVisible(&m_modified_sequence);
    
    // Different buttons will be shown depending on what mode it is, some will always be there. Like these ones here
    m_return_to_menu_button.addListener(this);
    m_return_to_menu_button.setBounds(0, 0, 80, 45); // constants when i have the spacing sorted
    m_return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(m_return_to_menu_button);
    
    m_load_file_button.addListener(this);
    m_load_file_button.setBounds(160, 0, 80, 45);
    m_load_file_button.setButtonText(load_file_text);
    addAndMakeVisible(m_load_file_button);
    
    display_modified_sequence_button.addListener(this);
    display_modified_sequence_button.setBounds(320, 0, 80, 45);
    display_modified_sequence_button.setButtonText(modified_sequence_text);
    addAndMakeVisible(display_modified_sequence_button);
    
    generate_sequence_button.addListener(this);
    generate_sequence_button.setBounds(480, 0, 80, 45);
    generate_sequence_button.setButtonText(generate_sequence_text);
    addAndMakeVisible(generate_sequence_button);
}

void
TrainingDataView::resized()
{
    m_original_sequence.setBounds(0,
                                  training_data_tool_bar_height,
                                  getWidth(),
                                  midi_sequence_view_height);
    m_modified_sequence.setBounds(0,
                                  training_data_tool_bar_height + midi_sequence_view_height,
                                  getWidth(),
                                  midi_sequence_view_height);
}

void
TrainingDataView::buttonClicked(juce::Button* button)
{
    if (button == &m_return_to_menu_button)
    {
        m_listener->returnToMainMenu();
    }
    else if (button == &m_load_file_button)
    {
        juce::FileChooser chooser{"Please load a File", default_file_path};
        if (chooser.browseForFileToOpen())
        {
            auto midi_file = chooser.getResult();
            // debug output if not a midifile
            m_original_sequence.loadSequence(midi_file);
        }
    }
    else if (button == &display_modified_sequence_button)
    {
        MidiSequence modified_sequence;
        if (m_original_sequence.getCurrentSequence(modified_sequence))
        {
            MidiSequence eighth_note_groupings = MidiFileUtility::getOnlyEighthNotes(modified_sequence);
            modified_sequence = MidiFileUtility::getOnlyEighthNotes(eighth_note_groupings);
            
            // this sets it now i can use it in the midi view
            m_modified_sequence.setSequence(modified_sequence);
            
            // then it can display the sequence as it is
            m_modified_sequence.displaySequence();
        }
    }
    else if (button == &generate_sequence_button)
    {
        std::vector<Sequence> sequence = SequenceUtility::generateAllSequenceObjects();
        MidiFileUtility::calculateEighthNoteGroupingKeys(sequence);
        
    }
    
    // m_sequences = generateAllSequenceObjects(); // can be reused for other functions
    // generate all eigth note groupings for each Sequence; - this will save each one to a db
}

void
TrainingDataView::updateViewPort(const MidiSequenceView* const changed_view, const int& x_posistion, const int& y_posistion)
{
    if (changed_view == &m_original_sequence)
    {
        m_modified_sequence.updateViewPort(x_posistion, y_posistion);
    }
    else if (changed_view == &m_modified_sequence)
    {
        m_original_sequence.updateViewPort(x_posistion, y_posistion);
    }
}

void
TrainingDataView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::black); // menu colour tbd
}
