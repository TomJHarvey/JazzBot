//
//  Sequence.hpp
//  MIDI_FILE_GENERATOR
//
//  Created by Tom Harvey on 30/03/2022.
//

#ifndef Sequence_hpp
#define Sequence_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

class Sequence
{
public:
    Sequence() = default;
    void setBeatInformation(const float& bar_onset,
                            const float& quarter_note_increment,
                            float& previous_beat_onset);
    float getMidiTickDuration(const float& note_duration,
                              const float& note_onset) const;
    float getMidiTickOnset(const float& note_onset) const;
    bool isNoteBeforeFirstBar(const float& note_onset) const;
    bool setCurrentBeat(const float& note_onset);
    static int getPpqn();
    static float calculateQuarterNoteIncrement(const float& next_bar_onset, const float& bar_onset);

private:
    bool findBeat(const float& note_onset, std::size_t& beat) const;
    
private:
    std::vector<float> m_beat_onsets;
    std::vector<float> m_beat_lengths;
    std::size_t m_current_beat;
};


#endif /* Sequence_hpp */
