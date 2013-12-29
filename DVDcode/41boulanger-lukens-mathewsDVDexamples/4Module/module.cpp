#include "module.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern bool sinReset;       //external variables
extern int sinPitch;
extern float sinAttack;
extern float sinDecay;
extern int sinWave;

Module::Module()
{
    //memory allocate these variables for all possible patch locations
    this->table = (float**)malloc(sizeof(float*)*PATCHES);
    this->tempout = (float*)malloc(sizeof(float) * PATCHES);
    this->tempvar2 = (float*)malloc(sizeof(float) * PATCHES);
    this->initflag = (int*)malloc(sizeof(int) * PATCHES);
    this->tempvar = (float*)malloc(sizeof(float) * PATCHES);
    this->tableIndex = (float*)malloc(sizeof(float) * PATCHES);
    this->yy1 = (float*)malloc(sizeof(float) * PATCHES);
    this->yy2 = (float*)malloc(sizeof(float) * PATCHES);
    this->zz1 = (float*)malloc(sizeof(float) * PATCHES);
    this->zz2 = (float*)malloc(sizeof(float) * PATCHES);
    this->RCOS = (float*)malloc(sizeof(float) * PATCHES);
    this->RSIN = (float*)malloc(sizeof(float) * PATCHES);
    this->file = (float**)malloc(sizeof(float) * PATCHES);
    this->fileMax = (float*)malloc(sizeof(float) * PATCHES);

    //init arrays and malloc multidimensional arrays
    for(int i = 0; i < PATCHES; i++)
    {
            this->initflag[i] = 0;
            this->tempout[i] = 0;
            this->tempvar[i] = 0;
            this->tempvar2[i] = 0;
            this->tableIndex[i] = 0;
            this->table[i] = (float*)malloc(sizeof(float)*MAX_DELAY);
            this->file[i] = (float*)malloc(sizeof(float)*MAX_FILE_SIZE);
    }

    initflag[6] = 1;    //this will be used as an envelope later - setting the initflag to 1 as the program starts will make sure no sound is heard on execution

    //arrays of sine wave amplitudes to be sent to the gen10 function
    //gen10 makes wave tables based on sum of sines (additive)
    float amps1[] = {1};                                                                                                            //sine
    float amps2[] = {1, 0, .3, 0, .2, 0, .14, 0, .111, 0, .090909, 0, .076923, 0, .066666, 0, .05823};                              //square
    float amps3[] = {1, .5, .33, .25, .2, .167, .14, .125, .111, .1, .090909, .0833333, .076923, .071428, .066666, .0625, .05823};  //sawtooth
    float amps4[] = {1, 0, .1111, 0, .04, 0, .00204, 0, .00123, 0, .000826, 0, .0005917, 0, .00044444, 0, .00034602, 0, .000277};   //triangle

    gen10(1, amps1, 1);			//sine wave
    gen10(2, amps2, 17);		//square wave
    gen10(3, amps3, 17);		//sawtooth wave
    gen10(4, amps4, 19);		//triangle wave

    generate(); //function to start up the audio
}


//destructor -- kill PortAudio when the instance of Module is destroyed
Module::~Module()
{
    err = Pa_StopStream( stream );
    err = Pa_CloseStream( stream );
    Pa_Terminate();
}


int Module::generate()
{
    // create the portaudio stream and start processing //
    // Initialize library before making any other calls
    err = Pa_Initialize();

    // Open an audio I/O stream
    err = Pa_OpenDefaultStream( &stream,
                              0,          // no input channels
                              1,          // mono output
                              paFloat32,  // 32 bit floating point output
                              44100,
                              256,        // frames per buffer
                              &Module::myPaCallback,
                              this);
    err = Pa_StartStream( stream );

    return 0;
}

//// portaudio callback function ////

// this function is defined in the portaudio.h header file and implemented here
int Module::myMemberCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
{
    // create a local pointer to the output buffer that we can increment (this is very common)
    float *out = (float*)output;

    // loop through the buffersize and write out values from our table
    for(int i=0; i<frameCount; i++)
    {
        out[i] = score();       //set the output sample to whatever score() returns
    }

    return 0;
}


