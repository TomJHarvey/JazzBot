//
//  MidiSequenceView.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 25/04/2022.
//

#ifndef MidiSequenceView_hpp
#define MidiSequenceView_hpp

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdio.h>
#include "CustomViewport.hpp"
#include "PianoRoll.hpp"

/**
    @brief This class is used to view a midi sequence on a piano roll
 */
class MidiSequenceView : public juce::Component
                       , public PianoRoll::Listener
                       , public CustomViewport::Listener
{
public:
    
    /**
        @brief A listener used when the viewport area is changed
     */
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void updateViewPort(const MidiSequenceView* const changed_view,
                                    const int& x_posistion,
                                    const int& y_posistion) = 0;
    };
    
    /**
        @brief Constructor
     */
    MidiSequenceView(Listener* listener);

    /**
     @brief Reads a midi file and displays it onto the piano roll
     @param midi_file to read
     */
    void loadSequence(const juce::File& midi_file);
    
    /**
     @brief Displays the current sequence
     */
    void displaySequence();
    
    /**
     @brief Sets the midi sequence from an existing midi sequence
     @param midi_sequence  copy
     */
    void setSequence(const MidiSequence& midi_sequence);
    
    /**
     @brief Re draws the bar label positions after the piano viewpor position hast has been changed
     */
    void setBarLabels();
    
    /**
     @brief Retrives the sql to create the database for this grouping.
     @param midi_sequence  to populate
     @returns if the sequence was retrieved
     */
    bool getCurrentSequence(MidiSequence& midi_sequence) const;
    
    /**
     @brief Updates the viewports position
     @param x_posistion  to use to reposition
     @param y_posistion  to use to reposition
     */
    void updateViewPort(const int& x_posistion, const int& y_posistion);
    
    /** Overrides juce::Component */
    void paint(juce::Graphics& g) override;
    
    /** Overrides juce::Component */
    void resized() override;

    /** Overrides PianoRoll::Listener */
    void resizeViewPort(const int& piano_roll_width) override;
    
    /** Overrides PianoRoll::Listener */
    void updateBarLabels(const std::size_t& number_of_bars) override;
    
    /** Overrides CustomViewport::Listener*/
    void viewportAreaChanged(juce::Viewport* viewport, const juce::Rectangle<int>& newVisibleArea) override;

private:
    /** The piano roll */
    std::unique_ptr<PianoRoll> m_piano_roll;
    
    /** Viewport used for the the piano roll */
    std::unique_ptr<CustomViewport> m_piano_view_port;
    
    /** Width of the piano roll, which can change depending on the sequence size */
    int m_piano_roll_width;
    
    /** The current midi sequence */
    MidiSequence m_midi_sequence;
    
    /** A group of labels containing the bar numbers*/
    juce::OwnedArray<juce::Label> m_bar_number_labels;
    
    /** The number of bars which can change depending on the sequence size*/
    std::size_t m_number_of_bars;
    
    /** A listener used to detect changes in the position of the current viewport*/
    Listener* m_listener;
};

#endif /* MidiSequenceView_hpp */
