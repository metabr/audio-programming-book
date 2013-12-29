#include <cstdio>		// the header stdio.h in C++ style
#include <cmath>		// the header math.h in C++ style
#include <FL/fl_ask.H> 	// an FLTK header for fl_message()

#include "main_prot.h"
#include "helloRingGUI.h"

const double SAMPLING_RATE = 44100;
const double TWO_PI = 3.14159265 * 2.0;
const int FRAMES_PER_BUFFER = 256;

PA_STUFF pa;        // the global definition of pa structure
RingModGUI *gui=0;  // the GUI object pointer

int audio_callback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    float  sine1, sine2, inLeft, inRight;
    float  *in=(float*) inputBuffer, *out = (float*)outputBuffer;
    static double phase1 = 0, phase2 = 0;
    static double si1 = 0, si2 = 0, wetVol = 0, dryVol = 0;
    unsigned long i;
    if (gui->isFreqUpdated) {      // if one of the frequencies has been updated,..
        si1 = TWO_PI * gui->freq1 / SAMPLING_RATE; // ...get the freqs of both sinusoids...
        si2 = TWO_PI * gui->freq2 / SAMPLING_RATE; // ...to obtain the sampling increments.
        gui->isFreqUpdated = false;// this flag prevents uselsess processing
    }
    if (gui->isVolUpdated) {       // if one of the volumes has been updated, ...
        wetVol = gui->wetVolume;   // ...get wet and...
        dryVol = gui->dryVolume;   // ...dry volumes.
        gui->isVolUpdated = false; // this flag prevents uselsess processing
    }
    for( i=0; i < framesPerBuffer; i++ ) { // the audio engine loop
        inLeft  = *in++;        // get a sample from the left input channel
        inRight = *in++;        // do the same for the right channel
        sine1 = sin(phase1);    // calculate the sinus for... 
        sine2 = sin(phase2);    // ...current phase of both sinusoids
        *out++ = (sine1+sine2) * inLeft  * wetVol + inLeft  * dryVol; // left out
        *out++ = (sine1+sine2) * inRight * wetVol + inRight * dryVol; // right out
        phase1 += si1;          // increment the phases of...
        phase2 += si2;          // ...the two sinusoids
    }
    return paContinue;
}

void init_stuff()
{
    Pa_Initialize();
}

void terminate_stuff()
{
    if (pa.audio != 0) {    // check if the audio stream object is actually opened
        Pa_StopStream( pa.audio );
        Pa_CloseStream( pa.audio );
    }
    Pa_Terminate();
}

void initFlags()
{
    pa.isInputDeviceChosen = false;     // no input device chosen
    pa.isOutputDeviceChosen = false;    // no output device chosen
    pa.isPlaying = false;               // not playing
}

void close_if_opened()  
{
    if (pa.audio != 0) {            // check if the stream object is already opened
        if (pa.isPlaying) {         // check if the stream object is playing
            Pa_StopStream( pa.audio ); // stop playing
            pa.isPlaying = false;   // inform that the stream object is stopped
        }
        Pa_CloseStream(pa.audio);   // close the stream and
        pa.audio = 0;               // invalidate its pointer
    }
}

void getAvailableInputs()
{
    close_if_opened();      // if the stream object has been opened, close it
    for (int i=0; i < Pa_GetDeviceCount(); i++) {   // traverse all audio devices
        pa.info = Pa_GetDeviceInfo(i);              // get info about this device
        pa.hostapi = Pa_GetHostApiInfo(pa.info->hostApi); // get info about its host API

        if (pa.info->maxInputChannels > 0) { // if this one is an input device
            sprintf(pa.s,"%d: [%s input] %s",i, pa.hostapi->name, pa.info->name );  
            gui->brwDevSelect->add(pa.s, (void *) i); // add it to the list-box items
        }
    }
}

void getAvailableOutputs()
{
    close_if_opened();      // if the stream object is already opened, close it
    for (int i=0; i < Pa_GetDeviceCount(); i++) {   // traverse all audio devices
        pa.info = Pa_GetDeviceInfo(i);              // get info about this device
        pa.hostapi = Pa_GetHostApiInfo(pa.info->hostApi); // get info about its host API
        if (pa.info->maxOutputChannels > 0) { // if this one is an output device
            sprintf( pa.s,"%d: [%s output] %s",i, pa.hostapi->name, pa.info->name );  
            gui->brwDevSelect->add(pa.s, (void *) i); // add it to the list-box items
        }
    }
}

void openInput()
{
    if (gui->inDevNum >=0) {                       // if an input device has been chosen
        pa.inputParameters.device = gui->inDevNum; // fill the inputParameters structure
        pa.inputParameters.channelCount = 2;         // stereo input 
        pa.inputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
        pa.inputParameters.suggestedLatency = pa.info->defaultLowInputLatency;
        pa.inputParameters.hostApiSpecificStreamInfo = 0;
        pa.isInputDeviceChosen = true;   // inform that an input device has been chosen
    }
    else {
        fl_message("You have to choose an audio input port from the list first!");
        pa.isInputDeviceChosen = false; // inform that no input device has been chosen
    }
}

void openOutput()
{
    if (gui->outDevNum >=0) {                      // if an output device has been chosen
        pa.outputParameters.device = gui->outDevNum;// fill the outputParameters structure 
        pa.outputParameters.channelCount = 2;       // stereo output 
        pa.outputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
        pa.outputParameters.suggestedLatency = pa.info->defaultLowOutputLatency;
        pa.outputParameters.hostApiSpecificStreamInfo = 0;
        pa.isOutputDeviceChosen = true;  // inform that an output device has been chosen
    }
    else {
        fl_message("You have to choose an audio ourtput port from the list first!");
        pa.isOutputDeviceChosen = false; // inform that no output device has been chosen
    }
}

void openPortaudio()
{
    close_if_opened();      // if the stream object is already opened, close it
    if (pa.isInputDeviceChosen && pa.isOutputDeviceChosen) { // only if both intput...
            Pa_OpenStream(                // ...and output devices has already been ...
              &pa.audio,                  // ...chosen, open the stream object 
              &pa.inputParameters, 
              &pa.outputParameters,
              SAMPLING_RATE,
              FRAMES_PER_BUFFER,                     
              paClipOff,            // no clip 
              audio_callback,       // callback function
              NULL );               // no data for the callback
    }
}

void play()
{
    if(pa.audio != 0) {         // check if the audio stream object is already opened
        Pa_StartStream(pa.audio); // start playing
        pa.isPlaying=true;      // inform that the stream object is currently playing
    }
    else if (pa.isInputDeviceChosen && pa.isOutputDeviceChosen) { // only if both intput...
        openPortaudio(); // ...and output devices has already been chosen, open the stream...
        Pa_StartStream(pa.audio);   //...and start playing
        pa.isPlaying=true;          // inform that the stream object is currently playing
    }
    else
        fl_message("You have to choose the audio input and audio output ports prior to play!");
}

void stop()
{
    if(pa.isPlaying) {              // check if the stream object is really playing
        Pa_StopStream(pa.audio);    // stop it
        pa.isPlaying = false;       // inform that the stream object is currently stopped
    }
}

int main()
{
    init_stuff();           // initialize all the stuff
    gui = new RingModGUI;   // create a GUI object
    gui->show();            // show the GUI object
    Fl::run();              // run the FLTK thread and block this function till the end of GUI
    terminate_stuff();      // cleanup all the stuff
    delete gui;             // destroy the GUI object. Not really necessary, but...
    return 0;
}

