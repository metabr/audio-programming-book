#ifndef __lfo2__
#define __lfo2__

#include <cmath>
#include "OSCIL.h"
#include "portaudio.h"
#include "LFO2gui.h"

class LFO2 : public LFO2gui 
{
    double CONTROL_RATE;
    OSCIL *lfo, *carrier;

    double freqLFO, ampLFO, freqCarrier;
    double logMin, logMax, ampMax;
    
    void setFreq(double f) { freqLFO = exp(f * (logMax - logMin) + logMin); }
    void setAmp(double a) { ampLFO = a * ampMax; }
    void joy_callback() { setFreq(joy->xvalue()); setAmp(1 - joy->yvalue()); }
    void LFO_radio_callback(int i) { lfo->fillTable((WAVESHAPE) i); }
    void carrier_radio_callback(int i) { carrier->fillTable((WAVESHAPE) i); }
    void carrier_freq_callback() {
        freqCarrier = exp(CarrierFreq->value() * (log(16000.0) - log(20.0)) + log(20.0)); //CORRIGE
    }
 public:
    LFO2(); 
    ~LFO2() { delete lfo; delete carrier; }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 


