//
//  TrainingDataToolBar.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#ifndef TrainingDataToolBar_hpp
#define TrainingDataToolBar_hpp

#include <stdio.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "MidiSequenceView.hpp"


class TrainingDataToolBar : public juce::Component
                          , public juce::Button::Listener
{
public:
    class Listener
    {
    public:
        virtual void returnToMainMenu() = 0;
    };
    
public:
    TrainingDataToolBar(Listener* listener,
                        MidiSequenceView* original_sequence,
                        MidiSequenceView* modified_sequence);
    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
private:
    Listener* m_listener;
    juce::TextButton m_return_to_menu_button;
    juce::TextButton m_load_file_button;
    MidiSequenceView& m_original_sequence;
    MidiSequenceView& m_modified_sequence;
};

#endif /* TrainingDataToolBar_hpp */
