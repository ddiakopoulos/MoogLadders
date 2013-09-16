#ifndef ML_IMPROVEDMOOG_H
#define ML_IMPROVEDMOOG_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

// Description
//============================================================================== 
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
#define VT  0.026

typedef struct {

    double       V1prev;
    double       V2prev;
    double       V3prev;
    double       V4prev;

    double       tV1prev;
    double       tV2prev;
    double       tV3prev;
    double       tV4prev;

    double       dV1prev;
    double       dV2prev;
    double       dV3prev;
    double       dV4prev;

} Ladder;

class ImprovedMoog : public LadderFilter {
    
public:
    
    ImprovedMoog();
    ~ImprovedMoog();
    
    void processSamples (float* samples, int numSamples) noexcept;

    void computeResonance(float r);
    void computeCutoff(float c);
    
private:

    Ladder* ladder;

    double _x;
    double _g; 
    
};


#endif