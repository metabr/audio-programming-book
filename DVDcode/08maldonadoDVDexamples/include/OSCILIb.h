#ifndef __OSCILIb__
#define __OSCILIb__

#include "OSCILI.h"
#include "asignal.h"

class OSCILIb : public OSCILI
{
protected:
    ASIGNAL Out;
    double *block;
    const U_LONG blockLen;
public:
    OSCILIb(WAVESHAPE _shape = Sine, double _sr = 44100, long _tablen = TABLE_LEN, 
            float _amp = 1, float _freq = 440, double initialPhase = 0) 
          : OSCILI(_shape, _sr, _tablen , _amp, _freq, initialPhase), 
            blockLen(ASIGNAL::getBlockLen()) 
    { 
        if (shape != Custom) fillTable(shape);
        block = Out.getBlock();
    }
    virtual ASIGNAL perfBlock(const double _amp, const double _freq) {
        amp = _amp; freq = _freq;
        double *out = block;
        U_LONG n = blockLen;
        do {
            double curr_phs = phase * tablen;
            long index = (long) curr_phs;
            double curr_samp = table[index];
            double next_samp = table[index+1];
            double fract = curr_phs - index;
            *out++ = (curr_samp + (next_samp - curr_samp) * fract) * amp;
            si = freq / sr; 
            phase += si;
            while (phase >=1 ) phase -= 1;
            while (phase < 0 ) phase += 1;
        } while (--n);
        return Out;
    }
    virtual ASIGNAL perfBlock(const  ASIGNAL& _amp, const ASIGNAL& _freq) {
        double *amp = _amp.getBlock();
        double *freq = _freq.getBlock();
        double *out = block;
        U_LONG n = blockLen;
        do {
            double curr_phs = phase * tablen;
            long index = (long) curr_phs;
            double curr_samp = table[index];
            double next_samp = table[index+1];
            double fract = curr_phs - index;
            *out++ = (curr_samp + (next_samp - curr_samp) * fract) * *amp++;
            si = *freq++ / sr; 
            phase += si;
            while (phase >=1 ) phase -= 1;
            while (phase < 0 ) phase += 1;
        } while (--n);
        return Out;
    }
    virtual ASIGNAL perfBlock(const double _amp, const ASIGNAL& _freq) {
        amp = _amp;
        double *freq = _freq.getBlock();
        double *out = block;
        U_LONG n = blockLen;
        do {
            double curr_phs = phase * tablen;
            long index = (long) curr_phs;
            double curr_samp = table[index];
            double next_samp = table[index+1];
            double fract = curr_phs - index;
            *out++ = (curr_samp + (next_samp - curr_samp) * fract) * amp;
            si = *freq++ / sr; 
            phase += si;
            while (phase >=1 ) phase -= 1;
            while (phase < 0 ) phase += 1;
        } while (--n);
        return Out;

    }
    virtual ASIGNAL perfBlock(const ASIGNAL& _amp, const double _freq) {
        double *amp = _amp.getBlock();
        freq = _freq;
        double *out = block;
        U_LONG n = blockLen;
        do {
            double curr_phs = phase * tablen;
            long index = (long) curr_phs;
            double curr_samp = table[index];
            double next_samp = table[index+1];
            double fract = curr_phs - index;
            *out++ = (curr_samp + (next_samp - curr_samp) * fract) * *amp++;
            si = freq / sr; 
            phase += si;
            while (phase >=1 ) phase -= 1;
            while (phase < 0 ) phase += 1;
        } while (--n);
        return Out;
    }
};

#endif