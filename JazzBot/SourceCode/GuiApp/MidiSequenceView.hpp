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
#include "PianoRoll.hpp"
#include "../MidiFileUtility.hpp"

class MidiSequenceView : public juce::Component
                       , public PianoRoll::Listener
{
public:
    MidiSequenceView();
    void paint(juce::Graphics& g) override;
    void resized() override;
    void loadSequence(const juce::File& midi_file);
    void displaySequence();
    void setSequence(const MidiSequence& midi_sequence);
    bool getCurrentSequence(MidiSequence& midi_sequence) const;
    void resizeViewPort(const int& piano_roll_width) override;
private:
    PianoRoll m_piano_roll;
    juce::Viewport m_piano_view_port;
    int m_piano_roll_width;
    MidiSequence m_midi_sequence;
};

#endif /* MidiSequenceView_hpp */
