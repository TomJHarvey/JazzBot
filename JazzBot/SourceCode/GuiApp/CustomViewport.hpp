//
//  CustomViewport.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 01/05/2022.
//

#ifndef CustomViewport_hpp
#define CustomViewport_hpp


#include <juce_gui_extra/juce_gui_extra.h>

class CustomViewport : public juce::Viewport
{
public:

    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void viewportAreaChanged(juce::Viewport* viewport, const juce::Rectangle<int>& newVisibleArea) = 0;
    };
    
    CustomViewport(const juce::String& componentName = juce::String());
    
    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override;
    void addListener(Listener* listener);
    void removeListener(Listener* listener);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomViewport)
    juce::ListenerList<Listener> listeners;
};

#include <stdio.h>

#endif /* CustomViewport_hpp */
