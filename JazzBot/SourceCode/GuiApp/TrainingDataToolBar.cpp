//
//  TrainingDataToolBar.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataToolBar.hpp"

static const char* return_to_menu_text = "Main Menu";

TrainingDataToolBar::TrainingDataToolBar(Listener* listener)
    : m_listener(listener)
{
    return_to_menu_button.addListener(this);
    return_to_menu_button.setBounds(0, 0, 80, 45);
    return_to_menu_button.setButtonText(return_to_menu_text);
    addAndMakeVisible(return_to_menu_button);
}

void
TrainingDataToolBar::buttonClicked(juce::Button* button)
{
    if (button == &return_to_menu_button)
    {
        m_listener->returnToMainMenu();
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
