//
//  GuiNote.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#ifndef GuiNote_hpp
#define GuiNote_hpp

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdio.h>

class GuiNote: public juce::Component
{
public:
    GuiNote(juce::Colour colour = juce::Colours::orange);
    GuiNote( const GuiNote &obj){this->m_colour = obj.m_colour;};
    void paint(juce::Graphics& g) override;
private:
    juce::Colour m_colour;
};

#endif /* GuiNote_hpp */
