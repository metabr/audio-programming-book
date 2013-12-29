#ifndef __lfo1__
#define __lfo1__

#include <cmath>
#include "portaudio.h"
#include "LFO1gui.h"

class LFO1 : public LFO1gui 
{
    double CONTROL_RATE;
    double freqLFO, ampLFO;         // LFO parameters
    double logMin, logMax, ampMax;  // MAXs and MINs
    double phaseLFO, phaseCarrier;  // instantaneous phases of the two oscillators

    // get carrier freq from the numeric text box widget
    double getCarrierFreq() { return CarrierFreq->value(); } 
    // set LFO freq, basing on a normalized value
    void setFreq(double f) { freqLFO = exp(f * (logMax - logMin) + logMin); }
    void setAmp(double a) { ampLFO = a * ampMax; } // set LFO amplitude
    void joy_callback() {                           
        double x = joy->xvalue();    // get normalized values from the joystick
        double y = 1 - joy->yvalue();// invert the y value to obtain down-up increase
        setFreq(x); // convert freq value to its actual exponential range
        setAmp(y);  // convert amp value to its actual range
    }
  public:
    LFO1(); 
    ~LFO1() { }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 

