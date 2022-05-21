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
#include "CustomViewport.hpp"
#include "PianoRoll.hpp"
#include "../MidiSequenceUtility.hpp"

class MidiSequenceView : public juce::Component
                       , public PianoRoll::Listener
                       , public CustomViewport::Listener
{
public:
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void updateViewPort(const MidiSequenceView* const changed_view,
                                    const int& x_posistion,
                                    const int& y_posistion) = 0;
    };
    
    MidiSequenceView(Listener* listener);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void loadSequence(const juce::File& midi_file);
    void displaySequence();
    void setSequence(const MidiSequence& midi_sequence);
    void setBarLabels();
    bool getCurrentSequence(MidiSequence& midi_sequence) const;
    void resizeViewPort(const int& piano_roll_width) override;
    void updateBarLabels(const std::size_t& number_of_bars) override;
    void updateViewPort(const int& x_posistion, const int& y_posistion);
    void viewportAreaChanged(juce::Viewport* viewport, const juce::Rectangle<int>& newVisibleArea) override;

private:
    std::unique_ptr<PianoRoll> m_piano_roll;
    std::unique_ptr<CustomViewport> m_piano_view_port;
    int m_piano_roll_width;
    MidiSequence m_midi_sequence;
    juce::OwnedArray<juce::Label> m_bar_number_labels;
    std::size_t m_number_of_bars;
    Listener* m_listener;
};

#endif /* MidiSequenceView_hpp */
