#pragma comment(user, "license")

#pragma once

#ifndef LADDER_FILTER_BASE_H
#define LADDER_FILTER_BASE_H

#include "Util.h"

class LadderFilterBase
{
    
public:
    
    LadderFilterBase(float sampleRate) : sampleRate(sampleRate) {}
    virtual ~LadderFilterBase() {}
    
    virtual void Process(float * samples, uint32_t n) noexcept = 0;
    
    virtual void SetResonance(float r) = 0;
    virtual void SetCutoff(float c) = 0;
    
    float GetResonance() { return _resonance; }
    float GetCutoff() { return _cutoff; }
    
protected:
    
    bool _active;
    float _cutoff;
    float _resonance;
    float _drive;
    
    float sampleRate;
};

#endif
