#pragma comment(user, "license")

#pragma once

#ifndef RK_SIMULATION_LADDER_H
#define RK_SIMULATION_LADDER_H

#include "LadderFilterBase.h"

class RKSimulationMoog : public LadderFilterBase
{
    
public:
    
    RKSimulationMoog(float sampleRate) : LadderFilterBase(sampleRate)
    {
    }
    
    virtual ~RKSimulationMoog()
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
