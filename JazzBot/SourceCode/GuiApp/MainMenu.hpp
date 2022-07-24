//
//  MainMenu.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>
#include <juce_gui_extra/juce_gui_extra.h>

/** Contains the number of menu items*/
static const std::size_t menu_items_size = 3;

/** Contains the menu items indexes*/
enum class MenuItem
{
    view_test_data,
    generate_sequence,
    view_sequence
};

/**
    @brief This class defines the main menu view, which selects between the different areas of the app.
 */
class MainMenu: public juce::Component,
                public juce::Button::Listener
{
public:
    
    /**
        @brief A listener used when a menu item is selected
     */
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void menuItemSelected(const MenuItem& menu_selection) = 0;
    };
    
    /**
        @brief Constructor
     */
    MainMenu(Listener* menu_listener);

    /** Overrides juce::Component */
    void paint (juce::Graphics& g) override;
    
    /** Overrides juce::Component */
    void resized() override;
    
    /** Overrides juce::Button::Listener*/
    void buttonClicked(juce::Button* button) override;
    
private:
    
    /** Contains the buttons to select each menu item */
    juce::TextButton m_menu_items[menu_items_size];
    
    /** Listener to detect when a menu item is selected*/
    Listener* m_main_menu_listener;
};




#endif /* MainMenu_hpp */
