# Moog Ladder Filters

This project contains several digital implementations of the classic 4-pole 24 dB/octave analog filter introduced in 1965. This filter is well known to add a nice character to any instrument, synthesized or acoustic. 

The ladder structure consists of four one-pole filters and a global negative feedback loop. Several researchers have attempted to discretize this filter, and their efforts are documented in the research/ directory. Most of the implementations are based on this research in some form. 

The filter classes do not rely on the use of any external libraries and can be used with little to no modification in other DSP projects. The project includes a test app that will play a short clip of white noise through the filters. 

# Filter Tuning & A Word of Warning
Each filter has a unique character, accurate or not. The newest model is from 2015, and the oldest dates back more than 20 years. Some try to remain true to their analog counterpart, where others are more approximate. Each of the models have not been rigorously verified for all combinations of cutoff, resonance, and sampling rate. Some of the models are purposely built to self-oscillate, but beware that others might blow up with parameters that exceed some undiscovered value.

# Models & Licenses

“Closed Source Friendly” indicates if the license terms permit redistribution in a closed-source product (like a VST plugin). Filtered output audio is fair game for any kind of sample library or music production, commercial or otherwise. In the case of copyright-only code, it is possible to contact the original author and request an explicit license.

Implementation | License | Original Source | Closed-Source Friendly
------------- | ------------- | ----------------- | -----------------
Stilson | Copyright | Moog~ by D. Lowenfels | -
Oberheim | Copyright | Will Pirkle | -
Simplified | Copyright | DAFX Example | - 
Microtracker | Copyright | Magnus Jonsson | -
Aaron | Copyright | Via Author | -
Huovilainen  | GPLv3 | CSound | No
Improved | ISC | Via Author | Yes
RKSimulation | BSD | Bob~ by Miller Puckette | Yes
MusicDSP | Unlicensed | MusicDSP.org | Maybe

## ToDo

* The Huovilainen and Simplified models need to be correctly oversampled and nyquist filtered
* Several filters have extra parameters that could be exposed (drive, thermal coefficients, Q, etc)
* Many filters could be easily modifiedfor HPF output
* Denormal prevention
* (Longer term) Filter response graphs
