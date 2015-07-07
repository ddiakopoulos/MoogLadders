#pragma comment(user, "license")

#pragma once

#ifndef LADDERFILTER_H
#define LADDERFILTER_H

class LadderFilter
{
public:
    virtual ~LadderFilter() {};
    virtual void processSamples (float* samples, int numSamples) noexcept = 0;
    virtual void computeResonance(float res) = 0;
    virtual void computeCutoff(float cut) = 0;
protected:
    bool _active;
    float _cutoff;
    float _resonance;
    float _drive;
};

#endif
