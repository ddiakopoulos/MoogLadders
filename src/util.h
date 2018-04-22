#pragma once

#ifndef MOOG_UTIL_H
#define MOOG_UTIL_H

#include <cmath>
#include <stdint.h>

#define MOOG_E         2.71828182845904523536028747135266250
#define MOOG_LOG2E     1.44269504088896340735992468100189214
#define MOOG_LOG10E    0.434294481903251827651128918916605082
#define MOOG_LN2       0.693147180559945309417232121458176568
#define MOOG_LN10      2.30258509299404568401799145468436421
#define MOOG_PI        3.14159265358979323846264338327950288
#define MOOG_PI_2      1.57079632679489661923132169163975144
#define MOOG_PI_4      0.785398163397448309615660845819875721
#define MOOG_1_PI      0.318309886183790671537767526745028724
#define MOOG_2_PI      0.636619772367581343075535053490057448
#define MOOG_2_SQRTPI  1.12837916709551257389615890312154517
#define MOOG_SQRT2     1.41421356237309504880168872420969808
#define MOOG_SQRT1_2   0.707106781186547524400844362104849039
#define MOOG_INV_PI_2  0.159154943091895

#define NO_COPY(C) C(const C &) = delete; C & operator = (const C &) = delete
#define NO_MOVE(C) NO_COPY(C); C(C &&) = delete; C & operator = (const C &&) = delete

#define SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;

// Linear interpolation, used to crossfade a gain table
inline float moog_lerp(float amount, float a, float b)
{
	return (1.0f - amount) * a + amount * b;
}

inline float moog_min(float a, float b)
{
	a = b - a;
	a += fabs(a);
	a *= 0.5f;
	a = b - a;
	return a;
}

// Clamp without branching
// If input - _limit < 0, then it really substracts, and the 0.5 to make it half the 2 inputs.
// If > 0 then they just cancel, and keeps input normal.
// The easiest way to understand it is check what happends on both cases.
inline float moog_saturate(float input)
{
	float x1 = fabs(input + 0.95f);
	float x2 = fabs(input - 0.95f);
	return 0.5f * (x1 - x2);
}

// Imitate the (tanh) clipping function of a transistor pair.
// to 4th order, tanh is x - x*x*x/3; this cubic's
// plateaus are at +/- 1 so clip to 1 and evaluate the cubic.
// This is pretty coarse - for instance if you clip a sinusoid this way you
// can sometimes hear the discontinuity in 4th derivative at the clip point
inline float clip(float value, float saturation, float saturationinverse)
{
	float v2 = (value * saturationinverse > 1 ? 1 :
				(value * saturationinverse < -1 ? -1:
				 value * saturationinverse));
	return (saturation * (v2 - (1./3.) * v2 * v2 * v2));
}

#define HZ_TO_RAD(f) (MOOG_PI_2 * f)
#define RAD_TO_HZ(omega) (MOOG_INV_PI_2 * omega)

#ifdef __GNUC__
	#define ctz(N) __builtin_ctz(N)
#else
	template<typename T>
	inline int ctz(T x)
	{
		int p, b;
		for (p = 0, b = 1; !(b & x); b <<= 1, ++p)
			;
		return p;
	}
#endif

inline double fast_tanh(double x) 
{
	double x2 = x * x;
	return x * (27.0 + x2) / (27.0 + 9.0 * x2);
}

#endif
