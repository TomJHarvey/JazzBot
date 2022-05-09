//
//  PianoRoll.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#include "PianoRoll.hpp"
#include <unordered_set>

static const int number_of_notes = 12;
static const int bar_line_height =  (number_of_piano_keys *  grid_line_height);
static const std::unordered_set<int> black_keys = {1,4,6,9,11};
static const std::size_t m_midi_bar_length = 1920;
static const int number_of_midi_note_values = 127;
static const int gui_note_scaling = 4;

static const int time_signature = 4; // in future this will be got from the song information file, along with artists title to display

PianoRoll::PianoRoll(Listener* listener)
    : m_number_of_bars(default_number_of_bars)
    , m_listener(listener)
{
    m_piano_roll_width = getPianoRollWidth(default_number_of_bars);
    drawHorizontalLines();
    drawVerticalLines(m_number_of_bars);
}

PianoRoll::~PianoRoll()
{
    m_gui_notes.clearQuick(true);
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
    m_piano_grid_bar_lines.resize(m_number_of_bars); // is this resize safe, does it just chop off the end values?
    m_piano_grid_beat_lines.resize(m_number_of_bars * time_signature);
    std::size_t line_position = keyboard_width + (bar_line_width * (index + 1)); // time sig would also affect this
    for (std::size_t count = index; count < number_of_bars; count ++)
    {
        m_piano_grid_bar_lines[count].startNewSubPath(line_position, 0);
        m_piano_grid_bar_lines[count].lineTo(line_position, bar_line_height);
        m_piano_grid_bar_lines[count].closeSubPath();
        line_position += bar_line_width;
    }
    
    line_position = keyboard_width + (bar_line_width * (index));
    for (std::size_t count = index; count < number_of_bars * time_signature; count ++)
    {
        m_piano_grid_beat_lines[count].startNewSubPath(line_position, 0);
        m_piano_grid_beat_lines[count].lineTo(line_position, bar_line_height);
        m_piano_grid_beat_lines[count].closeSubPath();
        line_position += beat_line_width;
    }
}

void
PianoRoll::setCurrentSequence(const MidiSequence& midi_sequence)
{
    m_current_sequence.clear();
    m_current_sequence = midi_sequence; // think a copy is okay, other option probably isnt needed or as safe.
    std::size_t number_of_bars =
        (static_cast<std::size_t>(m_current_sequence[m_current_sequence.size()-1].note_off) / m_midi_bar_length);
    
    m_piano_grid_bar_lines.resize(number_of_bars);
    m_piano_grid_beat_lines.resize(number_of_bars * time_signature);
    if (number_of_bars > m_number_of_bars)
    {
        drawVerticalLines(number_of_bars, m_number_of_bars);
    }
    else
    {
        drawVerticalLines(number_of_bars);
    }
    m_number_of_bars = number_of_bars;
    drawHorizontalLines();
    m_piano_roll_width = getPianoRollWidth(static_cast<int>(number_of_bars));
    m_listener->resizeViewPort(m_piano_roll_width);
    initialiseNotes();
}

void
PianoRoll::initialiseNotes()
{
    for (auto& gui_note: m_gui_notes)
    {
        gui_note->setVisible(false);
        removeChildComponent(gui_note);
    }
    m_gui_notes.clearQuick(true);
    m_gui_notes.remove(0);
    
    for (std::size_t index = 0; index < m_current_sequence.size(); index++)
    {
        m_gui_notes.add(std::make_unique<GuiNote>());
        int x_pos = static_cast<int>(m_current_sequence[index].note_on)/gui_note_scaling;
        int width = (static_cast<int>(m_current_sequence[index].note_off)/gui_note_scaling) - x_pos;
        int y_pos = (grid_line_height*number_of_midi_note_values) -
                    (grid_line_height * m_current_sequence[index].note_value);
        m_gui_notes[static_cast<int>(index)]->setBounds(x_pos + keyboard_width, y_pos ,width, grid_line_height);
        addAndMakeVisible(m_gui_notes[static_cast<int>(index)]);
    }
}

std::size_t
PianoRoll::getNumberOfBars() const
{
    return m_number_of_bars;
}

int
PianoRoll::getPianoRollWidth(const int &number_of_bars)
{
    return (number_of_bars * bar_line_width) + keyboard_width;
}

void
PianoRoll::resized()
{

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
            int line_height = (grid_line_height * count) + 1;
            g.drawRect(0, line_height , keyboard_width, grid_line_height -2);
            g.fillRect(0, line_height , keyboard_width, grid_line_height -2);
            g.setColour(juce::Colours::darkgrey);
            g.drawRect(keyboard_width, line_height , m_piano_roll_width - keyboard_width, grid_line_height -2);
            g.fillRect(keyboard_width, line_height , m_piano_roll_width - keyboard_width, grid_line_height -2);
            g.setColour(juce::Colours::darkgrey);
        }
    }
    
    g.setColour(juce::Colours::darkslategrey);
    for (std::size_t count = 0; count < m_number_of_bars * time_signature; count ++)
    {
        g.strokePath(m_piano_grid_beat_lines[count], juce::PathStrokeType (0.5f));
    }
    
    g.setColour(juce::Colours::black);
    for (std::size_t count = 0; count < m_number_of_bars; count ++)
    {
        g.strokePath(m_piano_grid_bar_lines[count], juce::PathStrokeType (1.0f));
    }
}
