#pragma once

#ifndef KRAJESKI_LADDER_H
#define KRAJESKI_LADDER_H

#include "LadderFilterBase.h"
#include "Util.h"

/*
This class implements Tim Stilson's MoogVCF filter
using 'compromise' poles at z = -0.3

Several improments are built in, such as corrections
for cutoff and resonance parameters, removal of the
necessity of the separation table, audio rate update
of cutoff and resonance and a smoothly saturating
tanh() function, clamping output and creating inherent
nonlinearities.

This code is Unlicensed (i.e. public domain); in an email exchange on
4.21.2018 Aaron Krajeski stated: "That work is under no copyright. 
You may use it however you might like."

Source: http://song-swap.com/MUMT618/aaron/Presentation/demo.html
*/

class KrajeskiMoog final : public LadderFilterBase
{
	
public:
	
    KrajeskiMoog(float sampleRate) : LadderFilterBase(sampleRate)
	{
		memset(state, 0, sizeof(state));
		memset(delay, 0, sizeof(delay));
		
		drive = 1.0;
		gComp = 1.0;
		
		SetCutoff(1000.0f);
		SetResonance(0.1f);
	}
	
	virtual ~KrajeskiMoog() { }
	
	virtual void Process(float * samples, const uint32_t n) override
	{
		for (int s = 0; s < n; ++s)
		{
			state[0] = tanh(drive * (samples[s] - 4 * gRes * (state[4] - gComp * samples[s])));
			
			for(int i = 0; i < 4; i++)
			{
				state[i+1] = g * (0.3 / 1.3 * state[i] + 1 / 1.3 * delay[i] - state[i + 1]) + state[i + 1];
				delay[i] = state[i];
			}
			samples[s] = state[4];
		}
	}
	
	virtual void SetResonance(float r) override
	{
		resonance = r;
		gRes = resonance * (1.0029 + 0.0526 * wc - 0.926 * pow(wc, 2) + 0.0218 * pow(wc, 3));
	}
	
	virtual void SetCutoff(float c) override
	{
		cutoff = c;
		wc = 2 * MOOG_PI * cutoff / sampleRate;
		g = 0.9892 * wc - 0.4342 * pow(wc, 2) + 0.1381 * pow(wc, 3) - 0.0202 * pow(wc, 4);
	}
	
private:
	
	double state[5];
	double delay[5];
	double wc; // The angular frequency of the cutoff.
	double g; // A derived parameter for the cutoff frequency
	double gRes; // A similar derived parameter for resonance.
	double gComp; // Compensation factor.
	double drive; // A parameter that controls intensity of nonlinearities.
	
};

#endif
