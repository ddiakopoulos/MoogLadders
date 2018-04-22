// Based on implementation in CSound5 (LGPLv2.1)
// https://github.com/csound/csound/blob/develop/COPYING

#pragma once

#ifndef HUOVILAINEN_LADDER_H
#define HUOVILAINEN_LADDER_H

#include "LadderFilterBase.h"

/*
Huovilainen developed an improved and physically correct model of the Moog
Ladder filter that builds upon the work done by Smith and Stilson. This model
inserts nonlinearities inside each of the 4 one-pole sections on account of the
smoothly saturating function of analog transistors. The base-emitter voltages of
the transistors are considered with an experimental value of 1.22070313 which
maintains the characteristic sound of the analog Moog. This model also permits
self-oscillation for resonances greater than 1. The model depends on five
hyperbolic tangent functions (tanh) for each sample, and an oversampling factor
of two (preferably higher, if possible). Although a more faithful
representation of the Moog ladder, these dependencies increase the processing
time of the filter significantly. Lastly, a half-sample delay is introduced for 
phase compensation at the final stage of the filter. 

References: Huovilainen (2004), Huovilainen (2010), DAFX - Zolzer (ed) (2nd ed)
Original implementation: Victor Lazzarini for CSound5

Considerations for oversampling: 
http://music.columbia.edu/pipermail/music-dsp/2005-February/062778.html
http://www.synthmaker.co.uk/dokuwiki/doku.php?id=tutorials:oversampling
*/ 

class HuovilainenMoog : public LadderFilterBase
{
public:
	
	HuovilainenMoog(float sampleRate) : LadderFilterBase(sampleRate), thermal(0.000025)
	{
		memset(stage, 0, sizeof(stage));
		memset(delay, 0, sizeof(delay));
		memset(stageTanh, 0, sizeof(stageTanh));
		SetCutoff(1000.0f);
		SetResonance(0.10f);
	}
	
	virtual ~HuovilainenMoog()
	{
		
	}
	
	virtual void Process(float * samples, uint32_t n) override
	{
		for (int s = 0; s < n; ++s)
		{
			// Oversample
			for (int j = 0; j < 2; j++) 
			{
				float input = samples[s] - resQuad * delay[5];
				delay[0] = stage[0] = delay[0] + tune * (tanh(input * thermal) - stageTanh[0]);
				for (int k = 1; k < 4; k++) 
				{
					input = stage[k-1];
					stage[k] = delay[k] + tune * ((stageTanh[k-1] = tanh(input * thermal)) - (k != 3 ? stageTanh[k] : tanh(delay[k] * thermal)));
					delay[k] = stage[k];
				}
				// 0.5 sample delay for phase compensation
				delay[5] = (stage[3] + delay[4]) * 0.5;
				delay[4] = stage[3];
			}
			samples[s] = delay[5];
		}

	}
	
	virtual void SetResonance(float r) override
	{
		resonance = r;
		resQuad = 4.0 * resonance * acr;
	}
	
	virtual void SetCutoff(float c) override
	{
		cutoff = c;

		double fc =  cutoff / sampleRate;
		double f  =  fc * 0.5; // oversampled 
		double fc2 = fc * fc;
		double fc3 = fc * fc * fc;

		double fcr = 1.8730 * fc3 + 0.4955 * fc2 - 0.6490 * fc + 0.9988;
		acr = -3.9364 * fc2 + 1.8409 * fc + 0.9968;

		tune = (1.0 - exp(-((2 * MOOG_PI) * f * fcr))) / thermal; 

		SetResonance(resonance);
	}
	
private:
	
	double stage[4];
	double stageTanh[3];
	double delay[6];

	double thermal;
	double tune;
	double acr;
	double resQuad;
	
}; 

#endif
