//
//  CustomViewport.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 01/05/2022.
//

#ifndef CustomViewport_hpp
#define CustomViewport_hpp


#include <juce_gui_extra/juce_gui_extra.h>

/**
    @brief This class overrides the existing juce::Viewport class to serve as a way for classes to be able to observe when a viewports
           position has changed
 */
class CustomViewport : public juce::Viewport
{
public:

    /**
        @brief A listener for the CustomViewPort which will detect when the objects view position has changed
     */
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void viewportAreaChanged(juce::Viewport* viewport, const juce::Rectangle<int>& newVisibleArea) = 0;
    };
    
    /**
        @brief Constructor
     */
    CustomViewport(const juce::String& componentName = juce::String());
    
    /**
        @brief Adds a listener
        @param listener to listen to detect changes in the viewport area
     */
    void addListener(Listener* listener);
    
    /**
        @brief Adds a listener
        @param listener to remove
     */
    void removeListener(Listener* listener);
    
    /** Overrides juce::ViewPort */
    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomViewport)
    juce::ListenerList<Listener> listeners;
};

#include <stdio.h>

#endif /* CustomViewport_hpp */
