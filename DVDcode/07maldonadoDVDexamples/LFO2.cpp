#include "LFO2.h"

LFO2 *lfoApp;

inline void LFO2::synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer) 
{       // the audio callback implementation, belonging to the LFO1 class

    double outLFO = lfo->tic(ampLFO, freqLFO);// get the ouput of the LFO
    carrier->input(1, freqCarrier + outLFO);  // get the actual frequency of the carrier

    float *out = outputBuffer;
    for( unsigned long i=0; i < framesPerBuffer; i++) { // the audio engine loop
        float outCarrier = (float) carrier->tic();    // get the output of the carrier
        *out++ = outCarrier;        // stream to the stereo ouptut (left channel)
        *out++ = outCarrier;        // right channel
    }
}

// audio callback function adapter
int LFO2_callback( const void *inBuf, void *outBuf, U_LONG framesPerBuf, 
        const PaStreamCallbackTimeInfo* tInfo, PaStreamCallbackFlags flags, void *userData )
{   lfoApp->synthesis_callback((float *) outBuf, framesPerBuf);
    return paContinue;
}

LFO2::LFO2()  // constructor
{
    SAMPLING_RATE = 44100;
    FRAMES_PER_BUFFER = 256;
    CONTROL_RATE = SAMPLING_RATE / FRAMES_PER_BUFFER;

    const double frqMinLFO = .1, frqMaxLFO = 100;

    freqLFO = 5;
    logMin = log(frqMinLFO);
    logMax = log(frqMaxLFO);
    ampMax = 300;
    ampLFO = 0;
    audio_callback = LFO2_callback;
    show();
	
	freqCarrier = 200;
    lfo = new OSCIL(Sine, CONTROL_RATE);
    carrier = new OSCIL(Sine, SAMPLING_RATE);

}

int main()
{
    lfoApp = new LFO2;
    Fl::run();
    delete lfoApp;
    return 0;
}
