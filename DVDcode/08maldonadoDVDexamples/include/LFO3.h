#ifndef __lfo3__
#define __lfo3__

#include <cmath>
#include <FL/fl_draw.H>
#include "OSCILI.h"
#include "portaudio.h"
#include "LFO3gui.h"

#define NUM_SINES 6

void computeSumOfSines(float *table, long tablen, void* sinData);

struct PARTIALS_DATA { // a struct that embeds all partial information
    double* numHarm;   // points to an array containing the order number of each partial
    double* levelHarm; // points to an array containing the level of each partial
    int numOfPartials; // total number of partials
    PARTIALS_DATA(int numpartials) : numOfPartials(numpartials) { // constructor
        numHarm   = new double[numOfPartials]; // create the arrays
        levelHarm = new double[numOfPartials];
    }
    ~PARTIALS_DATA() { // destructor
        delete [] numHarm; // delete the arrays
        delete [] levelHarm;
    }
};

class LFO3 : public LFO3gui
{
    double CONTROL_RATE;
    OSCILI *lfo, *carrier;

    double freqLFO, ampLFO, freqCarrier;
    double logMin, logMax, ampMax;

    void setFreq(double f) { freqLFO = exp(f * (logMax - logMin) + logMin); }
    void setAmp(double a) { ampLFO = a * ampMax; }
    void joy_callback() { setFreq(joy->xvalue()); setAmp(1 - joy->yvalue()); }

    void LFO_radio_callback(int i){ lfo->fillTable((WAVESHAPE) i); 
	                       redrawShape(LfoShape, lfo); }         // redraw the waveform
    void carrier_radio_callback(int i) { carrier->fillTable((WAVESHAPE) i); 
	                       redrawShape(CarrierShape, carrier); } // redraw the waveform
    void carrier_freq_callback() { freqCarrier = 
                  exp(CarrierFreq->value() * (log(16000.0) - log(20.0)) + log(20.0)); } //CORRIGE
    void redrawShape(Fl_DrawShape *o, OSCILI *osc) { // gets info to draw the wavefrom
        o->table = osc->getTable();        // provide table address to the widget object
        o->tablen = osc->getTableLength(); // provide table length to the widget object
        o->redraw();          // communicate FLTK that the widget has to be redrawn ASAP
    }
    void customWave_callback(bool isCarrier) {  // fills target table with a sum of sinusoids
        PARTIALS_DATA sinData(NUM_SINES);       // create a PARTIALS_DATA object
        for( int i = 0; i < NUM_SINES; i++) {   // fill it with appropriate data
            sinData.numHarm[i]   = numHarm[i]->value(); 
            sinData.levelHarm[i] = levelHarm[i]->value();
        }
        if (isCarrier) {                        // if the target table is the carrier then...
            carrier->fillTable(computeSumOfSines, (void*) &sinData); // ...fill its table...
            redrawShape(CarrierShape, carrier);                      // ...and redraw its shape
        }
        else                                        // else the target table is the LFO, so...
            lfo->fillTable(computeSumOfSines, (void*) &sinData);     // ...fill its table...
            redrawShape(LfoShape, lfo);                              // ...and redraw its shape
    }
  public: // the interface of the class
    LFO3(); 
    ~LFO3() { delete lfo; delete carrier; }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 
