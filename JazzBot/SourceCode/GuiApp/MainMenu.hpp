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

static const std::size_t menu_items_size = 3;

enum class MenuItem
{
    view_test_data,
    generate_sequence,
    view_sequence
};

class MainMenu: public juce::Component,
                public juce::Button::Listener
{
public:
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void menuItemSelected(const MenuItem& menu_selection) = 0;
        
    };
    
    MainMenu(Listener* menu_listener);
    // virtual ~MainMenu();
    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
private:
    juce::TextButton m_menu_items[menu_items_size];
    Listener* m_main_menu_listener;
};




#endif /* MainMenu_hpp */
