//
//  TrainingDataView.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 24/04/2022.
//

#include "TrainingDataView.hpp"

static const int training_data_tool_bar_height = 50;
static const int midi_sequence_view_height = 450;


TrainingDataView::TrainingDataView(TrainingDataToolBar::Listener* m_toolbar_listener)
    : m_toolbar(m_toolbar_listener)
{
    addAndMakeVisible(m_original_sequence);
    addAndMakeVisible(m_modified_sequence);
    addAndMakeVisible(m_toolbar);
}

void
TrainingDataView::resized()
{
    // fixed size;
    m_toolbar.setBounds(0, 0, getWidth(), training_data_tool_bar_height);
    m_original_sequence.setBounds(0,
                                  training_data_tool_bar_height,
                                  getWidth(),
                                  midi_sequence_view_height);
    m_modified_sequence.setBounds(0,
                                  training_data_tool_bar_height + midi_sequence_view_height,
                                  getWidth(),
                                  midi_sequence_view_height);
}

void
TrainingDataView::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::black); // menu colour tbd
}
