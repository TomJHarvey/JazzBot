//
//  MidiFileToolBar.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#ifndef MidiFileToolBar_hpp
#define MidiFileToolBar_hpp

#include <stdio.h>
#include <juce_gui_extra/juce_gui_extra.h>

class MidiFileToolBar : public juce::Component
{
public:
    MidiFileToolBar() = default; // yet to implement 
    void paint(juce::Graphics& g) override;
    void resized() override;
};

#endif /* MidiFileToolBar_hpp */
