#pragma comment(user, "license")

#pragma once

#ifndef AARON_LADDER_H
#define AARON_LADDER_H

#include "LadderFilterBase.h"

// http://song-swap.com/MUMT618/aaron/Presentation/demo.html
// http://song-swap.com/MUMT618/aaron/code/MoogVCF.h
// http://song-swap.com/MUMT618/aaron/code/MoogVCF.cpp

class AaronMoog : public LadderFilterBase
{
    
public:
    
    AaronMoog(float sampleRate) : LadderFilterBase(sampleRate)
    {
    }
    
    virtual ~AaronMoog()
    {
    }
    
    virtual void Process(float * samples, uint32_t n) noexcept override
    {
    }
    
    virtual void SetResonance(float r) override
    {
    }
    
    virtual void SetCutoff(float c) override
    {
    }
    
private:

};

#endif
