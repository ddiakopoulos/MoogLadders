#pragma comment(user, "license")

#pragma once

#ifndef SIMPLIFIED_LADDER_H
#define SIMPLIFIED_LADDER_H

#include "LadderFilterBase.h"

/*
The simplified nonlinear Moog filter is based on the full Huovilainen model,
with five nonlinear (tanh) functions (4 first-order sections and a feedback).
Like the original, this model needs an oversampling factor of at least two when
these nonlinear functions are used to reduce possible aliasing. This model
maintains the ability to self oscillate when the feedback gain is >= 1.0. 

References: DAFX - Zolzer (ed) (2nd ed)
Original implementation: Valimaki, Bilbao, Smith, Abel, Pakarinen, Berners (DAFX)

Considerations for oversampling: 
http://music.columbia.edu/pipermail/music-dsp/2005-February/062778.html
http://www.synthmaker.co.uk/dokuwiki/doku.php?id=tutorials:oversampling
*/

class SimplifiedMoog : public LadderFilterBase
{
    
public:
    
    SimplifiedMoog(float sampleRate) : LadderFilterBase(sampleRate)
    {
        // To keep the overall level approximately constant, comp should be set
        // to 0.5 resulting in a 6 dB passband gain decrease at the maximum resonance
        // (compared to a 12 dB decrease in the original Moog model
        gain_compensation = 0.5;
        
        _resonance = 0.10f;
        _cutoff = 1000.0f;
        
        for (int i = 0; i < 4; i++)
        {
            _stage[i] = 0.0;
            _stageZ1[i] = 0.0;
        }
        
        SetCutoff(1000.0f);
        SetResonance(0.10f);
    }
    
    virtual ~SimplifiedMoog()
    {
        
    }
    
    // This system is nonlinear so we are probably going to create a signal with components that exceed nyquist.
    // To prevent aliasing distortion, we need to oversample this processing chunk. Where do these extra samples
    // come from? Todo! We can use polynomial interpolation to generate the extra samples, but this is expensive.
    // The cheap solution is to zero-stuff the incoming sample buffer.
     
    // With resampling, numSamples should be 2x the frame size of the existing sample rate.
    // The output of this filter needs to be run through a decimator to return to the original samplerate.
    virtual void Process(float * samples, uint32_t n) noexcept override
    {
        // Processing still happens at sample rate...
        for (int samp = 0; samp < n; ++samp)
        {
            for (int stage = 0; stage < 4; ++stage)
            {
                // Stage 1, 2, 3
                if (stage)
                {
                    _input = _stage[stage-1];
                    _stageTANH[stage-1] = tanh(_input);
                    _stage[stage] = (_h * _stageZ1[stage] + _h0 * _stageTANH[stage-1]) + (1.0 - _g) * (stage != 3 ? _stageTANH[stage] : tanh(_stageZ1[stage]));
                }
                
                // New input, stage 0
                else
                {
                    _input = samples[samp] - ((4.0 * _resonance) * (_output - gain_compensation * samples[samp]));
                    _stage[stage] = (_h * tanh(_input) + _h0 * _stageZ1[stage]) + (1.0 - _g) * _stageTANH[stage];
                }
                
                _stageZ1[stage] = _stage[stage];
            }
            
            _output  = _stage[3];
            SNAP_TO_ZERO(_output);
            samples[samp] = _output;
        }
        
    }
    
    virtual void SetResonance(float r) override
    {
        // Direct mapping (no table or polynomial)
        _resonance = r;
    }
    
    virtual void SetCutoff(float c) override
    {
        _cutoff = c;
        
        float fs = sampleRate;
        float x2 = 2 * fs;
        
        // Normalized cutoff [0, 1] in radians: ((2*pi) * cutoff / samplerate)
        _g = (2 * MOOG_PI) * _cutoff / x2; // feedback coefficient at fs*2 because of doublesampling
        
        // FIR part with gain g
        _h = _g / 1.3;
        _h0 = _g * 0.3 / 1.3;
    }
    
private:
    
    double _output;
    double _last_stage;
    
    double _stage[4];
    double _stageZ1[4];
    double _stageTANH[3];
    
    double _input;
    
    double _h, _h0;
    double _g;
    
    float gain_compensation;
    
};

#endif
