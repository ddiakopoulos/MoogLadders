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
		bCoef = {{0.0f, 0.0f, 0.0f}};
		aCoef = {{0.0f, 0.0f}};
		w = {{0.0f, 0.0f}};
	}
	
	~BiQuadBase()
	{

	}
	
	// DF-II impl
	void Process(float * samples, const uint32_t n)
	{
		float out = 0;
		for (int s = 0; s < n; ++s)
		{
			out = bCoef[0] * samples[s] + w[0];
			w[0] = bCoef[1] * samples[s] - aCoef[0] * out + w[1];
			w[1] = bCoef[2] * samples[s] - aCoef[1] * out;
			samples[s] = out;
		}
	}

	float Tick(float s)
	{
		float out = bCoef[0] * s + w[0];
		w[0] = bCoef[1] * s - aCoef[0] * out + w[1];
		w[1] = bCoef[2] * s - aCoef[1] * out;
		return out;
	}

	void SetBiquadCoefs(std::array<float, 3> b, std::array<float, 2> a)
	{
		bCoef = b;
		aCoef = a;
	}
	
protected:
	std::array<float, 3> bCoef; // b0, b1, b2
	std::array<float, 2> aCoef; // a1, a2
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
		Q = 1;
		A = 1;

		a = {{0.0f, 0.0f, 0.0f}};
		b = {{0.0f, 0.0f, 0.0f}};

		SetCutoff(cutoff);
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
			} break;
				
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
		
		std::array<float, 2> aNorm;
		std::array<float, 3> bNorm;

		aNorm[0] = a[1] * factor;
		aNorm[1] = a[2] * factor;

		bNorm[0] = b[0] * factor;
		bNorm[1] = b[1] * factor;
		bNorm[2] = b[2] * factor;
		
		SetBiquadCoefs(bNorm, aNorm);
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

	void SetType(FilterType newType)
	{
		t = newType;
		UpdateCoefficients();
	}
	
	FilterType GetType()
	{
		return t;
	}
	
private:

	float sampleRate;
	
	float omega;
	float cosOmega;
	float sinOmega;
	
	float Q;
	float alpha;
	float A;

	std::array<float, 3> a;
	std::array<float, 3> b;
	
	FilterType t;
};

// +/-0.05dB above 9.2Hz @ 44,100Hz
class PinkingFilter
{
	double b0, b1, b2, b3, b4, b5, b6;
public:
	PinkingFilter() : b0(0), b1(0), b2(0), b3(0), b4(0), b5(0), b6(0) {}
	float process(const float s)
	{
		b0 = 0.99886 * b0 + s * 0.0555179;
		b1 = 0.99332 * b1 + s * 0.0750759;
		b2 = 0.96900 * b2 + s * 0.1538520;
		b3 = 0.86650 * b3 + s * 0.3104856;
		b4 = 0.55000 * b4 + s * 0.5329522;
		b5 = -0.7616 * b5 - s * 0.0168980;
		const double pink = (b0 + b1 + b2 + b3 + b4 + b5 + b6 + (s * 0.5362)) * 0.11;
		b6 = s * 0.115926;
		return pink;
	}
};

class BrowningFilter
{
float l;
public:
	BrowningFilter() : l(0) {}
	float process(const float s)
	{
		float brown = (l + (0.02f * s)) / 1.02f;
		l = brown;
		return brown * 3.5f; // compensate for gain
	}
};

#endif
