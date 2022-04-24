//
//  MainMenu.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "MainMenu.hpp"

#include <string>

static const char* view_test_data_text = "View Test Data";
static const char* generate_sequence_text = "Generate Sequence";
static const char* view_sequence = "View Sequence";
std::string menu_items_text[menu_items_size] = {view_test_data_text,
                                                generate_sequence_text,
                                                view_sequence};
static const int button_y_pos = 60;
static const int button_y_spacing = 185;
static const int button_x_pos = 30;
static const int button_height = 120;

MainMenu::MainMenu(MainMenuListener* menu_listener)
    : m_main_menu_listener(menu_listener)
{
    setSize (350, 650);
    
    for (std::size_t index = 0; index < menu_items_size; index ++)
    {
        m_menu_items[index].setButtonText(menu_items_text[index]);
        addAndMakeVisible(m_menu_items[index]);
        m_menu_items[index].addListener(this);
    }
}

//MainMenu::~MainMenu()
//{
//    m_main_menu_listener = nullptr;
//}

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
    // fixed size;
}

void
MainMenu::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::green); // menu colour tbd
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
