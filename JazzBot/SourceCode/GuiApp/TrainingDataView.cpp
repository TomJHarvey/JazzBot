//
//  TrainingDataView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataView.hpp"
#include "../NoteGrouping/EighthNotes.hpp"
#include "../NoteGrouping/GroupingsDatabase.hpp"
#include "../Utility/SequenceUtility.hpp"

static const int training_data_tool_bar_height = 50;
static const int midi_sequence_view_height = 450;

static const char* return_to_menu_text = "Main Menu";
static const char* load_file_text = "Load file";
static const char* view_algorithm_text = "View algorithm"; // this shows the algorithm at work, for example it will show just eigth notes
static const char* apply_algorithm_text = "Apply algorithm"; // this will apply the algorithm to all files and generate data to be used for sequence generation

static const char* eighth_note_groupings_db_string = "eighth_note_groupings.db";

static const std::string midi_files_directory = MIDI_FILES_DIRECTORY;
// static const std::string midi_file_extension = "mid";

static const juce::File default_file_path(midi_files_directory);

TrainingDataView::TrainingDataView(Listener* listener)
    : m_original_sequence(this)
    , m_modified_sequence(this)
    , m_note_grouping(new EighthNotes())
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
    
    view_algorithm_button.addListener(this);
    view_algorithm_button.setBounds(320, 0, 80, 45);
    view_algorithm_button.setButtonText(view_algorithm_text);
    addAndMakeVisible(view_algorithm_button);
    
    apply_algorithm_button.addListener(this);
    apply_algorithm_button.setBounds(480, 0, 80, 45);
    apply_algorithm_button.setButtonText(apply_algorithm_text);
    addAndMakeVisible(apply_algorithm_button);
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
    else if (button == &view_algorithm_button)
    {
        MidiSequence modified_sequence;
        if (m_original_sequence.getCurrentSequence(modified_sequence))
        {
            MidiSequence eighth_note_groupings = m_note_grouping->getModifiedSequence(modified_sequence);
            
            // this sets it now i can use it in the midi view
            m_modified_sequence.setSequence(eighth_note_groupings);
            
            // then it can display the sequence as it is
            m_modified_sequence.displaySequence();
        }
    }
    else if (button == &apply_algorithm_button)
    {
        if (m_sequences.empty())
        {
            m_sequences = SequenceUtility::generateAllSequenceObjects();
        }
        
        // its eight note string for now. That can be stored in noteGrouping as get database name.
//        if (!GroupingsDatabase::databaseExists(eighth_note_groupings_db_string))
//        {
            NoteGroupingData data = m_note_grouping->createDatabaseKeys(m_sequences);
            if (GroupingsDatabase::createDatabase(eighth_note_groupings_db_string))
            {
                GroupingsDatabase::populateDatabase(eighth_note_groupings_db_string, data);
            }
//        }
//        else
//        {
//            // could have a pop up window here,
//            std::cout << "Database " << eighth_note_groupings_db_string << " already exists" << std::endl;
//        }
        
        //std::cout << data.size() << std::endl;
    }
    // TODO: add a drop down menu to select the type of algorithm, it will change m_note_grouping
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
