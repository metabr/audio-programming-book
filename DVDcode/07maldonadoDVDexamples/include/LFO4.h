#ifndef __lfo4__
#define __lfo4__

#include <cmath>
#include <FL/fl_draw.H>
#include "OSCILI.h"
#include "portaudio.h"
#include "LFO4gui.h"

#define NUM_PARTIALS 6

void computeSumOfPartials(float *table, long tablen, void* partialData);

struct PARTIALS_DATA { // a struct that embeds all partial information
    double* numHarm;   // points to an array containing the order number of each partial
    double* levelHarm; // points to an array containing the level of each partial
    int numOfPartials; // total number of partials
    PARTIALS_DATA(int numpartials) : numOfPartials(numpartials) { // constructor
        numHarm   = new double[numOfPartials]; // create the arrays
        levelHarm = new double[numOfPartials];
    }
    ~PARTIALS_DATA() {     // destructor
        delete [] numHarm; // delete the arrays
        delete [] levelHarm;
    }
};

class LFO4 : public LFO4gui
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
                  exp(CarrierFreq->value() * (log(10000.0) - log(5.0)) + log(5.0)); } //CORRIGE
    void redrawShape(Fl_DrawShape *o, OSCILI *osc) { // gets info to draw the wavefrom
        o->table = osc->getTable();        // provide table address to the widget object
        o->tablen = osc->getTableLength(); // provide table length to the widget object
        o->redraw();          // communicate FLTK that the widget has to be redrawn ASAP
    }
    void customWave_callback(bool isCarrier) // fills target table with a sum of... 
    {                                        // ...partials of any shapes 
        PARTIALS_DATA partialsData(NUM_PARTIALS);     // create a PARTIALS_DATA object
        for( int i = 0; i < NUM_PARTIALS; i++) {      // fill it with appropriate data
            partialsData.numHarm[i]   = numHarm[i]->value();
            partialsData.levelHarm[i] = levelHarm[i]->value();
        }
        if (isCarrier)        // if the target table is the carrier
		{                     // initially, fill target table with the base shape
            if      (radioSine->value())     carrier_radio_callback(0); // initial sine
            else if (radioTriangle->value()) carrier_radio_callback(1); // initial triangle
            else if (radioUpRamp->value())   carrier_radio_callback(2); // initial up saw
            else if (radioDownRamp->value()) carrier_radio_callback(3); // initial down saw
            else if (radioSquare->value())   carrier_radio_callback(4); // initial square
        }
        else {					// else the target table is the LFO
            if      (radioSine->value())     LFO_radio_callback(0); // initial sine
            else if (radioTriangle->value()) LFO_radio_callback(1); // initial triangle
            else if (radioUpRamp->value())   LFO_radio_callback(2); // initial up saw
            else if (radioDownRamp->value()) LFO_radio_callback(3); // initial down saw
            else if (radioSquare->value())   LFO_radio_callback(4); // initial square
        }
        if (isCarrier) {  // if the target table is the carrier then fill its table...
            carrier->fillTable(computeSumOfPartials, (void*) &partialsData); 
            redrawShape(CarrierShape, carrier);      //...and redraw its shape
        }
        else {            // else the target table is the LFO, so fill its table.....
            lfo->fillTable(computeSumOfPartials, (void*) &partialsData); 
            redrawShape(LfoShape, lfo);               //...and redraw its shape
        }
    }
    void stepSequencerCompute() {  // computes the steps of an analog-style sequencer
        const int numOfSteps = (int) numSteps->value(); // get number of steps
        const long tablen = lfo->getTableLength();      // get the table length
        float *table = lfo->getTable();                 // get the table address
        double totTime, max = 0;
        int j, k;
        for (totTime=0, j=0; j < numOfSteps; j++) { 
            totTime += stepDur[j]->value();  // get the sum of all step durations
        }
        double currStepDur = stepDur[0]->value(); // get the dur of first step
        for (max=0, j=0, k=0; k < tablen; k++) {  // fill the LFO table with all steps
            table[k] = (float) stepLevel[j]->value(); // get curr step value and fill the table
            if (fabs(table[k]) > max) max = fabs(table[k]); // calculate maximum value

            if (k * totTime / tablen > currStepDur ) {
                j++;
                currStepDur += stepDur[j]->value();
            }
        }
        redrawShape(LfoShape, lfo);
    }
  public: // the interface of the class
    LFO4(); 
    ~LFO4() { delete lfo; delete carrier; }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 


