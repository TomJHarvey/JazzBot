//
//  CustomViewport.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 01/05/2022.
//

#include "CustomViewport.hpp"

CustomViewport::CustomViewport(const juce::String& componentName)
    : juce::Viewport(componentName)
{
    
}

void
CustomViewport::visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea)
{
    listeners.call(&Listener::viewportAreaChanged, this, newVisibleArea);
}

void
CustomViewport::addListener(Listener* listener)
{
    listeners.add(listener);
}

void
CustomViewport::removeListener(Listener* listener)
{
    listeners.remove(listener);
}
