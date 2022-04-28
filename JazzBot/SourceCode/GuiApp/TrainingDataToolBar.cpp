//
//  TrainingDataToolBar.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataToolBar.hpp"

static const char* return_to_menu_text = "Main Menu";
static const char* load_file_text = "Load file";
static const char* modified_sequence_text = "Modified Sequence"; // rename

static const std::string midi_files_directory = MIDI_FILES_DIRECTORY;
// static const std::string midi_file_extension = "mid";

static const juce::File default_file_path(midi_files_directory);

TrainingDataToolBar::TrainingDataToolBar(Listener* listener,
                                         MidiSequenceView* original_sequence,
                                         MidiSequenceView* modified_sequence)
    : m_listener(listener)
    , m_original_sequence(*original_sequence)
    , m_modified_sequence(*modified_sequence)
{
    m_return_to_menu_button.addListener(this);
    m_return_to_menu_button.setBounds(0, 0, 80, 45); // constants when i haev the spacing sorted
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
}

void
TrainingDataToolBar::buttonClicked(juce::Button* button)
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
            // output if not a midifile, needs a check
            m_original_sequence.loadSequence(midi_file);
        }
    }
    else if (button == &display_modified_sequence_button)
    {
        MidiSequence modified_sequence;
        if (m_original_sequence.getCurrentSequence(modified_sequence))
        {
            // call to a function that searches
            // this will apply the process highlightEigthNoteGroups()
            
//            std::cout << "first sequence " << std::endl;
//            for (auto& note : modified_sequence)
//            {
//                std::cout << "Note on: " << note.note_on << std::endl;
//                std::cout << "Note off: " << note.note_off << std::endl;
//                std::cout << "Note value: " << note.note_value << std::endl;
//            }
            
            modified_sequence = MidiFileUtility::getOnlyEigthNoteGroupings(modified_sequence);
            
//            std::cout << "Modified sequence " << std::endl;
//            for (auto& note : modified_sequence)
//            {
//                std::cout << "Note on: " << note.note_on << std::endl;
//                std::cout << "Note off: " << note.note_off << std::endl;
//                std::cout << "Note value: " << note.note_value << std::endl;
//            }
            
            // this sets it now i can use it in the midi view
            m_modified_sequence.setSequence(modified_sequence);
            
            // then it can display the sequence as it is
            m_modified_sequence.displaySequence();
        }
    }
}

void
TrainingDataToolBar::resized()
{
    // fixed size;
}

void
TrainingDataToolBar::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::blue); // menu colour tbd
}
