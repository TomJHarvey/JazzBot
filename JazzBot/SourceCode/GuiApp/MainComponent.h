#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>

#include "MainMenu.hpp"
#include "TrainingDataView.hpp"


#include "PianoRoll.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component
                    , public MainMenu::Listener
                    , public TrainingDataView::Listener
{
public:
    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;
    void menuItemSelected(const MenuItem& menu_selection) override;
    void returnToMainMenu() override;

private:
    MainMenu m_main_menu;
    TrainingDataView m_training_data_view;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
