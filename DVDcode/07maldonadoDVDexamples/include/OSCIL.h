// **************** A WAVETABLE OSCILLATOR CLASS ***********************
#ifndef __OSCIL__
#define __OSCIL__

#include <cmath>
#include <cstdlib>

#define TWO_PI  (2 * 3.14159265)

enum WAVESHAPE // enumerators that represent a possible wave shape
{
    Sine=0, Triangle, UpRamp, DownRamp, Square, Random, Custom, 
	TABLE_LEN = 5000
};

// a type of pointer to a custom function that fills the table
typedef void (*TABLEFUNC)(float*, long, void*); 

class OSCIL
{
  protected:
    long tablen;            // length of oscillator table
    double phase, si, sr;   // phase, sampling increment and sampling rate
    float *table;           // a pointer to the table array
    double amp, freq;       // amplitude and frequency of oscillator
    WAVESHAPE shape;        // wave shape to be stored into the table
  public:
    OSCIL(WAVESHAPE _shape = Sine, double _sr = 44100, long _tablen = TABLE_LEN, // constructor
          float _amp = 1, float _freq = 440, double initialPhase = 0) 
        : shape(_shape), sr(_sr), tablen(_tablen), amp(_amp), 
          freq(_freq), phase(initialPhase), si(0) 
    {
        table = new float[tablen+1]; // create the table 
        if (shape != Custom) fillTable(shape); // fill the table with a wave shape
    }
    
    virtual ~OSCIL() { delete [] table; } // destroy the table

    virtual double tic(double _amp, double _freq) { // outputs a single sample 
        amp = _amp; freq = _freq;       // get current amplitude and frequency
        double out = table[(long) (phase * tablen)] * amp; // calculate the output
        si = freq / sr;                 // calculate the sampling increment
        phase += si;                    // increment phase
        while (phase >=1 ) phase -= 1;  // check if the phase is out of range... 
        while (phase < 0 ) phase += 1;  // ...and eventually wrap it around
        return out;                     // return current sample
    }

    virtual double tic() { // version that doesn't touch amplitude and frequency
        double out = table[(long) (phase * tablen)] * amp; // calculate the output
        phase += si;                    // calculate the sampling increment
        while (phase >=1 ) phase -= 1;  // check if the phase is out of range...
        while (phase < 0 ) phase += 1;  // ...and eventually wrap it around
        return out;                     // return current sample
    }
    // updates amplitude and frequency
    void input(double _amp, double _freq) { amp = _amp; freq = _freq; si = freq / sr; }
    
    float* getTable() const {return table; }; // gets the address of the table
    long getTableLength() const { return tablen; } // gets the length of the table

    void setTableLength( long length) { // modifies the length and re-creates the table
        tablen = length;
        delete [] table;
        table = new float[tablen+1];
    }

    virtual void fillTable(WAVESHAPE s) { // fills the table with a standard wave shape
        long j;
        switch (s) 
        {
            case Triangle:
                for( j = 0; j < tablen/2; j++)
                    table[j] =  2 * (float) j/ (float) (tablen/2) - 1;
                for( j = tablen/2; j < tablen; j++)
                    table[j] = 1 -  (2 * (float) (j-tablen/2) / (float) (tablen/2));
                break;
            case UpRamp:
                for( j = 0; j < tablen; j++)
                    table[j] = (2 * (float) j / (float) tablen) - 1 ;
                break;
            case DownRamp:
                for( j = 0; j < tablen; j++)
                    table[j] = 1 -  (2 * (float) j / (float) tablen) ;
                break;
            case Square:
                for( j = 0; j < tablen/2; j++) table[j] = 1;
                for( j = tablen/2; j < tablen; j++) table[j] = -1;
                break;
            case Random:
                for( j = 0; j < tablen; j++)
                    table[j] = (2.0f * rand() / RAND_MAX) - 1;
                break;
            case Sine:
            default:
                for (j = 0; j < tablen; j++) 
                    table[j] = (float) sin(TWO_PI * j / tablen);
        }
    }
    // version that allows a user-defined function to fill the table
    virtual void fillTable( TABLEFUNC func, void *userData=0) { func(table, tablen, userData); }
    // version that allows a user-defined function to fill the table, modifying the length
    virtual void fillTable( TABLEFUNC func, long length, void *userData=0) { 
        setTableLength(length); 
        func(table, tablen, userData); 
    }
};

#endif