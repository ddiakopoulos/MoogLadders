#pragma comment(user, "license")

#pragma once

#ifndef HUOVILAINEN_LADDER_H
#define HUOVILAINEN_LADDER_H

#include "LadderFilterBase.h"

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

class HuovilainenMoog : public LadderFilterBase
{
public:
    
    HuovilainenMoog(float sampleRate) : LadderFilterBase(sampleRate), input(0), output(0)
    {
        memset(stage, 0, sizeof(stage));
        memset(stageZ1, 0, sizeof(stageZ1));
        memset(stageTanh, 0, sizeof(stageTanh));
        SetTransistorVoltage();
    }
    
    virtual ~HuovilainenMoog()
    {
        
    }
    
    virtual void Process(float * samples, uint32_t n) noexcept override
    {
        double localOutput;
        
        for (int s = 0; s < n; ++s)
        {
            // 2x oversampling
            for (int j = 0; j < 2; ++j)
            {
                // Filter stages (Huovilainen Fig 22)
                for (int stageIdx = 0; stageIdx < 4; ++stageIdx)
                {
                    if (stageIdx)
                    {
                        input = stage[stageIdx-1];
                        stageTanh[stageIdx-1] = tanh(input * thermal);
                        stage[stageIdx] = stageZ1[stageIdx] + tune * (stageTanh[stageIdx-1]) - (stageIdx != 3 ? stageTanh[stageIdx] : tanh(stageZ1[stageIdx] * thermal));
                    }
                    else
                    {
                        input = samples[s] - resonanceQuad * output;
                        stage[stageIdx] = stageZ1[stageIdx] + tune * (tanh(input * thermal) - stageTanh[stageIdx]);
                    }
                    
                    stageZ1[stageIdx] = stage[stageIdx];
                }
                
                // 0.5 sample delay for phase compensation (average two samples)
                localOutput = (stage[3] + lastStage) * 0.5;
                lastStage = stage[3];
            }
            
            output = localOutput;
            SNAP_TO_ZERO(output);
            samples[s] = output;
        }
    }
    
    virtual void SetResonance(float r) override
    {
        resonance = (r < 0) ? 0 : r;
        resonanceQuad = (4.0 * (double) resonance * acr); // (Modified Huovilainen Fig 23)
    }
    
    virtual void SetCutoff(float c) override
    {
        cutoff = c;
        
        double fc =  (double)(cutoff / sampleRate);
        
        double x_2 = fc / 2;
        double x2 = fc * fc;
        double x3 = fc * x2;
        
        // Frequency & amplitude compensation
        double fcr = 1.8730 * x3 + 0.4955 * x2 - 0.6490 * fc + 0.9988;
        
        // Resonance compensation
        acr = -3.9364 * x2 + 1.8409 * fc + 0.9968;
        
        // Normal scaled impulse invariant transformed one-pole filter; exp() models resonance
        // The coefficient g determines the cutoff frequency
        tune = (1.0 - exp(-(MOOG_2_PI * x_2 * fcr))) / thermal;
        
        SetResonance(resonance);
    }

    // Transistor base-emitter voltage simulation
    void SetTransistorVoltage(const double V = 1.22070313)
    {
        thermal = (1.0 / V);
        SetCutoff(1000.0f);
        SetResonance(0.10f);
    }
    
private:
    
    double output;
    double lastStage;
    
    double stage[4];
    double stageZ1[4];
    double stageTanh[3];
    
    double resonanceQuad;
    
    double thermal;
    double input;
    double tune;
    double acr;
}; 

#endif
