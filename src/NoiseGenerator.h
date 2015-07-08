#pragma comment(user, "license")

// Partially adapted from LabSound:
// Copyright (c) 2003-2015 Nick Porcino, All rights reserved. MIT Licensed.

#pragma once

#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <vector>
#include <stdint.h>
#include <exception>

class NoiseGenerator
{
public:
    
    enum NoiseType
    {
        WHITE,
        PINK
    };
    
    NoiseGenerator()
    {
        
    }
    
    ~NoiseGenerator()
    {
        
    }
    
    std::vector<float> produce(NoiseType t, int sampleRate, float seconds)
    {
        int samplesToGenerate = sampleRate * seconds;
        std::vector<float> samples;
        samples.resize(samplesToGenerate);
        
        switch (t)
        {
            case NoiseType::WHITE:
                for(int s = 0; s < samplesToGenerate; s++)
                {
                    float white = ((float)((whiteSeed & 0x7fffffff) - 0x40000000)) * (float)(1.0f / 0x40000000);
                    white = (white * 0.5f) - 1.0f;
                    samples[s] = white;
                    whiteSeed = whiteSeed * 435898247 + 382842987;
                }
                break;
                
            case NoiseType::PINK:
                // ToDo
                break;
                
            default:
                throw std::runtime_error("Invalid noise type");
        }
        return samples;
    }
    
private:
    
    uint32_t whiteSeed = 1489853723;
    
};

#endif
