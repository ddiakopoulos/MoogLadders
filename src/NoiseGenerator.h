#pragma comment(user, "license")

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

//@todo: pass samplerate down into filters

struct Noise
{
	Noise()
	{
		dist = std::uniform_real_distribution<float>(-1, 1);
	}
    std::mt19937 engine;
	std::uniform_real_distribution<float> dist;
};

struct WhiteNoise : public Noise
{
    float operator()()
    {
		return dist(engine);
    }
};

// Pink noise has a decrease of 3dB/Octave
struct PinkNoise : public Noise
{
    float operator()()
    {
		auto w = dist(engine);
		auto what = f.process(w);
        return what;
    }

	PinkingFilter f;
};

 // Brown noise has a decrease of 6dB/Octave
struct BrownNoise : public Noise
{
	BrownNoise()
	{
		f.SetCutoff(22050);
		f.SetQValue(0.1f);
	}
    float operator()()
    {
		auto w = dist(engine);
		return f.Tick(w);
    }
	RBJFilter f;
};

struct NoiseGenerator
{
    enum NoiseType
    {
        WHITE,
        PINK,
        BROWN,
    };
    
    NoiseGenerator(){}
    ~NoiseGenerator(){}
    
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
