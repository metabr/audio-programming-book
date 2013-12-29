#ifndef MODULE_H
#define MODULE_H

#include <portaudio.h>
#include <portsf.h>
#include "mainwindow.h"

// constants for generation of period waveforms and portaudio
#define TABLE_LENGTH        8192
#define ENV_SIZE            8192
#define HARMONICS           10
#define SAMPLE_RATE         44100.0
#define PATCHES             200
#define PI                  3.141592654
#define ISRX2P              .000142475857	//(1/samplingrate) * 2 * pi
#define MAX_DELAY           3*SAMPLE_RATE	//Size of delay buffer table
#define THRESHOLD           .99
#define MAX_FILE_SIZE       60*SAMPLE_RATE

class Module
{

public:
    float table_inc;    // table lookup - (increment) based on frequency, sample rate and table size
    float table_index;  // table lookup - current index
    int   table_num;    // which table number is being used
    float **table;      // array to store the table
    float duration;     // duration for module to run
    float* tempvar;     // used differently in different modules
    float* tempout;     // output of a module
    float* tempvar2;    // used differently in different modules
    int* initflag;      // whether or not the instance of a module has been initialized
    float* tableIndex;  // where in a table we are
    float* yy1;         // filter variables
    float* yy2;
    float* zz1;
    float* zz2;
    float* RCOS;
    float* RSIN;
    float** file;       // table to store an audio file
    float* fileMax;
    int counter;
    int bufferCounter;

    PaStream *stream; PaError err;


    Module();
    ~Module();

    int myMemberCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);

    int  generate();
    float score();
    void gen10(int ftable, float* amps, int nSines);
    void osc(int patch, float freq, float amp, int ftable);
    int envelope(int patch, float attack, float decay, float amp);
    void noise(int patch, float amp);
    void twopole(int patch, float input, float freq);
    void delay(int patch, float input, float length, float feedback);
    void expenv(int patch, float attack, float decay, float amp);
    void diskin(int patch, char* filename, int manual);


    static int myPaCallback(
            const void *input, void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData )
    {
        return ((Module*)userData)
                ->myMemberCallback(input, output, frameCount, timeInfo, statusFlags);
    }
};

#endif // MODULE_H
