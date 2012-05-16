#ifndef dafx_SimplifiedMoog_h
#define dafx_SimplifiedMoog_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

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
    
    double  _input; 
    
    double _h, _h0;
    double _g; 
    
    float gain_compensation; 
    
};


#endif
