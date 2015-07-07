#pragma comment(user, "license")

#pragma once

#ifndef HUOVILAINEN_LADDER_H
#define HUOVILAINEN_LADDER_H

#include "LadderFilter.h"

/*
Huovilainen developed an improved and physically correct model of the Moog
Ladder filter that builds upon the work done by Smith and Stilson. This model
inserts nonlinearities inside each of the 4 one-pole sections on account of the
smoothly saturating function of analog transistors. The base-emitter voltages of
the transistors are considered with an experimental value of 1.22070313 which
maintains the characteristic sound of the analog Moog. This model also permits
self-oscillation for resonances greater than 1. The model depends on five
hyperbolic tangent functions (tanh) for each sample, and an oversampling factor
of two (preferably higher, if possible). Although a more faithful
representation of the Moog ladder, these dependencies increase the processing
time of the filter significantly. Lastly, a half-sample delay is introduced for 
phase compensation at the final stage of the filter. 

References: Huovilainen (2004), Huovilainen (2010), DAFX - Zolzer (ed) (2nd ed)
Original implementation: Victor Lazzarini for CSound5

Considerations for oversampling: 
http://music.columbia.edu/pipermail/music-dsp/2005-February/062778.html
http://www.synthmaker.co.uk/dokuwiki/doku.php?id=tutorials:oversampling
*/ 

class HuovilainenMoog : public LadderFilter
{
    
public:
    
    HuovilainenMoog()
    {
        _resonance = 0.10f;
        _resonanceQuad = 0.0f;
        
        _cutoff = 1000.0f;
        _acr = 0.0f;
        _tune = 0.0f;
        
        float V = 1.22070313;
        _thermal = (1.0 / V);
        
        for (int i = 0; i < 4; i++) {
            _stage[i] = 0.0;
            _stageZ1[i] = 0.0;
        }
        
        for (int i = 0; i < 3; i++)
            _stageTANH[i] = 0.0;
        
        computeCutoff(1000);
        computeResonance(0.1f);
    }
    
    virtual ~HuovilainenMoog()
    {
        
    }
    
    virtual void processSamples(float* samples, int numSamples) noexcept override
    {
        // 2v = thermal
        double local_output;
        
        for (int samp = 0; samp < numSamples; ++samp)
        {
            // 2x oversampling
            for (int j = 0; j < 2; ++j)
            {
                
                // Filter stages (Huovilainen Fig 22)
                for (int stage = 0; stage < 4; ++stage)
                {
                    // Stages 1, 2, 3
                    if (stage)
                    {
                        _input = _stage[stage-1];
                        _stageTANH[stage-1] = tanh(_input*_thermal);
                        _stage[stage] = _stageZ1[stage] + _tune * (_stageTANH[stage-1]) - (stage != 3 ? _stageTANH[stage] : tanh(_stageZ1[stage]*_thermal));
                    }
                    // New input, stage 0
                    else
                    {
                        _input = samples[samp] - _resonanceQuad * _output;
                        _stage[stage] = _stageZ1[stage] + _tune * (tanh(_input*_thermal) - _stageTANH[stage]);
                    }
                    
                    _stageZ1[stage] = _stage[stage];
                }
                
                // .5 sample delay for phase compensation
                // This can be realized by averaging two samples.
                local_output = (_stage[3] + _last_stage) * 0.5;
                _last_stage = _stage[3];
                
            }
            
            _output = local_output;
            SNAP_TO_ZERO(_output);
            samples[samp] = _output;
        }
    }
    
    virtual void computeResonance(float r) override
    {
        if (r < 0)
            _resonance = 0;
        else
            _resonance = r;
        
        _resonanceQuad = (4.0 * (double) _resonance * _acr); // (Modified Huovilainen Fig 23)
    }
    
    virtual void computeCutoff(float c) override
    {
        _cutoff = cut;
        
        // Normalized Cutoff
        double fc =  (double)(_cutoff / 44100);
        
        double x_2 = fc/2;
        double x2 = fc*fc;
        double x3 = fc*x2;
        
        // Frequency & amplitude correction
        double fcr = 1.8730 * x3 + 0.4955 * x2 - 0.6490 * fc + 0.9988;
        
        // Resonance compensation
        _acr = -3.9364 * x2 + 1.8409 * fc + 0.9968;
        
        // Normal scaled impulse invariant transformed one-pole filter; exp() models resonance
        // The coefficient g determines the cutoff frequency
        _tune = (1.0 - exp(-((2*M_PI)*x_2*fcr))) / _thermal;
        
        computeResonance(_resonance);
    }

    void setTransistorVoltage(const float & V = 1.22070313)
    {
        // Base-emitter voltages of the transistors
        _thermal = (1.0 / V);
        
        computeCutoff(1000);
        computeResonance(0.1f);
    }
    
private:
    
    double   _output; 
    double   _last_stage;
    
    double   _stage[4];
    double   _stageZ1[4];
    double   _stageTANH[3];
    
    double  _resonanceQuad; 
    
    double  _thermal, _input, _tune, _acr;
    
}; 

#endif
