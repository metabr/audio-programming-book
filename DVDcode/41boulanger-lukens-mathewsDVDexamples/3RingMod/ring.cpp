//ring.cpp - source file for PortAudio stuff mainly dealing with ring modulation of an input

#include "ring.h"

extern float freq;      //global frequency
extern int table_num;   //global wave shape table number


//constructor - This gets called when an instance of Ring is created
Ring::Ring()
{
    freq = 40;          //set some initial values
    table_index = 0;

    // allocate space for the pointers to each array
    // tables => float **;
    tables = (float**)malloc(sizeof(float*)*NUM_WAVESHAPES);

    // allocate space for the arrays themselves
    for(int i = 0; i < NUM_WAVESHAPES; i++)
        tables[i] = (float*)malloc(sizeof(float)*TABLE_LENGTH);

    //after those mallocs, tables is now tables[NUM_WAVESHAPES][TABLE_LENGTH]

    //generate tables
    create_wave_sine(tables[0], TABLE_LENGTH);
    create_wave_saw(tables[1], TABLE_LENGTH);
    create_wave_triangle(tables[2], TABLE_LENGTH);


    // Initialize PortAudio library before making any other calls
    err = Pa_Initialize();

    // Open an audio I/O stream
    err = Pa_OpenDefaultStream( &stream,
                              1,          // one input channel
                              1,          // one output channel
                              paFloat32,  // 32 bit floating point output
                              44100,      // sample rate
                              512,        // frames per buffer
                              &Ring::myPaCallback,  //name of the callback function
                              this);

    // start stream
    err = Pa_StartStream( stream );
}

// sine wave - pretty simple to compute. divide one iteration around the unit circle into the
// number of points we want to sample and crank them out in a loop.
void Ring::create_wave_sine(float sine[], int wave_length) {
  int i;
  for(i = 0; i < wave_length; i++) {
    sine[i] = sin(2.0*M_PI*i/(float)wave_length);
  }
}

// sawtooth wave - ramp from 1 to -1 across the period
void Ring::create_wave_saw(float saw[], int wave_length) {
  int i;
  for(i = 0; i < wave_length; i++) {
    saw[i] = 1-i*2.0/(float)(wave_length);
  }
}

// triangle wave - ramp from -1 to 1 at the middle of the period, and then back down to -1
void Ring::create_wave_triangle(float triangle[], int wave_length) {
  int i;
  float halfway = ((float)wave_length)/2.;
  for(i = 0; i < wave_length; i++) {
    if(i < halfway)
      triangle[i] = -1 + i*2.0/(float)(halfway);
    else
      triangle[i] = 1 - (i-halfway)*2.0/(float)(halfway);
  }
}

//// portaudio callback function ////

// this function is defined in the portaudio.h header file and implemented here
int Ring::myMemberCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
{
    // create a local pointer to the output buffer that we can increment (this is very common)
    float *out = (float*)output;

    // local pointer to the input buffer so we can send it to score()
    float *in = (float*)input;

    // loop through the buffersize and write out values from our table
    for(int i=0; i<frameCount; i++)
    {
        out[i] = score(in[i]);      //set the output sample as whatever is returned from score
    }

    return 0;
}


//ALL DSP HAPPENS HERE
float Ring::score(float input)
{
    float output;           //one output sample
    float modifiedSignal;   //one sample of a modified sample
    table_inc = freq * (float)TABLE_LENGTH/(float)SAMPLE_RATE;  //increment = (freuency * table length) / sample rate -- standard table lookup formula

    //multiply the input by the oscillator
    modifiedSignal = input * tables[table_num][(int)table_index];

    //assign the output to be returned to the callback function
    output = modifiedSignal;

    //increment our table index
    table_index += table_inc;

    //if our index exceeds the end of the table, wrap back around to the beginning
    if(table_index > TABLE_LENGTH) table_index -= TABLE_LENGTH;

    //return the modified signal
    return output;
}


//destructor - kill PortAudio when ring is destroyed
Ring::~Ring()
{
    err = Pa_StopStream( stream );
    err = Pa_CloseStream( stream );
    Pa_Terminate();
}
