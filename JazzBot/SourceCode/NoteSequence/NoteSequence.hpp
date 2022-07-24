//
//  NoteSequence.hpp
//  JazzBotGui
//
//  Created by Tom Harvey on 23/06/2022.
//

#ifndef NoteSequence_hpp
#define NoteSequence_hpp

#include "../SequenceTypes.h"

class NoteSequence
{
public:
    virtual MidiSequence generateSequence(const std::size_t& number_of_choruses) = 0;
    virtual ~NoteSequence() {}
};

#endif /* NoteSequence_hpp */
