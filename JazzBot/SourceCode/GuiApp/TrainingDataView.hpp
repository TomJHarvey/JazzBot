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
#include "TrainingDataToolBar.hpp"

class TrainingDataView : public juce::Component
{
public:
    TrainingDataView(TrainingDataToolBar::Listener* m_toolbar_listener);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    TrainingDataToolBar m_toolbar;
};

#endif /* TrainingDataView_hpp */
