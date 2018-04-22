// This file is unlicensed and uncopyright as found at:
// http://www.musicdsp.org/showone.php?id=24
// Considering how widely this same code has been used in ~100 projects on GitHub with 
// various licenses, it might be reasonable to suggest that the license is CC-BY-SA

#pragma once

#ifndef MUSICDSP_MOOG_H
#define MUSICDSP_MOOG_H

#include "LadderFilterBase.h"
#include "Util.h"

class MusicDSPMoog : public LadderFilterBase
{
	
public:
	
	MusicDSPMoog(float sampleRate) : LadderFilterBase(sampleRate)
	{
		memset(stage, 0, sizeof(stage));
		memset(delay, 0, sizeof(delay));
		SetCutoff(1000.0f);
		SetResonance(0.10f);
	}
	
	virtual ~MusicDSPMoog()
	{

	}
	
	virtual void Process(float * samples, uint32_t n) override
	{
		for (int s = 0; s < n; ++s)
		{
			float x = samples[s] - resonance * stage[3];

			// Four cascaded one-pole filters (bilinear transform)
			stage[0] = x * p + delay[0]  * p - k * stage[0];
			stage[1] = stage[0] * p + delay[1] * p - k * stage[1];
			stage[2] = stage[1] * p + delay[2] * p - k * stage[2];
			stage[3] = stage[2] * p + delay[3] * p - k * stage[3];
		
			// Clipping band-limited sigmoid
			stage[3] -= (stage[3] * stage[3] * stage[3]) / 6.0;
			
			delay[0] = x;
			delay[1] = stage[0];
			delay[2] = stage[1];
			delay[3] = stage[2];

			samples[s] = stage[3];
		}
	}
	
	virtual void SetResonance(float r) override
	{
		resonance = r * (t2 + 6.0 * t1) / (t2 - 6.0 * t1);
	}
	
	virtual void SetCutoff(float c) override
	{
		cutoff = 2.0 * c / sampleRate;

		p = cutoff * (1.8 - 0.8 * cutoff);
		k = 2.0 * sin(cutoff * MOOG_PI * 0.5) - 1.0;
		t1 = (1.0 - p) * 1.386249;
		t2 = 12.0 + t1 * t1;

		SetResonance(resonance);
	}
	
private:
	
	double stage[4];
	double delay[4];

	double p;
	double k;
	double t1;
	double t2;

};

#endif
