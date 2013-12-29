#ifndef __LFO6__
#define __LFO6__

#include <cmath>
#include <FL/fl_draw.H>
#include "TABLE.h"
#include "portaudio.h"
#include "LFO6gui.h"

#define NUM_PARTIALS 6

void computeSumOfPartials(float *table, long tablen, void* partialData);

struct PARTIALS_DATA { // a struct that embeds all partial information
    double* numHarm;   // points to an array containing the order number of each partial
    double* levelHarm; // points to an array containing the level of each partial
    WAVESHAPE* shape;  // points to an array containing the shape identifer of each partial
    double* phase;     // points to an array containing the phase of each partial
    int numOfPartials; // total number of partials
    PARTIALS_DATA(int numpartials) : numOfPartials(numpartials) {  // consturctor
        numHarm = new double[numOfPartials];  // create the arrays
        levelHarm = new double[numOfPartials];
        shape = new WAVESHAPE[numOfPartials];
        phase = new double[numOfPartials];
    }
    ~PARTIALS_DATA() {     // destructor
        delete [] numHarm; // delete the arrays
        delete [] levelHarm;
        delete [] shape;
        delete [] phase;
    }
};

class LFO6 : public LFO6gui
{
    double CONTROL_RATE;
    TABLE *table;
    OSCILIextTABLE *lfo1, *lfo2, *lfo3, *lfo4, *carrier, *modulator; // the oscillator objects

    double freqLFO1, ampLFO1, freqLFO2, ampLFO2, freqLFO3, ampLFO3, freqLFO4;
    double freqCarrier, freqModulator, ampModulator;
    double logMin, logMax, ampMax;
    
