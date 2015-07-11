/*
Copyright (c) 2015, Dimitri Diakopoulos All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <vector>
#include <stdint.h>
#include <exception>
#include <array>
#include <random>

#include "Util.h"
#include "Filters.h"

struct WhiteNoiseSource
{
	WhiteNoiseSource() : dist(-1, 1) {}
	std::mt19937 engine;
	std::uniform_real_distribution<float> dist;
};

// Full spectrum noise
struct WhiteNoise : public WhiteNoiseSource
{
	float operator()() { return dist(engine); }
};

// Pink noise has a decrease of 3dB/Octave
struct PinkNoise : public WhiteNoiseSource
{
	float operator()() { return f.process(dist(engine)); }
	PinkingFilter f;
};

 // Brown noise has a decrease of 6dB/Octave
struct BrownNoise : public WhiteNoiseSource
{
	float operator()() { return f.process(dist(engine)); }
	BrowningFilter f;
};

// Note! This noise is only valid for 44100 because of the hard-coded filter coefficients
struct NoiseGenerator
{
	enum NoiseType
	{
		WHITE,
		PINK,
		BROWN,
	};
	
	std::vector<float> produce(NoiseType t, int sampleRate, int channels, float seconds)
	{
		int samplesToGenerate = sampleRate * seconds * channels;
		std::vector<float> samples;
		samples.resize(samplesToGenerate);
		
		switch (t)
		{
			case NoiseType::WHITE:
			{
				WhiteNoise n;
				for(int s = 0; s < samplesToGenerate; s++)
				{
					samples[s] = n();
				}
			} break;
			case NoiseType::PINK:
			{
				PinkNoise n;
				for(int s = 0; s < samplesToGenerate; s++)
				{
					samples[s] = n();
				}
			} break;
			case NoiseType::BROWN:
			{
				BrownNoise n;
				for(int s = 0; s < samplesToGenerate; s++)
				{
					samples[s] = n();
				}
			} break;
			default:
				throw std::runtime_error("Invalid noise type");
		}
		return samples;
	}
	
};

#endif
