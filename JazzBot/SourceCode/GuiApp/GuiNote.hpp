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

/**
    @brief A class used to display a gui note on the piano roll. It currently is just a juce::Component that has a colour. This class will
           potentially be extended when midi playback is implemented.
 */
class GuiNote: public juce::Component
{
public:
    /**
        @brief Constructor
     */
    GuiNote(juce::Colour colour = juce::Colours::orange);
    
    /**
        @brief Constructor used to set the colour of this GuiNote withanother GuiNote
        @param obj GuiNote to copy the colour from
     */
    GuiNote( const GuiNote &obj){this->m_colour = obj.m_colour;} // is this needed?
    
    /** Overrides juce::Component */
    void paint(juce::Graphics& g) override;
private:
    
    /** The colour of the GuiNote*/
    juce::Colour m_colour;
};

#endif /* GuiNote_hpp */
