//
//  MidiSequenceView.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#ifndef MidiSequenceView_hpp
#define MidiSequenceView_hpp

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdio.h>
#include "MidiFileToolBar.hpp"
#include "PianoRoll.hpp"

class MidiSequenceView : public juce::Component
{
public:
    MidiSequenceView();
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    MidiFileToolBar m_tool_bar;
    PianoRoll m_piano_roll;
    juce::Viewport m_piano_view_port;
};

#endif /* MidiSequenceView_hpp */
