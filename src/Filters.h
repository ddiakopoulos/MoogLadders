#pragma comment(user, "license")

#pragma once

#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>
#include <array>

#include "Util.h"

class BiQuadBase
{
    
public:
    
    BiQuadBase()
    {
		b = {{0.0f, 0.0f, 0.0f}};
		a = {{0.0f, 0.0f}};
		w = {{0.0f, 0.0f}};
    }
    
    ~BiQuadBase()
    {
        
    }
    
    // DF-II Implementation
    void Process(float * samples, const uint32_t n)
    {
		float out = 0;
        for (int s = 0; s < n; ++s)
        {
            out = b[0] * samples[s] + w[0];
			w[0] = b[1] * samples[s] - a[0] * out + w[1];
			w[1] = b[2] * samples[s] - a[1] * out;
			samples[s] = out;
        }
    }
    
protected:
    
    std::array<float, 3> b; // b0, b1, b2
    std::array<float, 2> a; // a1, a2
    std::array<float, 2> w; // delays
};

class RBJFilter : public BiQuadBase
{
public:
    
    enum FilterType
    {
        LOWPASS,
        HIGHPASS,
        BANDPASS,
        ALLPASS,
        NOTCH,
        PEAK,
        LOW_SHELF,
        HIGH_SHELF
    };
    
    RBJFilter(FilterType type = FilterType::LOWPASS, float cutoff = 1, float sampleRate = 44100) : sampleRate(sampleRate), t(type)
    {
        SetCutoff(cutoff);
        Q = 1;
        A = 1;
        dbGain = 0;
    }
    
    ~RBJFilter()
    {
        
    }
    
    void UpdateCoefficients()
    {
        cosOmega = cos(omega);
        sinOmega = sin(omega);
        
        switch (t)
        {
            case LOWPASS:
            {
                alpha = sinOmega / (2.0 * Q);
                b[0] = (1 - cosOmega) / 2;
                b[1] = 1 - cosOmega;
                b[2] = b[0];
                a[0] = 1 + alpha;
                a[1] = -2 * cosOmega;
                a[2] = 1 - alpha;
            }break;
                
            case HIGHPASS:
            {
                alpha = sinOmega / (2.0 * Q);
                b[0] = (1 + cosOmega) / 2;
                b[1] = -(1 + cosOmega);
                b[2] = b[0];
                a[0] = 1 + alpha;
                a[1] = -2 * cosOmega;
                a[2] = 1 - alpha;
            } break;
                
            case BANDPASS:
            {
                alpha = sinOmega * sinhf(logf(2.0) / 2.0 * Q * omega/sinOmega);
                b[0] = sinOmega / 2;
                b[1] = 0;
                b[2] = -b[0];
                a[0] = 1 + alpha;
                a[1] = -2 * cosOmega;
                a[2] = 1 - alpha;
            } break;
                
            case ALLPASS:
            {
                alpha = sinOmega / (2.0 * Q);
                b[0] = 1 - alpha;
                b[1] = -2 * cosOmega;
                b[2] = 1 + alpha;
                a[0] = b[2];
                a[1] = b[1];
                a[2] = b[0];
            } break;
                
            case NOTCH:
            {
                alpha = sinOmega * sinhf(logf(2.0) / 2.0 * Q * omega/sinOmega);
                b[0] = 1;
                b[1] = -2 * cosOmega;
                b[2] = 1;
                a[0] = 1 + alpha;
                a[1] = b[1];
                a[2] = 1 - alpha;
            } break;
                
            case PEAK:
            {
                alpha = sinOmega * sinhf(logf(2.0) / 2.0 * Q * omega/sinOmega);
                b[0] = 1 + (alpha * A);
                b[1] = -2 * cosOmega;
                b[2] = 1 - (alpha * A);
                a[0] = 1 + (alpha / A);
                a[1] = b[1];
                a[2] = 1 - (alpha / A);
            } break;
                
            case LOW_SHELF:
            {
                alpha = sinOmega / 2.0 * sqrt( (A + 1.0 / A) * (1.0 / Q - 1.0) + 2.0);
                b[0] = A * ((A + 1) - ((A - 1) * cosOmega) + (2 * sqrtf(A) * alpha));
                b[1] = 2 * A * ((A - 1) - ((A + 1) * cosOmega));
                b[2] = A * ((A + 1) - ((A - 1) * cosOmega) - (2 * sqrtf(A) * alpha));
                a[0] = ((A + 1) + ((A - 1) * cosOmega) + (2 * sqrtf(A) * alpha));
                a[1] = -2 * ((A - 1) + ((A + 1) * cosOmega));
                a[2] = ((A + 1) + ((A - 1) * cosOmega) - (2 * sqrtf(A) * alpha));
            } break;
                
            case HIGH_SHELF:
            {
                alpha = sinOmega / 2.0 * sqrt( (A + 1.0 / A) * (1.0 / Q - 1.0) + 2.0);
                b[0] = A * ((A + 1) + ((A - 1) * cosOmega) + (2 * sqrtf(A) * alpha));
                b[1] = -2 * A * ((A - 1) + ((A + 1) * cosOmega));
                b[2] = A * ((A + 1) + ((A - 1) * cosOmega) - (2 * sqrtf(A) * alpha));
                a[0] = ((A + 1) - ((A - 1) * cosOmega) + (2 * sqrtf(A) * alpha));
                a[1] = 2 * ((A - 1) - ((A + 1) * cosOmega));
                a[2] = ((A + 1) - ((A - 1) * cosOmega) - (2 * sqrtf(A) * alpha));
            } break;
        }
        
        // Normalize filter coefficients
        float factor = 1.0f / a[0];
        
        a[0] *= factor;
        a[1] *= factor;
        a[2] *= factor;
        
        b[0] *= factor;
        b[1] *= factor;
    }
    
    // In Hertz, 0 to Nyquist
    void SetCutoff(float c)
    {
        omega = HZ_TO_RAD(c) / sampleRate;
        UpdateCoefficients();
    }
    
    float GetCutoff()
    {
        return omega;
    }
    
    // Arbitrary, from 0.01f to ~20
    void SetQValue(float q)
    {
        Q = q;
        UpdateCoefficients();
    }
    
    float GetQValue()
    {
        return Q;
    }
    
private:
    
    float sampleRate;
    
    float omega;
    float cosOmega;
    float sinOmega;
    
    float Q;
    float alpha;
    float A;
    float dbGain;
    
    FilterType t;
    
};

#endif
