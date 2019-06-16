# Moog Ladder Filters

This project contains different digital implementations of the classic 4-pole, 24 dB/octave analog filter introduced in 1965. The filter is well-regarded to add a nice character to any sound source, either synthesized or acoustic. 

The ladder structure consists of four one-pole filters and a global negative feedback loop. Several academics have attempted to discretize this filter, and some academic publications on the topic can be found in the `research/` directory.

The filter classes do not rely on external libraries and can be used with little to no modification in other DSP projects. Every filter has been modified from its original implementation for code clarity and/or runtime performance. The project includes a test app that will pass white noise through each of the implemented filter variants. 

# Filter Tuning & A Word of Warning
Each model is unique. The newest is from 2015 while the oldest dates back over 20 years. Some try to remain true to their analog counterpart, where others are more approximate. The filters have not been rigorously verified for all combinations of cutoff, resonance, and sampling rate. Some are purposely built to self-oscillate, but beware the occasional blow-up with parameters that exceed some undiscovered value. 

# Models & Licenses

“Closed-Source Friendly” indicates if the individual license permits redistribution in a closed-source product (like a VST plugin). Filtered output audio is fair game for any kind of sample library or music production, commercial or otherwise. In the case of copyright-only code, it is possible to contact the original author to request an explicit license.

Implementation | License | Original Source | Closed-Source Friendly
------------- | ------------- | ----------------- | -----------------
Simplified | Custom | DAFX | No
Huovilainen  | LGPLv3 | CSound | If dynamically linked
Stilson | Unlicense | Moog~ by D. Lowenfels | Yes
Microtracker | Unlicense | Magnus Jonsson | Yes
Krajeski | Unlicense | Aaron Krajeski | Yes
MusicDSP | Suggested CC-BY-SA | MusicDSP.org | Yes
Oberheim | Custom | Will Pirkle | Yes
Improved | ISC | Via Author | Yes
RKSimulation | BSD | Bob~ by Miller Puckette | Yes

# ToDo

Community contributions are welcome.

* Several filters have extra parameters that could be exposed (drive, thermal coefficients, Q, etc).
* Many filters could be easily modified for HPF or other types of output.
* Filter response graphs.
* The Huovilainen and Simplified models need to be oversampled and nyquist filtered.

# License
If not otherwise stated in the header of a file, all other code in this project is released under the Unlicense.
