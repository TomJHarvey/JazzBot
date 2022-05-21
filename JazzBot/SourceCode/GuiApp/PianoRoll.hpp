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

#include "GuiNote.hpp"
#include "../MidiSequenceUtility.hpp"

// Might be better to put these in a constants file specifically for Piano roll and midiSequenceView
// if scaling were to come into play it may be slightly harder as some of these are ints and not floats.
static const int number_of_piano_keys = 88;
static const int grid_line_height = 15;
static const int keyboard_width = 50;
static const int bar_line_width = 480;
static const int beat_line_width = 120;
static const std::size_t default_number_of_bars = 8;

class PianoRoll : public juce::Component
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void resizeViewPort(const int& piano_roll_width) = 0;
        virtual void updateBarLabels(const std::size_t& number_of_bars) = 0;
    };
    
    PianoRoll(Listener* listener);
    virtual ~PianoRoll();
    void setCurrentSequence(const MidiSequence& midi_sequence);
    static int getPianoRollWidth(const int& number_of_bars);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void initialiseNotes();
    std::size_t getNumberOfBars() const;
    
private:
    void drawHorizontalLines();
    void drawVerticalLines(const std::size_t& number_of_bars, const std::size_t& index = 0);

    std::size_t m_number_of_bars;
    juce::Path m_piano_grid_horizontal_lines[number_of_piano_keys];
    std::vector<juce::Path> m_piano_grid_bar_lines; // do we need a vector as the data structure?
    std::vector<juce::Path> m_piano_grid_beat_lines;
    int m_piano_roll_width;
    MidiSequence m_current_sequence;
    juce::OwnedArray<GuiNote> m_gui_notes;
    Listener* m_listener;
};

#endif /* PianoRoll_hpp */