    double freqLFO(double f) { return exp(f * (logMax - logMin) + logMin);  }
    double ampLFO (double a) { return a * ampMax; }
    void joy_callback(int i) {
        double x = joy[i]->xvalue();
        double y = 1 - joy[i]->yvalue();
        switch (i) {
            case 1: freqLFO1 = freqLFO(x); ampLFO1 = ampLFO(y); break;
            case 2: freqLFO2 = freqLFO(x); ampLFO2 = ampLFO(y); break;
            case 3: freqLFO3 = freqLFO(x); ampLFO3 = ampLFO(y); break;
            case 4: 
                freqModulator = exp(x * (log(10000.0) - log(5.0)) + log(5.0)); 
                ampModulator = y * 700; 
                break;
        }
    }
    void carrier_freq_callback() { // called when moving corresponding slider
        freqCarrier = exp(CarrierFreq->value() * (log(10000.0) - log(5.0)) + log(5.0));
    }
    void lfo4_freq_callback() {    // called when moving corresponding slider
        freqLFO4 = exp(LFO4freq->value() * (log(500.0) - log(.05)) + log(.05)); 
    }
    void redrawShape(Fl_DrawShape *o, TABLE *tab) { // gets info to draw the wavefrom
        o->table = tab->getTable();        // provide table address to the widget object
        o->tablen = tab->getTableLength(); // provide table length to the widget object
        o->redraw();          // communicate FLTK that the widget has to be redrawn ASAP
    }
    void customWave_callback(int tableIndex)     // fills target table with a sum of...
    {                                            // ...partials of any shapes
        PARTIALS_DATA partialsData(NUM_PARTIALS);     // create a PARTIALS_DATA object
        for( int i = 0; i < NUM_PARTIALS; i++) {      // fill it with appropriate data
            partialsData.numHarm[i]   = numHarm[i]->value();
            partialsData.levelHarm[i] = levelHarm[i]->value();
            partialsData.shape[i]     = (WAVESHAPE) chPartial[i]->value();
            partialsData.phase[i]     = phsHarm[i]->value();
        }
        TABLE* tab = &table[tableIndex]; // get a pointer to current TABLE object
        tab->fillTable(computeSumOfPartials, (void*) &partialsData); // fill it with a shape
        redrawShape(tableShape[tableIndex], tab);                    // and redraw it
    }
    void stepSequencerCompute(int tableIndex) { // computes the steps of the sequencer
        TABLE* tab = &table[tableIndex]; // get a pointer to current TABLE object
        const int numOfSteps = (int) numSteps->value(); // get number of steps
        const long tablen = tab->getTableLength();      // get the table length
        float *tbl = tab->getTable();                   // get the table address
        int j, k;
        double totTime = 0, max = 0;
        for (k=0; k < numOfSteps; k++) {
            totTime += stepDur[k]->value();       // get the sum of all step durations
        }
        double currStepDur = stepDur[0]->value(); // get the dur of first step
        for (k=0, j=0; k < tablen; k++) { // fill the LFO table with all steps
            tbl[k] = (float) stepLevel[j]->value(); // get curr step value and fill the table
            if (fabs(tbl[k]) > max) max = fabs(tbl[k]);// calculate maximum value
            if (k * totTime / tablen > currStepDur ) { // check is current step is elapsed
                j++;                                // increment the step index...
                currStepDur += stepDur[j]->value(); //...and get the dur of next step
            }
        }
        redrawShape(tableShape[tableIndex], tab);
    }
    void adsrCompute(int tableIndex) {   // fills target table with an ADSR envelope
        TABLE* tab = &table[tableIndex]; // get a pointer to current TABLE object
        const long tablen = tab->getTableLength(); // get its length
        float *tbl = tab->getTable();    // get the address of its sample array   
        double totTime = 0;              // will contain the total duration of the pattern
        double sustDur = sldSustDur->value() * 8;    // get the sustain duration
        double pauseDur = sldPauseDur->value() * 15; // get the pause duration
        double decayDur = sldDecay->value()*8;       // get the decay duration
        double release = sldRelease->value() * 8;    // get the release duration
        totTime += sldAttak->value();                // add the attack duration   
        totTime += decayDur;                         // add the decay duration
        totTime += release;                          // add the release duration
        totTime += sustDur;                          // add the sustain duration
        totTime += pauseDur;                         // add the pause duration
        int k, i;
        double currStepSamples = (sldAttak->value()/totTime) * tablen; // get the last sample 
                                                                       // of this segment
        for (k = 0; k < currStepSamples; k++)           // The ATTAK segement 
            tbl[k] = (float) (k / currStepSamples);     // fill it with samples
        double decayStepSamples = (decayDur/totTime) * tablen; // get decay seg length
        currStepSamples += (decayDur/totTime) * tablen; // get last sample of this seg
        for (i=0; k < currStepSamples; k++, i++)        // The DECAY segment
            tbl[k] = (float) (1 - (i / decayStepSamples)  * (1-sldSustain->value())  ); // fill
        float sustLevel = (float) tbl[k-1];             // get sustain level
        currStepSamples += (sustDur / totTime) * tablen;// get last sample of this segment
        for ( ; k < currStepSamples; k++)               // The SUSTAIN segment
            tbl[k] = sustLevel;                         // fill it with samples
        double releaseStepSamples = (release/totTime) * tablen; // get release seg length
        currStepSamples += (release/totTime) * tablen;  // get last sample of this segment
        for (i=0 ; k < currStepSamples; k++, i++)       // The RELEASE segment
            tbl[k] = (float) (sustLevel - (i / releaseStepSamples) * sustLevel); // fill it
        currStepSamples += (pauseDur/totTime) * tablen; // get last sample after the pause 
        for ( ; k < currStepSamples; k++)               // The PAUSE segment
            tbl[k] = 0;                                 // fill it with zeros
        redrawShape(tableShape[tableIndex], tab);
    }
    void tableShape_callback(Fl_Choice* o, int tableIndex) // called when choosing a 
    {                                                      // shape for the TABLE object
        WAVESHAPE shape = (WAVESHAPE) o->value(); // get target shape from the Fl_Choice object
        switch (shape) {
            case Sine: case Triangle: case UpRamp: case DownRamp: case Square: case Random:
                table[tableIndex].fillTable(shape); 
                break;
            case Custom:
                customWave(tableIndex);
                break;
            case 7: // Step sequencer
                stepSequencer(tableIndex);
                break;
            case 8: // ADSR
                adsr(tableIndex);
                break;
        }
        redrawShape(tableShape[tableIndex], &table[tableIndex]);
    }
    void oscTable_callback(Fl_Choice* o, int oscIndex) // called when routing a table to
    {                                                  // one of the six oscillators
        int tableIndex = o->value();         // get target table index from the Fl_Choice object
        switch(oscIndex) {
            case 0:      lfo1->setTable(table[tableIndex]); break;
            case 1:      lfo2->setTable(table[tableIndex]); break;
            case 2:      lfo3->setTable(table[tableIndex]); break;      
            case 3:      lfo4->setTable(table[tableIndex]); break;
            case 4: modulator->setTable(table[tableIndex]); break;
            case 5:   carrier->setTable(table[tableIndex]); break;
        }
    }
  public:
    LFO6();    // constructor
    ~LFO6() {  // destructor 
        delete lfo1; delete lfo2; delete lfo3, delete lfo4; // delete all oscillators
        delete carrier; delete modulator; 
        delete [] table;                         // delete the array of TABLE objects
    }
    inline void synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer);
};

#endif 


