#ifndef __lfo5__
#define __lfo5__

#include <cmath>
#include <FL/fl_draw.H>
#include "OSCILIb.h"
#include "portaudio.h"
#include "LFO5gui.h"

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

class LFO5 : public LFO5gui
{
    double CONTROL_RATE;
    OSCILIb *lfo1, *lfo2, *lfo3, *carrier, *modulator; // the oscillator objects

    double freqLFO1, ampLFO1, freqLFO2, ampLFO2, freqLFO3, ampLFO3; // the input parameters 
    double freqCarrier, freqModulator, ampModulator;                // of the oscillators
    double logMin, logMax, ampMax;
    
    double freqLFO(double f) { return exp(f * (logMax - logMin) + logMin); }
    double ampLFO(double a) { return a * ampMax; }
    void joy_callback(int i) {
        double x = joy[i]->xvalue();
        double y = 1 - joy[i]->yvalue();
        switch (i) {
            case 1: freqLFO1 = freqLFO(x); ampLFO1 = ampLFO(y); break;
            case 2: freqLFO2 = freqLFO(x); ampLFO2 = ampLFO(y); break;
            case 3: freqLFO3 = freqLFO(x); ampLFO3 = ampLFO(y); break;
            case 4: 
                freqModulator = exp(x * (log(10000.0) - log(5.0)) + log(5.0)); //CORRIGE
                ampModulator = y * 3000; 
                break;
        }
    }
    void LFO_radio_callback(int i) { 
        lfo1->fillTable((WAVESHAPE) i); 
        lfo2->fillTable((WAVESHAPE) i); 
        lfo3->fillTable((WAVESHAPE) i); 
        redrawShape(LfoShape, lfo1);
    }
    void modulator_radio_callback(int i) { modulator->fillTable((WAVESHAPE) i); 
                                    redrawShape(CarrierShape, modulator); }
    void carrier_freq_callback() { freqCarrier = 
                  exp(CarrierFreq->value() * (log(10000.0) - log(5.0)) + log(5.0)); } //CORRIGE
    void redrawShape(Fl_DrawShape *o, OSCILIb *osc) { // gets info to draw the wavefrom
        o->table = osc->getTable();        // provide table address to the widget object
        o->tablen = osc->getTableLength(); // provide table length to the widget object
        o->redraw();          // communicate FLTK that the widget has to be redrawn ASAP
    }
    void customWave_callback(bool isAudio)    // fills target table with a sum of...
    {                                         // ...partials of any shapes
        PARTIALS_DATA partialsData(NUM_PARTIALS);     // create a PARTIALS_DATA object
        for( int i = 0; i < NUM_PARTIALS; i++) {      // fill it with appropriate data
            partialsData.numHarm[i]   = numHarm[i]->value();
            partialsData.levelHarm[i] = levelHarm[i]->value();
        }
        if (isAudio)          // if the target table works at audio rate...
        {                     // ...initially, fill target table with the base shape
            if      (radioSine->value())     modulator_radio_callback(0); // initial sine
            else if (radioTriangle->value()) modulator_radio_callback(1); // initial triangle
            else if (radioUpRamp->value())   modulator_radio_callback(2); // initial up saw
            else if (radioDownRamp->value()) modulator_radio_callback(3); // initial down saw
            else if (radioSquare->value())   modulator_radio_callback(4); // initial square
        }
        else {                  // else the target table is an LFO
            if      (radioSine->value())     LFO_radio_callback(0); // initial sine
            else if (radioTriangle->value()) LFO_radio_callback(1); // initial triangle
            else if (radioUpRamp->value())   LFO_radio_callback(2); // initial up saw
            else if (radioDownRamp->value()) LFO_radio_callback(3); // initial down saw
            else if (radioSquare->value())   LFO_radio_callback(4); // initial square
        }
        if (isAudio) {  // if the target table works at audio rate then fill its table...
            modulator->fillTable(computeSumOfPartials, (void*) &partialsData);
            redrawShape(CarrierShape, modulator);      //...and redraw its shape
        }
        else {           // else the target table is an LFO, so fill its table.....
            lfo1->fillTable(computeSumOfPartials, (void*) &partialsData);
            lfo2->fillTable(computeSumOfPartials, (void*) &partialsData);
            lfo3->fillTable(computeSumOfPartials, (void*) &partialsData);
            redrawShape(LfoShape, lfo1);              //...and redraw its shape
        }
    }
    void fillStep(OSCILIb *o) {  // computes the steps of an analog-style sequencer
        const int numOfSteps = (int) numSteps->value(); // get number of steps
        const long tablen = o->getTableLength();        // get the table length
        float *table = o->getTable();                   // get the table address
        double totTime, max=0;
        int j, k;
        for (totTime=0, j=0; j < numOfSteps; j++) { 
            totTime += stepDur[j]->value();  // get the sum of all step durations
        }
        double currStepDur = stepDur[0]->value(); // get the dur of first step
        for (max=0, j=0, k=0; k < tablen; k++) {  // fill the LFO table with all steps
            table[k] = (float) stepLevel[j]->value(); // get curr step value and fill the table
            if (fabs(table[k]) > max) max = fabs(table[k]); // calculate maximum value
            if (k * totTime / tablen > currStepDur ) { // check is current step is elapsed
                j++;                                // increment the step index...
                currStepDur += stepDur[j]->value(); //...and get the dur of next step
            }
        }
        redrawShape(LfoShape, o);
    }
    void stepSequencerCompute() { // simply calls fillStep() a time for each LFO
        fillStep(lfo1);
        fillStep(lfo2);
        fillStep(lfo3);
    }
  public:
    LFO5(); 
    ~LFO5() { 
        delete lfo1; delete lfo2; delete lfo3;
        delete carrier; delete modulator;
    }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 


