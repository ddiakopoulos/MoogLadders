#include <iostream>

#include "ImprovedMoog.h"

ImprovedMoog::ImprovedMoog() {
	
	DBG(__PRETTY_FUNCTION__); 
	
	ladder = new Ladder(); 

	ladder->V1prev  = 0.0;
	ladder->V2prev  = 0.0;
	ladder->V3prev  = 0.0;
	ladder->V4prev  = 0.0;
	ladder->tV1prev = 0.0;
	ladder->tV2prev = 0.0;
	ladder->tV3prev = 0.0;
	ladder->tV4prev = 0.0;
	ladder->dV1prev = 0.0;
	ladder->dV2prev = 0.0;
	ladder->dV3prev = 0.0;
	ladder->dV4prev = 0.0;

	_cutoff = 100;
	_resonance = .05;
	_drive = 0.05; 

}

ImprovedMoog::~ImprovedMoog() {
	
}

void ImprovedMoog::computeCutoff(float cut) {
	
	_cutoff = cut; 

	_x = M_PI * _cutoff / 44100;
	_g = 4.0 * M_PI * VT * _cutoff * (1.0 - _x) / (1.0 + _x);
	
	
}

void ImprovedMoog::computeResonance(float r) {
	
	_resonance = r; 
	
}


void ImprovedMoog::processSamples(float *samples, int numSamples) throw() {

	for (uint32_t i = 0; i < numSamples; i++) {

		const float in = samples[i]; 

		double dV1 = -_g * (tanh((_drive * in[i] + _resonance * ladder->V4prev) / (2.0 * VT)) + ladder->tV1prev);
		ladder->V1prev += (dV1 + ladder->dV1prev) / (2.0 * 44100);
		ladder->dV1prev = dV1;
		ladder->tV1prev = tanh(ladder->V1prev / (2.0 * VT));

		double dV2 = _g * (ladder->tV1prev - ladder->tV2prev);
		ladder->V2prev += (dV2 + ladder->dV2prev) / (2.0 * 44100);
		ladder->dV2prev = dV2;
		ladder->tV2prev = tanh(ladder->V2prev / (2.0 * VT));

		double dV3 = _g * (ladder->tV2prev - ladder->tV3prev);
		ladder->V3prev += (dV3 + ladder->dV3prev) / (2.0 * 44100);
		ladder->dV3prev = dV3;
		ladder->tV3prev = tanh(ladder->V3prev / (2.0 * VT));

		double dV4 = _g * (ladder->tV3prev - ladder->tV4prev);
		ladder->V4prev += (dV4 + ladder->dV4prev) / (2.0 * 44100);
		ladder->dV4prev = dV4;
		ladder->tV4prev = tanh(ladder->V4prev / (2.0 * VT));

		// Output
		samples[i] = ladder->V4prev;

	}

}

