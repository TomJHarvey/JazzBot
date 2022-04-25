//
//  PianoRoll.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#ifndef PianoRoll_hpp
#define PianoRoll_hpp

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdio.h>

static const int number_of_piano_keys = 88;
static const int grid_line_height = 15;

class PianoRoll : public juce::Component
{
public:
    PianoRoll();
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    juce::Path m_piano_grid_horizontal_lines[number_of_piano_keys];
    int m_piano_roll_width; // might be resizeable.
};



#endif /* PianoRoll_hpp */
