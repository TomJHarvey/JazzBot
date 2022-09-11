//
//  TrainingDataView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataView.hpp"
#include "../NoteGrouping/EighthNotes.hpp"
#include "../Utility/DatabaseUtility.hpp"
#include "../Utility/SequenceUtility.hpp"

static const int training_data_tool_bar_height = 50;
static const int midi_sequence_view_height = 450;

static const char* return_to_menu_text = "Main Menu";
static const char* load_file_text = "Load file";
static const char* view_algorithm_text = "View algorithm"; // this shows the algorithm at work, for example it will show just eigth notes
static const char* generate_sequence_data_text = "Generate Sequence Data"; // this will apply the algorithm to all files and generate data to be used for sequence generation

static const int button_height = 45;
static const int button_width = 120;
static const int load_file_x_position = 160;
static const int view_algorithm_x_position = 320;
static const int generate_sequence_data_x_position = 480;

TrainingDataView::TrainingDataView(Listener* listener)
    : m_original_sequence(this)
    , m_modified_sequence(this)
    , m_note_grouping(new EighthNotes())
    , m_listener(listener)
    , m_midi_files_directory(PROJECT_DIRECTORY)
{
    
    m_midi_files_directory =  m_midi_files_directory.replace("JazzBot/JazzBot", "JazzBot/midi_files"); // fix relative path bug in file chooser.

    addAndMakeVisible(&m_original_sequence);
    addAndMakeVisible(&m_modified_sequence);
    
    // Different buttons will be shown depending on what mode it is, some will always be there. Like these ones here
    m_return_to_menu_button.addListener(this);
    m_return_to_menu_button.setBounds(0, 0, button_width, button_height); // constants when i have the spacing sorted
    m_return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(m_return_to_menu_button);
    
    m_load_file_button.addListener(this);
    m_load_file_button.setBounds(load_file_x_position, 0, button_width, button_height);
    m_load_file_button.setButtonText(load_file_text);
    addAndMakeVisible(m_load_file_button);
    
    view_algorithm_button.addListener(this);
    view_algorithm_button.setBounds(view_algorithm_x_position, 0, button_width, button_height);
    view_algorithm_button.setButtonText(view_algorithm_text);
    addAndMakeVisible(view_algorithm_button);
    
    generate_sequence_data_button.addListener(this);
    generate_sequence_data_button.setBounds(generate_sequence_data_x_position, 0, button_width, button_height);
    generate_sequence_data_button.setButtonText(generate_sequence_data_text);
    addAndMakeVisible(generate_sequence_data_button);
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
        juce::FileChooser chooser{"", m_midi_files_directory};
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
            MidiSequence note_groupings = m_note_grouping->getModifiedSequence(modified_sequence);
            
            // this sets it now i can use it in the midi view
            m_modified_sequence.setSequence(note_groupings);
            
            // then it can display the sequence as it is
            m_modified_sequence.displaySequence();
        }
    }
    else if (button == &generate_sequence_data_button)
    {
        if (m_sequences.empty())
        {
            m_sequences = SequenceUtility::generateAllSequenceObjects();
        }
        
        // its eight note string for now. That can be stored in noteGrouping as get database name.
        std::string database_location;
        if (!DatabaseUtility::databaseExists(m_note_grouping->getDatabaseName(), m_note_grouping->getDatabaseDirectory(), database_location))
        {
            std::vector<std::string> sql_insert_statements;
            m_note_grouping->getSQLInsertStatements(m_sequences, sql_insert_statements);
            if (!sql_insert_statements.empty())
            {                
                // create the database by passing in the database filename, the database directory and the sql for creating the database
                if (DatabaseUtility::createDatabase(m_note_grouping->getDatabaseName(), m_note_grouping->getDatabaseDirectory(), m_note_grouping->getDatabaseCreationSQL()))
                {
                    // create the database by passing in the database filename, the database directory and the sql for inserting the note grouping data into the datbase
                    DatabaseUtility::populateDatabase(m_note_grouping->getDatabaseName(), m_note_grouping->getDatabaseDirectory(), sql_insert_statements);
                }
            }
        }
        else
        {
            // could have a pop up window here,
            std::cout << "Database already exists at " << database_location << std::endl;
        }
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
