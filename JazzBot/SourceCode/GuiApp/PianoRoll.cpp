//
//  PianoRoll.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#include "PianoRoll.hpp"
#include <unordered_set>

static const int keyboard_width = 50;
static const int default_piano_roll_width = 4000;
static const int number_of_notes = 12;

static const std::unordered_set<int> black_keys = {0,2,5,7,10};

PianoRoll::PianoRoll()
    : m_piano_roll_width(default_piano_roll_width)
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
}
