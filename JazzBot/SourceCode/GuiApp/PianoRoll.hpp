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

#include "../SequenceTypes.h"

/**
 @brief Constants for piano roll gui elements. It Might be better to put these in a constants file specifically for
        Piano roll and midiSequenceView if scaling were to come into play it may be slightly harder as some of these
        are ints and not floats.
 */
static const int number_of_piano_keys = 88;
static const int grid_line_height = 15;
static const int keyboard_width = 50;
static const int bar_line_width = 480;
static const int beat_line_width = 120;
static const std::size_t default_number_of_bars = 8;

/**
    @brief Class to display a midi sequence in a GUI representation of a piano roll
 */
class PianoRoll : public juce::Component
{
public:
    
    /**
        @brief A listener used to update the size of the viewport and accomponying bar labels
     */
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void resizeViewPort(const int& piano_roll_width) = 0;
        virtual void updateBarLabels(const std::size_t& number_of_bars) = 0;
    };
    
    /**
        @brief Constructor
     */
    PianoRoll(Listener* listener);
    
    /**
        @brief Destructor
     */
    virtual ~PianoRoll();
    
    /**
     @brief Draws a midi sequence and bar lines on the piano roll
     @param midi_sequence to draw
     */
    void setCurrentSequence(const MidiSequence& midi_sequence);
    
    /**
     @brief Calculates the width of the piano roll based on the number of bars
     @param number_of_bars used to calculate piano roll width
     @returns The width in pixels
     */
    static int getPianoRollWidth(const int& number_of_bars);
    
    /**
     @brief Gets the number number_of_bars in the piano roll
     @returns The number of bars
     */
    std::size_t getNumberOfBars() const;
   
    /** Overrides juce::Component */
    void paint(juce::Graphics& g) override;
    
    /** Overrides juce::Component */
    void resized() override;
    
private:
    
    /**
     @brief Draws the midi notes
     */
    void initialiseNotes();
    
    /**
     @brief Draws the horizontal lines for the piano roll
     */
    void drawHorizontalLines();
    
    /**
     @brief Draws the horizontal lines for the piano roll
     @param number_of_bars used to calculate the amount of vertical lines
     @param index position of where to draw the vertical lines
     */
    void drawVerticalLines(const std::size_t& number_of_bars, const std::size_t& index = 0);

    /** Number of bars in piano roll */
    std::size_t m_number_of_bars;
    
    /** The horizontal lines for the piano roll */
    juce::Path m_piano_grid_horizontal_lines[number_of_piano_keys];
    
    /** The vertical lines for each bar for the piano roll */
    std::vector<juce::Path> m_piano_grid_bar_lines; // do we need a vector as the data structure?
    
    /** The vertical lines for each beat for the piano roll */
    std::vector<juce::Path> m_piano_grid_beat_lines;
    
    /** The width of the piano roll in pixels  */
    int m_piano_roll_width;
    
    /** The sequence to draw on the piano roll  */
    MidiSequence m_current_sequence;
    
    /** The notes to draw on the piano roll */
    juce::OwnedArray<GuiNote> m_gui_notes;
    
    /** A listener to detect changes in class  */
    Listener* m_listener;
};

#endif /* PianoRoll_hpp */
