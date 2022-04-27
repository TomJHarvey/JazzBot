//
//  GuiNote.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 26/04/2022.
//

#include "GuiNote.hpp"

GuiNote::GuiNote(juce::Colour colour)
    : m_colour(colour)
{
}

void
GuiNote::paint(juce::Graphics & g)
{
    g.fillAll(m_colour); // menu colour tbd
}
