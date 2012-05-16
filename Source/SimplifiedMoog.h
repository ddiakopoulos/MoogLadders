#ifndef ML_SIMPLFIEDMOOG_H
#define ML_SIMPLFIEDMOOG_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

// Description
//============================================================================== 
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


class SimplifiedMoog : public LadderFilter {
    
public:
    
    SimplifiedMoog();
    ~SimplifiedMoog();
    
    void processSamples (float* samples, int numSamples) noexcept;
    
    void computeResonance(float r);
    void computeCutoff(float c);
    
private:
    
    double   _output; 
    double   _last_stage;
    
    double   _stage[4];
    double   _stageZ1[4];
    double   _stageTANH[3];
    
    double   _input; 
    
    double   _h, _h0;
    double   _g; 
    
    float    gain_compensation; 
    
};


#endif