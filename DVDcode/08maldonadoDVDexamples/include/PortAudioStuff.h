#ifndef __pastuff__
#define __pastuff__
#include "portaudio.h"
#include "PortAudioGUI.h"

typedef unsigned long U_LONG; // abbreviates the digitation of unsigned long

class PAstuff : public PortAudioGUI{       // a class encapsulating Portaudio-related stuff
  protected:
    PaStream *audio;    // the audio stream object pointer
    PaStreamParameters outputParameters, inputParameters; // Portaudio parameter structures  
    int numInChannels, numOutChannels;
    const PaDeviceInfo  *info;    // a pointer to a Portadio device information structure
    const PaHostApiInfo *hostapi; // a pointer t host API information structure
    char s[256];                // string buffers to temporary store several text
    char inStr[256];
    char outStr[256];
    bool isInputDeviceChosen;   // inform if an input device has been chosen
    bool isOutputDeviceChosen;  // inform if an output device has been chosen
    bool isPlaying;             // inform if Portaudio stream is playing

    void close_if_opened();
    void getAvailableInputs();
    void getAvailableOutputs();
    void selectDevice(int i, int data, const char *text);   
    void openInput();
    void openOutput();
    void openPortaudio();
    virtual void play();
    virtual void stop();
  public:
    double SAMPLING_RATE;
    U_LONG FRAMES_PER_BUFFER;
    
    PAstuff(int numOutChans=2, int numInChans=0); // constructor for both input and output
    ~PAstuff();
    int (*audio_callback) ( const void *inBuf, void *outBuf, U_LONG framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData ) ;
};

#endif