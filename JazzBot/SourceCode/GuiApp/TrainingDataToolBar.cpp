//
//  TrainingDataToolBar.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataToolBar.hpp"

static const char* return_to_menu_text = "Main Menu";
static const char* load_file_text = "Load file";

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
    m_return_to_menu_button.setBounds(0, 0, 80, 45);
    m_return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(m_return_to_menu_button);
    
    m_load_file_button.addListener(this);
    m_load_file_button.setBounds(160, 0, 80, 45);
    m_load_file_button.setButtonText(load_file_text);
    addAndMakeVisible(m_load_file_button);
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
            auto file = chooser.getResult();
            //m_original_sequence
            
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
