#ifndef ML_STILSON_H
#define ML_STILSON_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

// Description
//============================================================================== 
/*
 
 A digital model of the classic Moog filter was presented first by Stilson and
 Smith. This model uses a cascade of one-pole IIR filters in series with a global
 feedback to produce resonance. A digital realization of this filter introduces a
 unit delay, effectively making it a fifth-order filter. Unfortunately, this
 delay also has the effect of coupling the cutoff and resonance parameters,
 uncharacteristic of the uncoupled control of the original Moog ladder. As a
 compromise, a zero can be inserted at z = -0.3 inside each one pole section to
 minimize the coupling the parameters (humans are not particularly sensitive to
 variations in Q factor). Although fast coefficient updates can be achieved since
 the nonlinearities of the Moog are not considered, the filter becomes unstable
 with very large resonance values and does not enter self-oscillation. 
 
 References: Stilson and Smith (1996), DAFX - Zolzer (ed) (2nd ed)
 Original implementation: Tim Stilson 
 
 */ 

class StilsonMoog : public LadderFilter {
    
public:
    
    StilsonMoog();
    ~StilsonMoog();
    
    void processSamples (float* samples, int numSamples) noexcept;
    float processSingleSampleRaw (float sample) noexcept;
    
    void computeResonance(float res);
    void computeCutoff(float cut);

private:
    
    float _p, _Q; 
    float _state[4];
    float _output; 
    
}; 

#endif