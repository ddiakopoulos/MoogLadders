// Copyright (c) 2003, David Lowenfels
// Released as the moog~ pd extern.

// This code is Unlicensed (i.e. public domain); in an email exchange on
// 6.14.2019 David Lowenfels stated "You're welcome to use the Moog~ code, 
// license it free as in beer or whatever :)"

#pragma once

#ifndef STILSON_LADDER_H
#define STILSON_LADDER_H

#include "LadderFilterBase.h"

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
Original implementation: Tim Stilson, David Lowenfels
*/

static float S_STILSON_GAINTABLE[199] =
{
	0.999969, 0.990082, 0.980347, 0.970764, 0.961304, 0.951996, 0.94281, 0.933777, 0.924866, 0.916077,
	0.90741, 0.898865, 0.890442, 0.882141 , 0.873962, 0.865906, 0.857941, 0.850067, 0.842346, 0.834686,
	0.827148, 0.819733, 0.812378, 0.805145, 0.798004, 0.790955, 0.783997, 0.77713, 0.770355, 0.763672,
	0.75708 , 0.75058, 0.744141, 0.737793, 0.731537, 0.725342, 0.719238, 0.713196, 0.707245, 0.701355,
	0.695557, 0.689819, 0.684174, 0.678558, 0.673035, 0.667572, 0.66217, 0.65686, 0.651581, 0.646393,
	0.641235, 0.636169, 0.631134, 0.62619, 0.621277, 0.616425, 0.611633, 0.606903, 0.602234, 0.597626,
	0.593048, 0.588531, 0.584045, 0.579651, 0.575287 , 0.570953, 0.566681, 0.562469, 0.558289, 0.554169,
	0.550079, 0.546051, 0.542053, 0.538116, 0.53421, 0.530334, 0.52652, 0.522736, 0.518982, 0.515289,
	0.511627, 0.507996 , 0.504425, 0.500885, 0.497375, 0.493896, 0.490448, 0.487061, 0.483704, 0.480377,
	0.477081, 0.473816, 0.470581, 0.467377, 0.464203, 0.46109, 0.457977, 0.454926, 0.451874, 0.448883,
	0.445892, 0.442932, 0.440033, 0.437134, 0.434265, 0.431427, 0.428619, 0.425842, 0.423096, 0.42038,
	0.417664, 0.415009, 0.412354, 0.409729, 0.407135, 0.404572, 0.402008, 0.399506, 0.397003, 0.394501,
	0.392059, 0.389618, 0.387207, 0.384827, 0.382477, 0.380127, 0.377808, 0.375488, 0.37323, 0.370972,
	0.368713, 0.366516, 0.364319, 0.362122, 0.359985, 0.357849, 0.355713, 0.353607, 0.351532, 0.349457,
	0.347412, 0.345398, 0.343384, 0.34137, 0.339417, 0.337463, 0.33551, 0.333588, 0.331665, 0.329773,
	0.327911, 0.32605, 0.324188, 0.322357, 0.320557, 0.318756, 0.316986, 0.315216, 0.313446, 0.311707,
	0.309998, 0.308289, 0.30658, 0.304901, 0.303223, 0.301575, 0.299927, 0.298309, 0.296692, 0.295074,
	0.293488, 0.291931, 0.290375, 0.288818, 0.287262, 0.285736, 0.284241, 0.282715, 0.28125, 0.279755,
	0.27829, 0.276825, 0.275391, 0.273956, 0.272552, 0.271118, 0.269745, 0.268341, 0.266968, 0.265594,
	0.264252, 0.262909, 0.261566, 0.260223, 0.258911, 0.257599, 0.256317, 0.255035, 0.25375
};

class StilsonMoog : public LadderFilterBase
{
public:
	
	StilsonMoog(float sampleRate) : LadderFilterBase(sampleRate)
	{
		memset(state, 0, sizeof(state));
		SetCutoff(1000.0f);
		SetResonance(0.10f);
	}
	
	virtual ~StilsonMoog()
	{
		
	}
	
	virtual void Process(float * samples, uint32_t n) override
	{
		float localState;
		
		for (int s = 0; s < n; ++s)
		{
			// Scale by arbitrary value on account of our saturation function
			const float input = samples[s] * 0.65f;
			
			// Negative Feedback
			output = 0.25 * (input - output);
			
			for (int pole = 0; pole < 4; ++pole)
			{
				localState = state[pole];
				output = moog_saturate(output + p * (output - localState));
				state[pole] = output;
				output = moog_saturate(output + localState);
			}
			
			SNAP_TO_ZERO(output);
			samples[s] = output;
			output *= Q; // Scale stateful output by Q
		}
	}
	
	virtual void SetResonance(float r) override
	{
		r = moog_min(r, 1);
		resonance = r;
		
		double ix;
		double ixfrac;
		int ixint;
		
		ix = p * 99;
		ixint = floor(ix);
		ixfrac = ix - ixint;
		
		Q = r * moog_lerp(ixfrac, S_STILSON_GAINTABLE[ixint + 99], S_STILSON_GAINTABLE[ixint + 100]);
	}
	
	virtual void SetCutoff(float c) override
	{
		cutoff = c;
		
		// Normalized cutoff between [0, 1]
		double fc = (cutoff) / sampleRate;
		double x2 = fc * fc;
		double x3 = fc * fc * fc;
		
		// Frequency & amplitude correction (Cubic Fit)
		p = -0.69346 * x3 - 0.59515 * x2 + 3.2937 * fc - 1.0072;
		
		SetResonance(resonance);
	}
	
private:
	
	double p;
	double Q; 
	double state[4];
	double output; 
}; 

#endif
