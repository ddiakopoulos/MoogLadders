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

struct PinkNoise
{
    float operator()()
    {
        return 0;
    }
};

struct WhiteNoise
{
    float operator()()
    {
        return 0;
    }
};

struct BrownNoise
{
    float operator()()
    {
        return 0;
    }
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
