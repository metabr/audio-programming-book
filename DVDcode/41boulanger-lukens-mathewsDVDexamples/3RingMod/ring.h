#ifndef RING_H
#define RING_H

#include <portaudio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TABLE_LENGTH        8192        //table length for wave tables
#define SAMPLE_RATE         44100.0     //sampling rate
#define NUM_WAVESHAPES      3           //number of wave tables

class Ring
{
public:
    Ring();     //constructor
    ~Ring();    //destructor

    //PortAudio variables
    PaStream *stream; PaError err;
    float table_inc;   // table lookup - (increment) based on frequency, sample rate and table size
    float table_index; // table lookup - current index
    float **tables;    // two dimensional array storing different wave tables

    //make wave shapes
    void create_wave_sine     (float sine[],     int wave_length);
    void create_wave_saw      (float saw[],      int wave_length);
    void create_wave_triangle (float triangle[], int wave_length);

    //score function for all dsp
    float score	(float input);


    //member callback
    int myMemberCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);

    //portaudio callback
    static int myPaCallback(
            const void *input, void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData )
    {
        return ((Ring*)userData)
                ->myMemberCallback(input, output, frameCount, timeInfo, statusFlags);
    }
};

#endif // RING_H
