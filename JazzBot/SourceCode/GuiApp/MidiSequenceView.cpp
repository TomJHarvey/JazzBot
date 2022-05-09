#include "MidiSequenceView.hpp"

static const int tool_bar_height = 20;
static const int arrange_window_height = (number_of_piano_keys + 1) * grid_line_height;


MidiSequenceView::MidiSequenceView(Listener* listener)
    : m_piano_roll_width(PianoRoll::getPianoRollWidth(default_number_of_bars))
    , m_number_of_bars(default_number_of_bars)
    , m_listener(listener)
{
    m_piano_roll = std::make_unique<PianoRoll>(this);
    m_piano_view_port = std::make_unique<CustomViewport>();
    m_piano_view_port->setViewedComponent (m_piano_roll.get(), false);
    m_piano_view_port->addListener(this);
    addAndMakeVisible (*m_piano_view_port);
    setBarLabels();
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
MidiSequenceView::updateBarLabels(const std::size_t& number_of_bars)
{
    m_number_of_bars = number_of_bars;
    setBarLabels();
}

void
MidiSequenceView::setBarLabels()
{
    int bar_number_position = keyboard_width;
    for (int i = 0; i < static_cast<int>(m_number_of_bars); i++)
    {
        m_bar_number_labels.add(new juce::Label({}, "Label Number " + juce::String(i)));
        m_bar_number_labels[i]->setText(std::to_string(i),juce::dontSendNotification);
        m_bar_number_labels[i]->setBounds(bar_number_position - m_piano_view_port->getViewPositionX(),
                                          0,
                                          keyboard_width,
                                          tool_bar_height);
        addAndMakeVisible(m_bar_number_labels[i]);
        bar_number_position += bar_line_width;
    }
}

void
MidiSequenceView::viewportAreaChanged(juce::Viewport* viewport,
                                      const juce::Rectangle<int>& newVisibleArea)
{
    juce::ignoreUnused(newVisibleArea);
    updateBarLabels(m_piano_roll->getNumberOfBars());
    m_listener->updateViewPort(this, viewport->getViewPositionX(), viewport->getViewPositionY());
}

void
MidiSequenceView::updateViewPort(const int& x_posistion, const int& y_posistion)
{
    m_piano_view_port->setViewPosition(x_posistion, y_posistion);
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
