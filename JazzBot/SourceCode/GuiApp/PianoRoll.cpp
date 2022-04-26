//
//  PianoRoll.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#include "PianoRoll.hpp"
#include <unordered_set>

static const int number_of_notes = 12;
static const int bar_line_height =  number_of_piano_keys *  grid_line_height;
static const std::unordered_set<int> black_keys = {0,2,5,7,10};
static const std::size_t m_midi_bar_length = 960;
// static const float beat_line_width = 480; // todo

PianoRoll::PianoRoll(Listener* listener)
    : m_number_of_bars(default_number_of_bars)
    , m_listener(listener)
{
    m_piano_roll_width = getPianoRollWidth(default_number_of_bars);
    drawHorizontalLines();
    drawVerticalLines(m_number_of_bars);
}

void
PianoRoll::drawHorizontalLines()
{
    int grid_line_position = grid_line_height;
    
    for (int count = 0; count < number_of_piano_keys; count ++)
    {
        m_piano_grid_horizontal_lines[count].startNewSubPath(0, grid_line_position);
        m_piano_grid_horizontal_lines[count].lineTo(m_piano_roll_width, grid_line_position);
        m_piano_grid_horizontal_lines[count].closeSubPath();
        grid_line_position += grid_line_height;
    }
}

void
PianoRoll::drawVerticalLines(const std::size_t& number_of_bars,const std::size_t& index)
{
    // this needs own function, it can change. change default value
    m_piano_grid_bar_lines.resize(m_number_of_bars); // is this resize safe, does it just chop off the end values?
    std::size_t line_width = keyboard_width + (bar_line_width * (index +1));
    for (std::size_t count = index; count < number_of_bars; count ++)
    {
        m_piano_grid_bar_lines[count].startNewSubPath(line_width, 0);
        m_piano_grid_bar_lines[count].lineTo(line_width, bar_line_height);
        m_piano_grid_bar_lines[count].closeSubPath();
        line_width += bar_line_width;
    }
}

void
PianoRoll::setCurrentSequence(const MidiSequence& midi_sequence)
{
    m_current_sequence = midi_sequence; // think a copy is okay, other option probably isnt needed or as safe.
    std::size_t number_of_bars = (static_cast<std::size_t>(m_current_sequence[m_current_sequence.size()-1].note_off) / m_midi_bar_length)/ 10;
    m_piano_grid_bar_lines.resize(number_of_bars);
    drawVerticalLines(number_of_bars, m_number_of_bars);
    m_piano_roll_width = getPianoRollWidth(static_cast<int>(number_of_bars));
    m_listener->resizeViewPort(m_piano_roll_width);
    m_number_of_bars = number_of_bars;
    drawHorizontalLines();
    repaint();
}

int
PianoRoll::getPianoRollWidth(const int &number_of_bars)
{
    return (number_of_bars * bar_line_width) + keyboard_width;
}

void
PianoRoll::resized()
{
    // set size of child components
}

void
PianoRoll::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::grey);
    g.setColour (juce::Colours::white);
    g.drawRect(0, 0, keyboard_width, getHeight());
    g.fillRect(0, 0, keyboard_width, getHeight());
    
    g.setColour(juce::Colours::darkgrey);
    
    for (int count = 0; count < number_of_piano_keys; count ++)
    {
        g.strokePath(m_piano_grid_horizontal_lines[count], juce::PathStrokeType (0.5f));
        if (black_keys.find(count % number_of_notes) != black_keys.end())
        {
            g.setColour(juce::Colours::black);
            int line_height = (grid_line_height) * (count) + 1;
            g.drawRect(0, line_height , keyboard_width, grid_line_height -2);
            g.fillRect(0, line_height , keyboard_width, grid_line_height -2);
            g.setColour(juce::Colours::darkgrey);
            g.drawRect(keyboard_width, line_height , m_piano_roll_width - keyboard_width, grid_line_height -2);
            g.fillRect(keyboard_width, line_height , m_piano_roll_width - keyboard_width, grid_line_height -2);
            g.setColour(juce::Colours::darkgrey);
        }
    }
    
    g.setColour(juce::Colours::black);
    for (std::size_t count = 0; count < m_number_of_bars; count ++)
    {
        g.strokePath(m_piano_grid_bar_lines[count], juce::PathStrokeType (1.0f));
    }
}