//DSP happens here
float Module::score()
{
    float output = 0;       //variable for one output sample

    //if play sound button is pressed, reset the envelope
    if(sinReset)
    {
        tempvar2[6] = 0;        //reset the envelope finished flag    (envelope is on patch 6)
        initflag[6] = 0;        //reset the initflag for the envelope
        sinReset = 0;           //turn off sinReset
    }

    //if the envelope is still running, make sound (tempvar2 is the envelope finished flag in evelopes)
    if(!tempvar2[6])
    {
        osc(5, sinPitch, .8, sinWave);      //oscillator on patch 5 with pitch and wave assigned from the UI
        expenv(6, sinAttack, sinDecay, 1);  //exponential envelope on patch 6 with the attack and decay assigned from the UI

        output = tempout[5] * tempout[6];   //set the output sample as the enveloped oscillator (osc * env)
    }

    return output;      //return 1 output sample
}




//
//MODULES  ---- these modules are all explained better in Richard Boulanger's DVD chapter in the code example this is taken from
//              only two of these modules are used in this example, but I have left all of them in to allow you to experiment with making
//              different types of synthesizers that can take advantage of any of the modules
//

//Gen 10 - fill a function table based on a sum of sines.
void Module::gen10(int ftable, float* amps, int nSines)
{
    int i, j;
    float max = 0;

    for(i = 0; i < TABLE_LENGTH; i++)
    {
        for(j = 0; j < nSines; j++)
            table[ftable][i] += (sin((2.0*PI*i/(float)TABLE_LENGTH)*(j+1))) * amps[j];

        if(fabs(table[ftable][i]) > max) max = fabs(table[ftable][i]);
    }

    for(i = 0; i < TABLE_LENGTH; i++)
        table[ftable][i] /= max;
}


//oscillator - table look up oscillator
//Uses function tables generated by the gen10 function
//tempvar = table increment
void Module::osc(int patch, float freq, float amp, int ftable)
{
    if(!initflag[patch])
    {
        initflag[patch] = 1;
        tempvar[patch] = 0;
        tempout[patch] = 0;
        tableIndex[patch] = 0;
    }

    //set table increment based on frequency
    tempvar[patch] = (freq * TABLE_LENGTH/SAMPLE_RATE);

    //set output
    if((int)tableIndex[patch] >= 0)
        tempout[patch] = table[ftable][(int)tableIndex[patch]] * amp;

    //increment
    tableIndex[patch] += tempvar[patch];

    //if index is greater than the size, then loop back around to the beginning
    if(tableIndex[patch] > TABLE_LENGTH) tableIndex[patch] -= TABLE_LENGTH;
}




//env
//tempvar = table increment
//tempvar2 = envelope finished flag
int Module::envelope(int patch, float attack, float decay, float amp)
{
    if(initflag[patch] == 0)
    {
        int i;
        float total, Nattack, Ndecay;

        initflag[patch] = 1;
        tempvar2[patch] = 0;

        total = attack+decay;
        Nattack = (attack / total) * ENV_SIZE;
        Ndecay = ENV_SIZE - Nattack;

        for(i = 0; i < ENV_SIZE; i++)
        {
            if(i < Nattack)
                table[patch][i] = i / Nattack;
            else
                table[patch][i] = (Ndecay - (i - Nattack)) / Ndecay;
        }

        table[patch][0] = 0;
        table[patch][ENV_SIZE] = 0;

        tempvar[patch] = ((1/total) * ENV_SIZE/SAMPLE_RATE);	//tempvar = table increment
        tableIndex[patch] = 0;
        tempout[patch] = .0001;
    }

    tempout[patch] = table[patch][(int)tableIndex[patch]];
    tempout[patch] *= amp;
    tableIndex[patch] += tempvar[patch];

    if(tableIndex[patch] > ENV_SIZE)
    {
        tempvar2[patch] = 1;
        tempout[patch] = 0;
    }

    return 0;
}

void Module::noise(int patch, float amp)
{
    tempout[patch] = rand() % 44100;
    tempout[patch] /= 44100.;
    tempout[patch] *= amp;
}



