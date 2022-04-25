//
//  MidiSequenceView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#include "MidiSequenceView.hpp"

static const int tool_bar_height = 25;
static const int default_arrange_window_length = 3000;
static const int arrange_window_height = (number_of_piano_keys + 1) * grid_line_height;

MidiSequenceView::MidiSequenceView()
{
    addAndMakeVisible(m_tool_bar);
    m_tool_bar.setBounds(0, 0, getWidth(), tool_bar_height);
    addAndMakeVisible(m_piano_roll);
    m_piano_view_port.setViewedComponent(&m_piano_roll, false);
    m_piano_view_port.setScrollBarsShown(true,true);
    addAndMakeVisible(m_piano_view_port);
    
}
void
MidiSequenceView::resized()
{
    // set size of child components, is it needed here too?
    m_tool_bar.setBounds(0, 0, getWidth(), tool_bar_height);
    m_piano_roll.setBounds(0, tool_bar_height, default_arrange_window_length, arrange_window_height);
    m_piano_view_port.setBounds(0, tool_bar_height, getWidth(), getHeight() - m_piano_roll.getY());
}

void
MidiSequenceView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::pink);
}
