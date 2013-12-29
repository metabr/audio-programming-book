#include <iostream>

#include "CRandShaperSimpleTest.h"
#include "portsf.h"

#include "CSineWave.h"
#include "CRandShaper.h"

///////////////////////////////////////////////// constants

#define		kSampleRate		44100

///////////////////////////////////////////////// constructor

CRandShaperSimpleTest::CRandShaperSimpleTest()
{
}

///////////////////////////////////////////////// test

void
CRandShaperSimpleTest::Test(const char * outfile, double freq, double stepsize, double duration) {
	CSineWave *			osc = 0;		// sine wave oscillator
	CRandShaper *		shaper = 0;		// shaping function
	int					err;			// error code
	PSF_PROPS			props;			// sound file properties
	int					sfd;			// sound file descriptor
	float				*buffer = 0;	// sound buffer
	long				numSamples;		// the number of samples
	
	// portsf startup
	err = psf_init();
	if (err) {
		std::cout	<< "error : unable to start the portsf" << std::endl;
		return;
	}
	
	// sound file properties setup
	props.srate = kSampleRate;		// sampling rate = kSampleRate
	props.samptype = PSF_SAMP_16;	// sampling size = 16 bits
	props.chans = 1;				// num of channels = 1 (mono)
	props.format = PSF_STDWAVE;		// sound file format = standard WAVE
	props.chformat = STDWAVE;		// speaker configuration
	
	// creating a sound file
	sfd = psf_sndCreate(outfile, &props, 0, 0, PSF_CREATE_RDWR);
	if (sfd < 0) {
		std::cout	<< "error : unable to create output sound file" << std::endl;
		goto exit;
	}
	
	// preparing the sound buffer
	numSamples = kSampleRate * duration * props.chans;		// in this code, props.chans is 1
	buffer = (float *) malloc(numSamples * sizeof(float));
	if (buffer == NULL) {
		std::cout	<< "error : unable to allocate memory" << std::endl;
		goto exit;
	}
	
	// *** oscilator ***
	osc = new CSineWave(kSampleRate);
	osc->SetFreq(freq);
	osc->SetAmp(0.5);
	osc->Init();
	osc->Process(buffer, numSamples);
	
	// *** shaping ***
	shaper = new CRandShaper(stepsize);
	shaper->Init();
	shaper->Process(buffer, numSamples);
	
	// write to the file
	psf_sndWriteFloatFrames(sfd, buffer, numSamples);
	
exit:
	if (shaper) delete shaper;
	if (osc) delete osc;
	
	// dispose the buffer
	if (buffer != NULL) free(buffer);
	
	// closing the sound file
	if (sfd >= 0) psf_sndClose(sfd);
	
	// closing portsf
	psf_finish();
}