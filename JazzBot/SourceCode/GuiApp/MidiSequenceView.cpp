//
//  MidiSequenceView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#include "MidiSequenceView.hpp"

static const int tool_bar_height = 20;
static const int arrange_window_height = (number_of_piano_keys + 1) * grid_line_height;

MidiSequenceView::MidiSequenceView()
    : m_piano_roll(this)
{
    m_piano_roll_width = PianoRoll::getPianoRollWidth(default_number_of_bars);
    addAndMakeVisible(m_piano_roll);
    m_piano_view_port.setViewedComponent(&m_piano_roll, false);
    m_piano_view_port.setScrollBarsShown(true,true);
    addAndMakeVisible(m_piano_view_port);
}

void
MidiSequenceView::loadSequence(const juce::File& midi_file)
{
    // Then have the sequence loaded into a data structure, then printed into the piano roll.
    // If sequence in piano roll, remove all notes, load a new sequence.
    
     if (MidiFileUtility::parseMidiFile(midi_file, m_midi_sequence))
     {
         m_piano_roll.setCurrentSequence(m_midi_sequence);
     }
}

void
MidiSequenceView::resizeViewPort(const int& piano_roll_width)
{
    m_piano_roll_width = piano_roll_width;
    resized();
}

void
MidiSequenceView::resized()
{
    // set size of child components, is it needed here too?
    m_piano_roll.setBounds(0, tool_bar_height, m_piano_roll_width, arrange_window_height);
    m_piano_view_port.setBounds(0, tool_bar_height, getWidth(), getHeight() - m_piano_roll.getY());
}

void
MidiSequenceView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::pink);
    g.setColour(juce::Colours::slateblue);
    g.drawRect(0, 0, getWidth(), tool_bar_height);
    g.fillRect(0, 0, getWidth(), tool_bar_height);
    m_piano_roll.repaint();
}
