//
//  NoteGrouping.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/06/2022.
//

#ifndef NoteGrouping_hpp
#define NoteGrouping_hpp

#include <stdio.h>

#include "SequenceTypes.h"

class NoteGrouping
{
public:
    virtual MidiSequence getModifiedSequence(const MidiSequence& midi_sequence) = 0;
    virtual NoteGroupingData createDatabaseKeys(const std::vector<Sequence>& sequence) = 0;
    virtual ~NoteGrouping() {}
};

#endif /* NoteGrouping_hpp */
