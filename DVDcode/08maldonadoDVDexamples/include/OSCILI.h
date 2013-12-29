// **************** AN INTERPOLATING OSCILLATOR CLASS ***********************
#ifndef __OSCILI__
#define __OSCILI__

#include "OSCIL.h"

class OSCILI : public OSCIL // inherits the OSCIL class 
{
protected:
    void copy1stSmp() {          // copies the first sample to the length+1th...
        table[tablen] = table[0];//..position in order to allow linear interpolation 
    }
public:
    // constructor must be declared, in order to call the new version of fillTable()
    OSCILI(WAVESHAPE _shape = Sine, double _sr = 44100, long _tablen = TABLE_LEN, 
            float _amp = 1, float _freq = 440, double initialPhase = 0) 
		:  OSCIL(_shape, _sr, _tablen , _amp, _freq, initialPhase)  
	{ copy1stSmp(); }
    virtual double tic(double _amp, double _freq) {
        amp = _amp; freq = _freq;
        double curr_phs = phase * tablen;   // get current phase and scale it
        long index = (long) curr_phs;       // get the integer part only
        double curr_samp = table[index];    // get current sample of the table
        double next_samp = table[index+1];  // get the next sample too
        double fract = curr_phs - index;    // get the fractional part of phase
        // apply the linear interpolation formula to get the output
        double out = (curr_samp + (next_samp - curr_samp) * fract) * amp;
        si = freq / sr;                     // calculate the sampling increment
        phase += si;                        // increment phase
        while (phase >=1 ) phase -= 1;      // check if the phase is out of range...
        while (phase < 0 ) phase += 1;      // ...and eventually wrap it around
        return out;                         // return current interpolated sample
    }
    virtual double tic() { // version that doesn't touch amplitude and frequency
        double curr_phs = phase * tablen;   // get current phase
        long index = (long) curr_phs;       // get the integer part only
        double curr_samp = table[index];    // get current sample of the table
        double next_samp = table[index+1];  // get the next sample too
        double fract = curr_phs - index;    // get the fractional part of phase
        // apply the linear interpolation formula to get the output
        double out = (curr_samp + (next_samp - curr_samp) * fract) * amp;
        phase += si;                        // increment phase
        while (phase >=1 ) phase -= 1;      // check if the phase is out of range...
        while (phase < 0 ) phase += 1;      // ...and eventually wrap it around
        return out;                         // return current interpolated sample
    }
    virtual void fillTable(WAVESHAPE s) { // new version that overrides the old one
        OSCIL::fillTable(s);    // call the old version
        copy1stSmp();           // copy the first sample to the last + 1 position
    }
    virtual void fillTable( TABLEFUNC func, void *userData=0) { // new version 
        OSCIL::fillTable(func, userData);   // call the old version
        copy1stSmp();           // copy the first sample to the last + 1 position
    }
    virtual void fillTable( TABLEFUNC func, long length, void *userData=0) { 
        OSCIL::fillTable(func, length, userData); // call the old version
        copy1stSmp();           // copy the first sample to the last + 1 position
    }
};

#endif

