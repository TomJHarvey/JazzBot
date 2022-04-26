//
//  PianoRoll.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#ifndef PianoRoll_hpp
#define PianoRoll_hpp

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdio.h>

#include "../MidiFileUtility.hpp"

// Might be better to put these in a constants file specifically for Piano roll and midiSequenceView
// if scaling were to come into play it may be slightly harder as some of these are ints and not floats.
static const int number_of_piano_keys = 88;
static const int grid_line_height = 15;
static const std::size_t default_number_of_bars = 8;
static const int keyboard_width = 50;
static const int bar_line_width = 480;

class PianoRoll : public juce::Component
{
public:
    class Listener
    {
    public:
        virtual void resizeViewPort(const int& piano_roll_width) = 0;
    };
    
    PianoRoll(Listener* listener);
    void setCurrentSequence(const MidiSequence& midi_sequence);
    static int getPianoRollWidth(const int& number_of_bars);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    void drawHorizontalLines();
    void drawVerticalLines(const std::size_t& number_of_bars, const std::size_t& index = 0);

    juce::Path m_piano_grid_horizontal_lines[number_of_piano_keys];
    std::vector<juce::Path> m_piano_grid_bar_lines;
    int m_piano_roll_width; // might be resizeable.
    std::size_t m_number_of_bars;
    MidiSequence m_current_sequence;
    Listener* m_listener;
    
};



#endif /* PianoRoll_hpp */
