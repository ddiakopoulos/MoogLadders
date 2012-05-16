#include "Huovilainen.h"

HuovilainenMoog::HuovilainenMoog() {
    
    DBG(__PRETTY_FUNCTION__); 
    
    _resonance = 0.10f;
    _resonanceQuad = 0.0f;
    
    _cutoff = 1000.0f;
    _acr = 0.0f;
    _tune = 0.0f;
    
    float V = 1.22070313; 
    _thermal = (1.0 / V);	

    for (int i = 0; i < 4; i++) {
        _stage[i] = 0.0;
        _stageZ1[i] = 0.0;
    }
        
    for (int i = 0; i < 3; i++)
        _stageTANH[i] = 0.0;

    computeCutoff(1000);
    computeResonance(0.1f);
    
}

HuovilainenMoog::~HuovilainenMoog() {
    
}


void HuovilainenMoog::computeCutoff(float cut) {
    
    _cutoff = cut;
    
    // Normalized Cutoff
    double fc =  (double)(_cutoff / 44100);
    
    double x_2 = fc/2;
    double x2 = fc*fc;
    double x3 = fc*x2;
    
    // Frequency & amplitude correction
    double fcr = 1.8730 * x3 + 0.4955 * x2 - 0.6490 * fc + 0.9988;
    
    // Resonance compensation 
    _acr = -3.9364 * x2 + 1.8409 * fc + 0.9968;
    
    // Normal scaled impulse invariant transformed one-pole filter; exp() models resonance
    // The coefficient g determines the cutoff frequency
    _tune = (1.0 - exp(-((2*M_PI)*x_2*fcr))) / _thermal; // Huovilainen
    
    computeResonance(_resonance);

}

void HuovilainenMoog::computeResonance(float r) {
    
    if (r < 0) 
        _resonance = 0;
    else 
        _resonance = r;
    
    _resonanceQuad = (4.0 * (double) _resonance * _acr); // (Modified Huovilainen Fig 23)		

}

void HuovilainenMoog::setTransistorVoltage(const float &V) {
    
    // Base-emitter voltages of the transistors
    _thermal = (1.0 / V); 
    
    computeCutoff(1000);
    computeResonance(0.1f);
    
}

void HuovilainenMoog::processSamples(float *samples, int numSamples) throw() {

    // 2v = thermal 
    
    double local_output; 
    
    for (int samp = 0; samp < numSamples; ++samp) {
        
        // 2x oversampling
        for (int j = 0; j < 2; ++j) {
            
            // Filter stages (Huovilainen Fig 22)		
            for (int stage = 0; stage < 4; ++stage) {
                
                // Stages 1, 2, 3
                if (stage) {
                    _input = _stage[stage-1];
                    _stageTANH[stage-1] = tanh(_input*_thermal); 
                    _stage[stage] = _stageZ1[stage] + _tune * (_stageTANH[stage-1]) - (stage != 3 ? _stageTANH[stage] : tanh(_stageZ1[stage]*_thermal));
                }
                
                // New input, stage 0
                else {
                    _input = samples[samp] - _resonanceQuad * _output;
                    _stage[stage] = _stageZ1[stage] + _tune * (tanh(_input*_thermal) - _stageTANH[stage]); 
                }
                
                _stageZ1[stage] = _stage[stage];
                
            }
            
            // .5 sample delay for phase compensation 
            // This can be realized by averaging two samples.
            local_output = (_stage[3] + _last_stage) * 0.5;
            _last_stage = _stage[3];
        
        }
        
        _output = local_output; 
        
        SNAP_TO_ZERO (_output);
        
        samples[samp] = _output;
        
    }
    
}



