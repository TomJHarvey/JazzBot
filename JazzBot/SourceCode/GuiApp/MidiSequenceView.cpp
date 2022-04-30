#include "MidiSequenceView.hpp"

static const int tool_bar_height = 20;
static const int arrange_window_height = (number_of_piano_keys + 1) * grid_line_height;

MidiSequenceView::MidiSequenceView()
{
    m_piano_roll_width = PianoRoll::getPianoRollWidth(default_number_of_bars);
    m_piano_roll = std::make_unique<PianoRoll>(this);
    m_piano_view_port = std::make_unique<juce::Viewport>();
    m_piano_view_port->setViewedComponent (m_piano_roll.get(), false);
    addAndMakeVisible (*m_piano_view_port);
}

void
MidiSequenceView::loadSequence(const juce::File& midi_file)
{
    m_midi_sequence.clear();
    if (MidiFileUtility::parseMidiFile(midi_file, m_midi_sequence))
    {
        displaySequence();
    }
}

void
MidiSequenceView::displaySequence()
{
    if (!m_midi_sequence.empty())
    {
       // m_piano_roll.setCurrentSequence(m_midi_sequence);
        m_piano_roll->setCurrentSequence(m_midi_sequence);
    }
}

void
MidiSequenceView::setSequence(const MidiSequence& midi_sequence)
{
    m_midi_sequence = midi_sequence;
}

bool
MidiSequenceView::getCurrentSequence(MidiSequence& midi_sequence) const
{
    if (!m_midi_sequence.empty())
    {
        midi_sequence = m_midi_sequence;
    }
    return (!midi_sequence.empty());
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
    m_piano_roll->setBounds(0, tool_bar_height, m_piano_roll_width, arrange_window_height);
    m_piano_view_port->setBounds(0, tool_bar_height, getWidth(), getHeight() - m_piano_roll->getY());
}

void
MidiSequenceView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::pink);
    g.setColour(juce::Colours::slateblue);
    g.drawRect(0, 0, getWidth(), tool_bar_height);
    g.fillRect(0, 0, getWidth(), tool_bar_height);
    m_piano_roll->repaint();
}
