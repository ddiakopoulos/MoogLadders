/* -------------------------------------------------------------------------
 * This source code is provided without any warranties as published in 
 * DAFX book 2nd edition, copyright Wiley & Sons 2011, available at 
 * http://www.dafx.de. It may be used for educational purposes and not 
 * for commercial applications without further permission.
 * -------------------------------------------------------------------------
 */

#pragma once

#ifndef SIMPLIFIED_LADDER_H
#define SIMPLIFIED_LADDER_H

#include "LadderFilterBase.h"

/*
The simplified nonlinear Moog filter is based on the full Huovilainen model,
with five nonlinear (tanh) functions (4 first-order sections and a feedback).
Like the original, this model needs an oversampling factor of at least two when
these nonlinear functions are used to reduce possible aliasing. This model
maintains the ability to self oscillate when the feedback gain is >= 1.0.

References: DAFX - Zolzer (ed) (2nd ed)
Original implementation: Valimaki, Bilbao, Smith, Abel, Pakarinen, Berners (DAFX)
This is a transliteration into C++ of the original matlab source (moogvcf.m)

Considerations for oversampling:
http://music.columbia.edu/pipermail/music-dsp/2005-February/062778.html
http://www.synthmaker.co.uk/dokuwiki/doku.php?id=tutorials:oversampling
*/

class SimplifiedMoog : public LadderFilterBase
{
public:
	
	SimplifiedMoog(float sampleRate) : LadderFilterBase(sampleRate)
	{
		// To keep the overall level approximately constant, comp should be set
		// to 0.5 resulting in a 6 dB passband gain decrease at the maximum resonance
		// (compared to a 12 dB decrease in the original Moog model
		gainCompensation = 0.5;
		
		memset(stage, 0, sizeof(stage));
		memset(stageZ1, 0, sizeof(stageZ1));
		memset(stageTanh, 0, sizeof(stageTanh));
		
		SetCutoff(1000.0f);
		SetResonance(0.10f);
	}
	
	virtual ~SimplifiedMoog()
	{
		
	}
	
	// This system is nonlinear so we are probably going to create a signal with components that exceed nyquist.
	// To prevent aliasing distortion, we need to oversample this processing chunk. Where do these extra samples
	// come from? Todo! We can use polynomial interpolation to generate the extra samples, but this is expensive.
	// The cheap solution is to zero-stuff the incoming sample buffer.
	// With resampling, numSamples should be 2x the frame size of the existing sample rate.
	// The output of this filter needs to be run through a decimator to return to the original samplerate.
	virtual void Process(float * samples, uint32_t n) override
	{
		// Processing still happens at sample rate...
		for (int s = 0; s < n; ++s)
		{
			for (int stageIdx = 0; stageIdx < 4; ++stageIdx)
			{
				if (stageIdx)
				{
					input = stage[stageIdx-1];
					stageTanh[stageIdx-1] = tanh(input);
					stage[stageIdx] = (h * stageZ1[stageIdx] + h0 * stageTanh[stageIdx-1]) + (1.0 - g) * (stageIdx != 3 ? stageTanh[stageIdx] : tanh(stageZ1[stageIdx]));
				}
				else
				{
					input = samples[s] - ((4.0 * resonance) * (output - gainCompensation * samples[s]));
					stage[stageIdx] = (h * tanh(input) + h0 * stageZ1[stageIdx]) + (1.0 - g) * stageTanh[stageIdx];
				}
				
				stageZ1[stageIdx] = stage[stageIdx];
			}
			
			output = stage[3];
			SNAP_TO_ZERO(output);
			samples[s] = output;
		}
	}
	
	virtual void SetResonance(float r) override
	{
		resonance = r;
	}
	
	virtual void SetCutoff(float c) override
	{
		cutoff = c;
		
		// Not being oversampled at the moment... * 2 when functional
		float fs2 = sampleRate;
		
		// Normalized cutoff [0, 1] in radians: ((2*pi) * cutoff / samplerate)
		g = (2 * MOOG_PI) * cutoff / fs2; // feedback coefficient at fs*2 because of doublesampling
		g *= MOOG_PI / 1.3; // correction factor that allows _cutoff to be supplied Hertz
		
		// FIR part with gain g
		h = g / 1.3;
		h0 = g * 0.3 / 1.3;
	}
	
private:
	
	double output;
	double lastStage;
	
	double stage[4];
	double stageZ1[4];
	double stageTanh[3];
	
	double input;
	double h;
	double h0;
	double g;
	
	float gainCompensation;
};

#endif
