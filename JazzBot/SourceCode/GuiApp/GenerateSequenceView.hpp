//
//  GenerateSequenceView.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/06/2022.
//

#ifndef GenerateSequenceView_hpp
#define GenerateSequenceView_hpp

#include <stdio.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "MidiSequenceView.hpp"
#include "../NoteGrouping/NoteGrouping.hpp"
#include "../SequenceTypes.h"
#include "../NoteSequence/NoteSequence.hpp"

class GenerateSequenceView : public juce::Component
                           , public MidiSequenceView::Listener
                           , public juce::Button::Listener
{
public:
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void exitGenerateSequence() = 0;
    };
    
    GenerateSequenceView(Listener* listener);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void updateViewPort(const MidiSequenceView* const changed_view, const int& x_posistion, const int& y_posistion) override;
    void buttonClicked(juce::Button* button) override;

private:
    MidiSequenceView m_midi_sequence;
    ChordSequence m_chord_sequence;
    SongInformation m_song_information;
    juce::File m_chord_sequence_file;
    juce::File m_song_information_file;
    Listener* m_listener;
    juce::TextButton m_return_to_menu_button;
    juce::TextButton m_select_chord_sequence_button;
    juce::TextButton m_generate_melody_button;
    std::unique_ptr<NoteSequence> m_note_sequence;

};

#endif /* GenerateSequenceView_hpp */
