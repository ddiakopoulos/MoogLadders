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
			for(int s = 0; s < samplesToGenerate; s++) samples[s] = n();
		} break;
		case NoiseType::PINK:
		{
			PinkNoise n;
			for(int s = 0; s < samplesToGenerate; s++) samples[s] = n();
		} break;
		case NoiseType::BROWN:
		{
			BrownNoise n;
			for(int s = 0; s < samplesToGenerate; s++) samples[s] = n();
		} break;
		default: throw std::runtime_error("Invalid noise type");
		}
		return samples;
	}
	
};

#endif
