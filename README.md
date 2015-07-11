# Moog Ladder Filters

This project contains several digital implementations of the classic 4-pole 24 dB/octave analog filter introduced in 1965. This filter is well known to sound musical, in that it adds a nice character to any instrument and does not follow an idealized form of an accurate/digital filter. 

The ladder structure consists of four one-pole filters and a global negative feedback loop. Several researchers have attempted to discretize this filter, including Stilson and Smith (1996), Wise (1998), Huovilainen (2004, 2010), Fontana (2007). 

More recently, a simplified model (based on the Huovilainen implementation) was presented in the book DAFX: Digital Audio Effects (2011) edited by Udo Zolzer. In 2013, an improved model was introduced by D'Angelo and Valimaki at the International Conference on Acoustics, Speech, and Signal Processing (ICASSP). 

The filter classes do not rely on the use of any external libraries and can be used with little to no modification in other DSP projects. The project includes a test app that makes uses of RTAudio for cross-platform audio output. 

# Models & Licenses

Commercial use indicates if the license terms permit redistribution in a closed-source product (like a VST plugin). Filtered output audio is fair game for any kind of sample library or music production, commercial or otherwise. 

Implementation | License | Original Source | Commercial Use? 
------------- | ------------- | ----------------- | -----------------
Stilson | Copyright | Moog~ by D. Lowenfels | No
Oberheim | Copyright | Will Pirkle | No 
Simplified | Copyright | DAFX Example | No
Microtracker | Copyright | Magnus Jonsson | No
Aaron | Copyright | Via Author | No 
Huovilainen  | GPLv3 | CSound Src | OSS 
Improved | ISC | Via Author | Yes
RKSimulation | BSD | Bob~ by Miller Puckette | Yes
MusicDSP | Unlicensed | MusicDSP.org | Maybe

## ToDo

* The Huovilainen and Simplified models need to be correctly oversampled and nyquist filtered