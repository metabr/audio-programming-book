#ifndef __main_prot__
#define __main_prot__

#include "portaudio.h"

struct PA_STUFF {       // a structure containing some items accessed by several files
    PaStream *audio;    // the audio stream object pointer
    PaStreamParameters outputParameters, inputParameters; // Portaudio parameter structures
    const PaDeviceInfo  *info;    // a pointer to a Portadio device information structure
    const PaHostApiInfo *hostapi; // a pointer t host API information structure

    char s[256];        // a string buffer to temporary store several text
    bool isInputDeviceChosen;   // inform if an input device has been chosen
    bool isOutputDeviceChosen;  // inform if an output device has been chosen
    bool isPlaying;             // inform if Portaudio stream is playing
    PA_STUFF() {audio = 0;}     // the constructor of this structure
};

extern PA_STUFF pa;             // a declaration of a PA_STUFF object as external
void initFlags();               // function prototypes
void getAvailableInputs();
void getAvailableOutputs();
void openInput();
void openOutput();
void play();
void stop();

#endif


