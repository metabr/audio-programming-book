#include "portaudio.h"
#include "PortAudioGUI.h"
#include "PortAudioStuff.h"
#include <FL/fl_ask.H>  // an FLTK header for fl_message()
#include <cstdio>

PAstuff::PAstuff(int numOutChans, int numInChans) 
{
    numInChannels = numInChans;
    numOutChannels = numOutChans;
    if (numInChannels < 1) { // if no input channel, deactivate corresponding items
		mnuChooseInput->label("(input not available)");
		inDevMenu->label("(input not available)");
		mnuChooseInput->deactivate();
		inDevMenu->deactivate();
    }
    audio = 0;
    menuStop->deactivate();         // deactivate corresponding menu item
    menuStart->deactivate();        // deactivate corresponding menu item
    isInputDeviceChosen  = false;   // no input device chosen
    isOutputDeviceChosen = false;   // no output device chosen
    isPlaying = false;              // not playing
    Pa_Initialize(); 
}

PAstuff::~PAstuff()
{
    if (audio != 0) {    // check if the audio stream object is actually opened
        Pa_StopStream( audio );
        Pa_CloseStream( audio );
    }
    Pa_Terminate();
}

void PAstuff::close_if_opened()
{
    if (audio != 0) {            // check if the stream object is already opened
        if (isPlaying) {         // check if the stream object is playing
            Pa_StopStream( audio ); // stop playing
            isPlaying = false;   // inform that the stream object is stopped
        }
        Pa_CloseStream(audio);   // close the stream and
        audio = 0;               // invalidate its pointer
    }
}

void PAstuff::getAvailableInputs()
{
    close_if_opened();      // if the stream object has been opened, close it
    for (int i=0; i < Pa_GetDeviceCount(); i++) {   // traverse all audio devices
        info = Pa_GetDeviceInfo(i);              // get info about this device
        hostapi = Pa_GetHostApiInfo(info->hostApi); // get info about its host API

        if (info->maxInputChannels > 0) { // if this one is an input device
            sprintf(s,"%d: [%s input] %s",i, hostapi->name, info->name );  
            brwDevSelect->add(s, (void *) i); // add it to the list-box items
        }
    }
}

void PAstuff::getAvailableOutputs()
{
    close_if_opened();      // if the stream object is already opened, close it
    for (int i=0; i < Pa_GetDeviceCount(); i++) {   // traverse all audio devices
        info = Pa_GetDeviceInfo(i);              // get info about this device
        hostapi = Pa_GetHostApiInfo(info->hostApi); // get info about its host API
        if (info->maxOutputChannels > 0) { // if this one is an output device
            sprintf( s,"%d: [%s output] %s",i, hostapi->name, info->name );  
            brwDevSelect->add(s, (void *) i); // add it to the list-box items
        }
    }
}

void PAstuff::selectDevice(int i, int data, const char *text)
{
    if (isInput) { 
    if (i<=0) {
        inDevNum = paNoDevice;
        sprintf(inStr,"IN: no device selected");    
    }
    else {
        inDevNum = data; 
        sprintf(inStr,"IN: %s", text);
    }
    inDevMenu->label(inStr);
  }  else {
    if (i<=0) {
        outDevNum = paNoDevice;
        sprintf(inStr,"OUT: no device selected");   
    }
    else {
        outDevNum = data; 
        sprintf(outStr,"OUT: %s", text);
    }
    outDevMenu->label(outStr);
  };
}

void PAstuff::openInput()
{
    if (inDevNum >=0) {                     // if an input device has been chosen
        inputParameters.device = inDevNum;  // fill the inputParameters structure
        inputParameters.channelCount = numInChannels;         // stereo input 
        inputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
        inputParameters.suggestedLatency = info->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = 0;
        isInputDeviceChosen = true;   // inform that an input device has been chosen
        if (isOutputDeviceChosen) menuStart->activate();
    }
    else {
        fl_message("You have to choose an audio input port from the list first!");
        isInputDeviceChosen = false; // inform that no input device has been chosen
    }
}

void PAstuff::openOutput()
{
    if (outDevNum >=0) {                        // if an output device has been chosen
        outputParameters.device = outDevNum;    // fill the outputParameters structure 
        outputParameters.channelCount = numOutChannels; // stereo output 
        outputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
        outputParameters.suggestedLatency = info->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = 0;
        isOutputDeviceChosen = true;  // inform that an output device has been chosen
        if (isInputDeviceChosen || numInChannels<1) menuStart->activate();
    }
    else {
        fl_message("You have to choose an audio output port from the list first!");
        isOutputDeviceChosen = false; // inform that no output device has been chosen
    }
}

void PAstuff::openPortaudio()
{
    close_if_opened();      // if the stream object is already opened, close it
    PaStreamParameters *inParms;
    if (isInputDeviceChosen) 
        inParms = &inputParameters;
    else {
        inParms = 0;
        if (numInChannels > 0) fl_message("No input: streaming output only!");
    }
    
    if (isOutputDeviceChosen) { // only if both intput...
            Pa_OpenStream(      // ...and output devices has already been ...
              &audio,           // ...chosen, open the stream object 
              inParms, 
              &outputParameters,
              SAMPLING_RATE,
              FRAMES_PER_BUFFER,                     
              paClipOff,        // no clip 
              audio_callback,   // callback function (in this case is a function pointer)
              NULL );           // no data for the callback
    }
}

void PAstuff::play()
{
    if(audio != 0) {            // check if the audio stream object is already opened
        Pa_StartStream(audio);  // start playing
        isPlaying=true;         // inform that the stream object is currently playing
    }
    else if (isOutputDeviceChosen) { // only if both intput...
        openPortaudio(); // ...and output devices has already been chosen, open the stream...
        Pa_StartStream(audio);   //...and start playing
        isPlaying=true;          // inform that the stream object is currently playing
    }
    if (isPlaying) {
        mnuChooseInput->deactivate(); 
        mnuChooseOutput->deactivate();
        menuStart->deactivate();
        menuStop->activate();
    }
    else
        fl_message("You have to choose the audio input and audio output ports prior to play!");
}

void PAstuff::stop()
{
    if(isPlaying) {              // check if the stream object is really playing
        Pa_StopStream(audio);    // stop it
        isPlaying = false;       // inform that the stream object is currently stopped
        if (numInChannels>0)  mnuChooseInput->activate();
        mnuChooseOutput->activate();
        menuStart->activate();
        menuStop->deactivate();
   }
}


