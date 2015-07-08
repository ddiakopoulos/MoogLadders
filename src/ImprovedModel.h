#pragma comment(user, "license")

#pragma once

#ifndef IMPROVED_LADDER_H
#define IMPROVED_LADDER_H

#include "LadderFilterBase.h"

/*
This model is based on a reference implementation of an algorithm developed by
Stefano D'Angelo and Vesa Valimaki, presented in a paper published at ICASSP in 2013. 
This improved model is based on a circuit analysis and compared against a reference 
Ngspice simulation. In the paper, it is noted that this particular model is
more accurate in preserving the self-oscillating nature of the real filter. 

References: "An Improved Virtual Analog Model of the Moog Ladder Filter"
Original Implementation: D'Angelo, Valimaki
*/

// Thermal voltage (26 milliwats at room temperature)
#define VT 0.026

struct LadderData
{
    double V1prev;
    double V2prev;
    double V3prev;
    double V4prev;

    double tV1prev;
    double tV2prev;
    double tV3prev;
    double tV4prev;

    double dV1prev;
    double dV2prev;
    double dV3prev;
    double dV4prev;
};

class ImprovedMoog : public LadderFilterBase
{
    
public:
    
    ImprovedMoog(float sampleRate) : LadderFilterBase(sampleRate)
    {
        ladder = new LadderData();
        memset(&ladder, 0, sizeof(LadderData));

        _drive = 0.05;

		SetCutoff(1000.0f);
        SetResonance(0.05f);
    }
    
    virtual ~ImprovedMoog()
    {
        delete ladder;
    }
    
    virtual void Process(float * samples, uint32_t n) noexcept override
    {
        double dV1, dV2, dV3, dV4;
        float inSample;
        
        for (int i = 0; i < n; i++)
        {
            inSample = samples[i];
            
            dV1 = -_g * (tanh((_drive * inSample + _resonance * ladder->V4prev) / (2.0 * VT)) + ladder->tV1prev);
            ladder->V1prev += (dV1 + ladder->dV1prev) / (2.0 * sampleRate);
            ladder->dV1prev = dV1;
            ladder->tV1prev = tanh(ladder->V1prev / (2.0 * VT));
            
            dV2 = _g * (ladder->tV1prev - ladder->tV2prev);
            ladder->V2prev += (dV2 + ladder->dV2prev) / (2.0 * sampleRate);
            ladder->dV2prev = dV2;
            ladder->tV2prev = tanh(ladder->V2prev / (2.0 * VT));
            
            dV3 = _g * (ladder->tV2prev - ladder->tV3prev);
            ladder->V3prev += (dV3 + ladder->dV3prev) / (2.0 * sampleRate);
            ladder->dV3prev = dV3;
            ladder->tV3prev = tanh(ladder->V3prev / (2.0 * VT));
            
            dV4 = _g * (ladder->tV3prev - ladder->tV4prev);
            ladder->V4prev += (dV4 + ladder->dV4prev) / (2.0 * sampleRate);
            ladder->dV4prev = dV4;
            ladder->tV4prev = tanh(ladder->V4prev / (2.0 * VT));
            
            samples[i] = ladder->V4prev;
        }
    }

    virtual void SetResonance(float r) override
    {
        _resonance = r;
    }
    
    virtual void SetCutoff(float c) override
    {
        _cutoff = c;
        _x = MOOG_PI * _cutoff / sampleRate;
        _g = 4.0 * MOOG_PI * VT * _cutoff * (1.0 - _x) / (1.0 + _x);
    }
    
private:

    LadderData * ladder;

    double _x;
    double _g; 
    
};

#endif
