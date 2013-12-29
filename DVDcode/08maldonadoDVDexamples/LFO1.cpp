#include "LFO1.h"

double	TWO_PI  (3.14159265f * 2.0f);

LFO1 *lfoApp;	// the pointer of the application object is declared as 
				// global because must be accessed by LFO1_callback()

inline void LFO1::synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer) 
{		// the audio callback implementation, belonging to the LFO1 class
	double outLFO = sin(phaseLFO) * ampLFO;			// get the ouput of the LFO
	double siLFO = TWO_PI * freqLFO / CONTROL_RATE; // get the LFO sampling increment
	phaseLFO += siLFO;								// increment LFO phase
	
	double frequencyCarrier = getCarrierFreq() + outLFO; // get the actual frequency of the carrier
	double siCarrier = TWO_PI * frequencyCarrier / SAMPLING_RATE; // obtain carrier sampling increment

	float *out = outputBuffer;
	// ------- the audio engine loop -----------
	for( unsigned long i=0; i < framesPerBuffer; i++) {	
		float outCarrier = (float) sin(phaseCarrier); // get the output of the carrier
		*out++ = outCarrier;		// stream to the stereo ouptut (left channel)
		*out++ = outCarrier;		// stream to the stereo ouptut (right channel)
		phaseCarrier += siCarrier;	// increment the phase of the carrier
    }
	// ------- end of audio engine loop -------- 
}

// the audio callback function adapter. It is periodically called by the Portaudio mechanism
int LFO1_callback( const void *inBuf, void *outBuf, U_LONG framesPerBuf, 
		const PaStreamCallbackTimeInfo* tInfo, PaStreamCallbackFlags flags, void *userData )
{	lfoApp->synthesis_callback((float *) outBuf, framesPerBuf);
	return paContinue;
}

LFO1::LFO1() // constructor of the application class
{
	SAMPLING_RATE = 44100;
	FRAMES_PER_BUFFER = 256;
	CONTROL_RATE = SAMPLING_RATE / FRAMES_PER_BUFFER;
	
	const double frqMinLFO = .1, frqMaxLFO = 100; // set min and max frequencies of LFO

	freqLFO = 5;				// set initial frequency of the LFO
	logMin = log(frqMinLFO);	// set log of min LFO freq, for speeding-up calculation
	logMax = log(frqMaxLFO);	// set log of max LFO freq, for speeding-up calculation
	ampMax = 300;				// set maximum amplitude value of the LFO
	ampLFO = 0;					// set inital amplitude of the LFO
	phaseLFO = 0;				// set initial phase of the LFO	 
	phaseCarrier = 0;			// set initial phase of the carrier
	audio_callback = LFO1_callback; // set the appropriate audio_callback function
	show();						// show the GUI of the application
}

int main()
{
	lfoApp = new LFO1;	// create the application object
	Fl::run();			// run the FLTK GUI (and the application object)
	delete lfoApp;		// delete the application object
	return 0;
}

