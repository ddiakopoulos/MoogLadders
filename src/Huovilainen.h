#ifndef ML_HUOVILAINEN_H
#define ML_HUOVILAINEN_H

#include "../JuceLibraryCode/JuceHeader.h"
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

class HuovilainenMoog : public LadderFilter {
    
public:
    
    HuovilainenMoog();
    ~HuovilainenMoog();
    
    void processSamples (float* samples, int numSamples) noexcept;
    
    void computeResonance(float r);
    void computeCutoff(float c);

    void setTransistorVoltage(const float& V=1.22070313);
    
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