#ifndef dafx_Stilson_h 
#define dafx_Stilson_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "LadderFilter.h"

// Great discussion on oversampling:
// http://music.columbia.edu/pipermail/music-dsp/2005-February/062778.html
// Also see:
//http://www.synthmaker.co.uk/dokuwiki/doku.php?id=tutorials:oversampling

/* A digital model of the Moog filter was first presented by Stilson and Smith [10].
 As in the analog prototype, it has four one-pole filters in series, and a global 
 feedback is used to produce the resonance. To realize the filter, a unit delay 
 has to be inserted in the feedback path, but this couples the cutoff and the 
 resonance controls. Various ways of compensation have been examined, 
 with the “compromise” version [10] being the most attractive. The “compromise” 
 version inserts a zero at z = −0.3 inside each one-pole section, 
 thus mostly decoupling the resonance and the cutoff parameters. While the Stilson 
 and Smith Moog model is certainly useful and solves the problem of fast 
 coefficient update, it becomes unstable with very large resonance values 
 and it cannot self-oscillate. Furthermore, it does not emulate the 
 characteristic distortion produced by the original transistor ladder circuit.
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
