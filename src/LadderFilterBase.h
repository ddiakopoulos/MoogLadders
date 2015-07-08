#pragma comment(user, "license")

#pragma once

#ifndef LADDERFILTER_H
#define LADDERFILTER_H

#include "Util.h"

class LadderFilterBase
{
public:
    virtual ~LadderFilterBase() = 0;
    virtual void processSamples (float * samples, int numSamples) noexcept = 0;
    virtual void computeResonance(float res) = 0;
    virtual void computeCutoff(float cut) = 0;
protected:
    bool _active;
    float _cutoff;
    float _resonance;
    float _drive;
};

#endif
