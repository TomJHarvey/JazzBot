#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>

#include "MainMenu.hpp"
#include "TrainingDataView.hpp"
#include "GenerateSequenceView.hpp"

#include "PianoRoll.hpp"

//==============================================================================
/*
    @brief This component lives inside the main window, and is used to show the training data view,
           the generate sequence view and the main menu for the app
*/
class MainComponent : public juce::Component
                    , public MainMenu::Listener
                    , public TrainingDataView::Listener
                    , public GenerateSequenceView::Listener
{
public:
    /**
        @brief Constructor
     */
    MainComponent();
    
    /** Overrides juce::Component */
    void paint (juce::Graphics&) override;
    
    /** Overrides juce::Component */
    void resized() override;
    
    /** Overrides MainMenu::Listener */
    void menuItemSelected(const MenuItem& menu_selection) override;
    
    /** TrainingDataView::Listener */
    void returnToMainMenu() override;
    
    /** GenerateSequenceView::Listener */
    void exitGenerateSequence() override; // same as above function, needs refactoring

private:
    /** The main menu to select each view  */
    MainMenu m_main_menu;
    
    /** Used to test out algorithms and save them to a database */
    TrainingDataView m_training_data_view;
    
    /** Used to  generate sequence files from generated database files from the training data view*/
    GenerateSequenceView m_generate_sequence_view;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
