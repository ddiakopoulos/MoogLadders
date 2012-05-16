# Moog Ladder Filter Models

This project contains 3 different digital models of the classic 4-pole 24 dB/octave Moog Ladder originally introduced in 1965. The cascaded structure consists of four one-pole filters and a global negative feedback loop. Several researchers have attempted to discritize this filter, including Stilson and Smith (1996), Wise (1998), Huovilainen (2004, 2010), Fontana (2007). More recently, a simplified model (based on Huovilainen's) was presented in the book DAFX: Digital Audio Effects (2011) edited by Udo Zolzer. This project contains implementations of the Silson, Huovilainen, and Simplified models.

The project will build as an AU on OSX with the JUCE library. The filter classes do not rely on the use of any external libraries or classes and can be used with little or no modifications in other DSP projects. 

## ToDo 

* The Huovilainen and Simplified models do not handle the 2x oversampling required of them and may alias under certain conditions. In the future, these models need to zero-stuff the incoming sample block and apply and anti-aliasing half-band filter prior to output. 