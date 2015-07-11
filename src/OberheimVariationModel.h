#pragma comment(user, "license")

#pragma once

#ifndef OBERHEIM_VARIATION_LADDER_H
#define OBERHEIM_VARIATION_LADDER_H

#include "LadderFilterBase.h"
#include "Util.h"

class VAOnePole
{
public:

    VAOnePole(float sr) : sampleRate(sr)
    {
        Reset();
    }
    
    void Reset()
    {
        dAlpha = 1.0;
        dBeta = 0.0;
        dGamma = 1.0;
        dDelta = 0.0;
        dEpsilon = 0.0;
        da0 = 1.0;
        dFeedback = 0.0;
        dZ1 = 0.0;
    }
    
    double Tick(double xn)
    {
        xn = xn * dGamma + dFeedback + dEpsilon * GetFeedbackOutput();
        double vn = (da0 * xn - dZ1) * dAlpha;
        double lpfOutput = vn + dZ1;
        dZ1 = vn + lpfOutput;
        return lpfOutput;
    }
    
    void SetFeedback(double fb) { dFeedback = fb; }
    double GetFeedbackOutput(){return dBeta * (dZ1 + dFeedback * dDelta); }
    void SetAlpha(double a) { dAlpha = a; };
    void SetBeta(double b) { dBeta = b; };
    
private:
    float sampleRate;
    double dAlpha;
    double dBeta;
    double dGamma;
    double dDelta;
    double dEpsilon;
    double da0;
    double dFeedback;
    double dZ1;
};

class OberheimVariationMoog : public LadderFilterBase
{
    
public:
    
    OberheimVariationMoog(float sampleRate) : LadderFilterBase(sampleRate)
    {
        LPF1 = new VAOnePole(sampleRate);
        LPF2 = new VAOnePole(sampleRate);
        LPF3 = new VAOnePole(sampleRate);
        LPF4 = new VAOnePole(sampleRate);
        
        saturation = 1.0;
        Q = 3.0;
        
        SetCutoff(1000.f);
        SetResonance(0.1f);
    }
    
    virtual ~OberheimVariationMoog()
    {
        delete LPF1;
        delete LPF2;
        delete LPF3;
        delete LPF4;
    }
    
    virtual void Process(float * samples, uint32_t n) noexcept override
    {
        for (int s = 0; s < n; ++s)
        {
            float input = samples[s];
            
            double sigma =
                LPF1->GetFeedbackOutput() +
                LPF2->GetFeedbackOutput() +
                LPF3->GetFeedbackOutput() +
                LPF4->GetFeedbackOutput();
            
            input *= 1.0 + dK;
            
            // calculate input to first filter
            double u = (input - dK * sigma) * dAlpha0;
            
            u = tanh(saturation * u);
            
            double stage1 = LPF1->Tick(u);
            double stage2 = LPF2->Tick(stage1);
            double stage3 = LPF3->Tick(stage2);
            double stage4 = LPF4->Tick(stage3);
            
            // Oberheim variations
            samples[s] = dA*u + dB*stage1 + dC*stage2 + dD*stage3 + dE*stage4;
        }
    }
    
    virtual void SetResonance(float r) override
    {
        resonance = r;
        dK = (4.0) * (Q - 1.0)/(10.0 - 1.0);
    }
    
    virtual void SetCutoff(float c) override
    {
        cutoff = c;
        
        // prewarp for BZT
        double wd = 2.0 * MOOG_PI * cutoff;
        double T = 1.0 / sampleRate;
        double wa = (2.0 / T) * tan(wd * T / 2.0);
        double g = wa * T / 2.0;
        
        // Feedforward coeff
        double G = g / (1.0 + g);
        
        LPF1->SetAlpha(G);
        LPF2->SetAlpha(G);
        LPF3->SetAlpha(G);
        LPF4->SetAlpha(G);

        LPF1->SetBeta(G*G*G / (1.0 + g));
        LPF2->SetBeta(G*G / (1.0 + g));
        LPF3->SetBeta(G / (1.0 + g));
        LPF4->SetBeta(1.0 / (1.0 + g));
        
        dGamma = G*G*G*G;
        dAlpha0 = 1.0 / (1.0 + dK * dGamma);
        
        // Oberheim variations / LPF4
        dA = 0.0;
        dB = 0.0;
        dC = 0.0;
        dD = 0.0;
        dE = 1.0;
    }
    
private:
    
    VAOnePole * LPF1;
    VAOnePole * LPF2;
    VAOnePole * LPF3;
    VAOnePole * LPF4;
    
    double dK;
    double dGamma;
    double dAlpha0;
    double Q;
    double saturation;
    
    double dA;
    double dB;
    double dC;
    double dD;
    double dE;
};

#endif
