# Moog Ladder Filters

This project contains several digital implementations of the classic 4-pole 24 dB/octave analog filter introduced in 1965. This filter is well known to sound musical, in that it adds a nice character to any instrument and does not follow an idealized form of an accurate/digital filter. 

The ladder structure consists of four one-pole filters and a global negative feedback loop. Several researchers have attempted to discretize this filter, including Stilson and Smith (1996), Wise (1998), Huovilainen (2004, 2010), Fontana (2007). More recently, a simplified model (based on the Huovilainen implementation) was presented in the book DAFX: Digital Audio Effects (2011) edited by Udo Zolzer. In 2013, an improved model was introduced by D'Angelo and Valimaki at the International Conference on Acoustics, Speech, and Signal Processing (ICASSP). This project contains implementations of the Silson, Huovilainen, Simplified, and Improved models.

The project will build as an AU on OSX with the JUCE library. The filter classes do not rely on the use of any external libraries or classes and can be used with little to no modification in other DSP projects.

# Licenses

Implementation | License | Original Source | Commercial Use? 
------------- | ------------- | ----------------- | -----------------
Huovilainen  | GPLv3  | CSound | No 
ImprovedMoog  | ISC  | Via Author | Yes
SimplifiedMoog  | Spiritual CC BY-NC-SA  | Transliteration of DAFX example | No
Stilson | None | Moog~ by D. Lowenfels | Unknown

## ToDo

* The Huovilainen and Simplified models do not handle the 2x oversampling required of them and will alias under specific conditions. In the future, these models need to be modified such that the incoming sample block is zero-stuffed and later half-band filtered immediately prior to output.
