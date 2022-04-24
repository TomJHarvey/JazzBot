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
    TrainingDataToolBar(Listener* listener);
    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
private:
    Listener* m_listener;
    juce::TextButton return_to_menu_button;
};

#endif /* TrainingDataToolBar_hpp */