//exponential envelope
void Module::expenv(int patch, float attack, float decay, float amp)
{
    if(!initflag[patch])
    {
        initflag[patch] = 1;
        tempvar2[patch] = 0;

        float a, b;
        float total = attack + decay;
        int i;
        attack = (attack/total) * ENV_SIZE;
        decay = ENV_SIZE - attack;

        a = 1/(attack * attack);
        b = 1/(decay * decay);

        for(i = 0; i < attack; i++)
            table[patch][i] = a*i*i;
        for(i = 0; i < decay; i++)
            table[patch][i+(int)attack] = (b*i*i) - decay*2*b*i + decay*b + 1;

        tempvar[patch] = ((1/total) * ENV_SIZE/SAMPLE_RATE);	//tempvar = table increment
        tableIndex[patch] = 0;
    }
    tempout[patch] = table[patch][(int)tableIndex[patch]];
    tempout[patch] *= amp;
    tableIndex[patch] += tempvar[patch];

    if(tableIndex[patch] > ENV_SIZE)
    {
        tempvar2[patch] = 1;
        tempout[patch] = 0;
    }
}


//twopole filter
// phi=(2pi/sampling_rate)*(freq) where freq is the "real frequency" of pole in hz
// rr=exp(- (2pi/sampling_rate)*(sigma) ) where sigma is "decay freq" of pole in hz
//                         ALTERNATIVELY
// rr=exp((- 1/(decay*sampling_rate)) where decay is the time in seconds for impulse response to decay to 1/efloat twopole(float input)
//
//Filter programmed by Max Mathews. I have adapted it to work in this program.
void Module::twopole(int patch, float input, float freq)
{
    float rr,phi;
    float sigma = 1000;

    //initialize
    //This filter cannot take a varible cutoff frequency.
    //Once the frequency has been set, it can't be moved without reinitializing the entire filter.
    if(initflag[patch] == 0)
    {
        initflag[patch] = 1;

        //lots of math
        rr = exp(-sigma * ISRX2P);
        phi = ISRX2P * freq;    			//ISRX2P=(1/samplingrate)*2*pi
        RCOS[patch] = rr * cos(phi);
        RSIN[patch] = rr * sin(phi);

        //set some starting values
        zz1[patch] = .01;
        yy1[patch] = .01;
    }

    //more math
    yy2[patch] = (RCOS[patch] * yy1[patch]) - (RSIN[patch] * (zz1[patch] - input));
    zz2[patch] = (RSIN[patch] * yy1[patch]) + (RCOS[patch] * zz1[patch]);
    zz1[patch] = zz2[patch];
    yy1[patch] = yy2[patch];

    //set output
    tempout[patch] = zz1[patch];
}



void Module::delay(int patch, float input, float length, float feedback)
{
    int i;

    //initialize some stuff
    if(!initflag[patch])
    {
        if(length > 3) exit(0);
        initflag[patch] = 1;
        for(i=0;i<length*SAMPLE_RATE;i++)
            table[patch][i] = 0;
        tempvar[patch] = 0;
        tableIndex[patch] = 0;
    }

    //write sample to buffer adding feedback from anything that is already there.
    table[patch][(int)tableIndex[patch]] = input + (feedback*table[patch][(int)tableIndex[patch]]);

    //increment buffer pointer
    tableIndex[patch]++;

    //if the buffer length has been reached, reset the index
    if(tableIndex[patch] > length*SAMPLE_RATE)
        tableIndex[patch] = 0;

    //output delayed buffer
    tempout[patch] = table[patch][(int)tableIndex[patch]];
}



//diskin
//sound file reader
void Module::diskin(int patch, char* filename, int manual)
{
    if(!initflag[patch])
    {
        initflag[patch] = 1;
        tableIndex[patch] = 0;

        PSF_PROPS props;
        long totalread;
        int ifd = -1;
        float* frame = NULL;

        psf_init();

        ifd = psf_sndOpen(filename, &props, 0);

        printf("chans: %d\n", props.chans);

        frame = (float*) malloc(props.chans * sizeof(float));

        file[patch][0] = psf_sndReadFloatFrames(ifd,frame,1);
        totalread = 0;
        while ((psf_sndReadFloatFrames(ifd,frame,1)) == 1)
        {
            file[patch][totalread] = frame[0];
            totalread++;
        }

        fileMax[patch] = totalread;
        printf("%s: total frames read: %d\n", filename, totalread);
    }

    if(!manual)
    {
        tempout[patch] = file[patch][(int)tableIndex[patch]];
        tableIndex[patch]++;
        if(tableIndex[patch] > fileMax[patch]) tableIndex[patch] = 0;
    }
}
