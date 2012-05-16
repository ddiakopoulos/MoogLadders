#ifndef dafx_Huovilainen_h
#define dafx_Huovilainen_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

/* Huovilainen has developed an improved model that models the ladder circuit by 
inserting nonlinearities inside the one-pole sections [3]. This improved model
more closely emulates the characteristic sound and is also capable of self-oscillation.
A disadvantage is the need for five hyperbolic tangent (tanh) function evaluations 
per sample and oversampling by factor two at least. */ 

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