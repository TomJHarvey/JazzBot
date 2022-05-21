//
//  TrainingDataView.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#ifndef TrainingDataView_hpp
#define TrainingDataView_hpp

#include <stdio.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "MidiSequenceView.hpp"

class TrainingDataView : public juce::Component
                       , public MidiSequenceView::Listener
                       , public juce::Button::Listener
{
public:
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void returnToMainMenu() = 0;
    };
    
    TrainingDataView(Listener* listener);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void updateViewPort(const MidiSequenceView* const changed_view, const int& x_posistion, const int& y_posistion) override;
    void buttonClicked(juce::Button* button) override;

private:
    MidiSequenceView m_original_sequence;
    MidiSequenceView m_modified_sequence;
    Listener* m_listener;
    juce::TextButton m_return_to_menu_button;
    juce::TextButton m_load_file_button;
    juce::TextButton display_modified_sequence_button;
    juce::TextButton generate_sequence_button;

};

#endif /* TrainingDataView_hpp */
