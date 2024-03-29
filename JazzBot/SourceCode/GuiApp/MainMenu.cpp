//
//  MainMenu.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "MainMenu.hpp"
#include <string>

static const char* generate_sequence_data_text = "Generate Sequence Data";
static const char* generate_sequence_text = "Generate Sequence";
static const std::string menu_items_text[menu_items_size] = {generate_sequence_data_text,
                                         generate_sequence_text};
static const int button_y_pos = 60;
static const int button_y_spacing = 185;
static const int button_x_pos = 30;
static const int button_height = 120;
static const int menu_width = 350;
static const int menu_height = 650;

MainMenu::MainMenu(Listener* menu_listener)
    : m_main_menu_listener(menu_listener)
{
    setSize (menu_width, menu_height);

    for (std::size_t index = 0; index < menu_items_size; index++)
    {
        m_menu_items[index].setButtonText(menu_items_text[index]);
        addAndMakeVisible(m_menu_items[index]);
        m_menu_items[index].addListener(this);
    }
}

void
MainMenu::buttonClicked(juce::Button* button)
{
    for (std::size_t index = 0; index < menu_items_size; index ++)
    {
        if (button == &m_menu_items[index])
        {
            m_main_menu_listener->menuItemSelected(static_cast<MenuItem>(index));
        }
    }
}

void
MainMenu::resized()
{
}

void
MainMenu::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::purple); // menu colour tbd
    int button_pos = button_y_pos;
    for (std::size_t index = 0; index < menu_items_size; index ++)
    {
        m_menu_items[index].setBounds(button_x_pos,
                                      button_pos,
                                      getWidth() - (button_x_pos * 2),
                                      button_height);
        button_pos += button_y_spacing;
    }
}
