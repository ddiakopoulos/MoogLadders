#ifndef ML_MOOGFILTER_BASE_H
#define ML_MOOGFILTER_BASE_H

#define SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;

class LadderFilter {
    
public:
    
    virtual ~LadderFilter() {};
    
    virtual void processSamples (float* samples, int numSamples) noexcept {};
    virtual void computeResonance(float res) {};
    virtual void computeCutoff(float cut) {};
    
protected: 
    
    bool _active;
    
    float _cutoff;
    float _resonance;
    
};

#endif
