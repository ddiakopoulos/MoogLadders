#include <iostream>

#include "SimplifiedMoog.h"

SimplifiedMoog::SimplifiedMoog() {
	
	DBG(__PRETTY_FUNCTION__); 
	
	// To keep the overall level approximately constant, comp should be set
	// to 0.5 resulting in a 6 dB passband gain decrease at the maximum resonance 
	// (compared to a 12 dB decrease in the original Moog model
	gain_compensation = 0.5; 
	
	_resonance = 0.10f;
	_cutoff = 1000.0f;
	
	for (int i = 0; i < 4; i++) {
		_stage[i] = 0.0;
		_stageZ1[i] = 0.0;
	}
	
	computeCutoff(1000);
	computeResonance(0.1f);
	
}

SimplifiedMoog::~SimplifiedMoog() {
	
}


void SimplifiedMoog::computeCutoff(float cut) {
	
	_cutoff = cut; 
	
	float fs = 44100;
	float x2 = 2*fs; 
 
	// Normalized cutoff [0, 1] in radians: ((2*pi) * cutoff / samplerate)
	_g = (2 * M_PI) * _cutoff / x2; // feedback coefficient at fs*2 because of doublesampling
	
	// FIR part with gain g
	_h = _g / 1.3;
	_h0 = _g * 0.3 / 1.3;
	
	
}

void SimplifiedMoog::computeResonance(float r) {
	
	// Direct mapping (no table or polynomial)
	_resonance = r; 
	
}

/* This system is nonlinear so we are probably going to create a signal with components that exceed nyquist.
 * To prevent aliasing distortion, we need to oversample this processing chunk. Where do these extra samples 
 * come from? Todo! We can use polynomial interpolation to generate the extra samples, but this is expensive.
 * The cheap solution is to zero-stuff the incoming sample buffer. 

 * With resampling, numSamples should be 2x the frame size of the existing sample rate.
 * The output of this filter needs to be run through a decimator to return to the original SR.
 */  

void SimplifiedMoog::processSamples(float *samples, int numSamples) throw() {
	
	// Processing still happens at sample rate, but we have
	for (int samp = 0; samp < numSamples; ++samp) {
		
		for (int stage = 0; stage < 4; ++stage) {
			
			// Stage 1, 2, 3
			if (stage) {
				_input = _stage[stage-1];
				_stageTANH[stage-1] = tanh(_input);
				_stage[stage] = (_h * _stageZ1[stage] + _h0 * _stageTANH[stage-1]) + (1.0 - _g) * (stage != 3 ? _stageTANH[stage] : tanh(_stageZ1[stage]));
			}

			// New input, stage 0 
			else {
				_input = samples[samp] - ((4.0 * _resonance) * (_output - gain_compensation * samples[samp]));
				_stage[stage] = (_h * tanh(_input) + _h0 * _stageZ1[stage]) + (1.0 - _g) * _stageTANH[stage]; 
			}
			
			_stageZ1[stage] = _stage[stage];
									   
		}
			
		_output  = _stage[3];
		
		SNAP_TO_ZERO (_output);
		
		samples[samp] = _output;

	}
	
}
