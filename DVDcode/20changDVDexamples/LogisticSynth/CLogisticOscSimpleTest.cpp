#include <iostream>

#include "CLogisticOscSimpleTest.h"
#include "portsf.h"

///////////////////////////////////////////////// constants

#define		kSampleRate		44100

///////////////////////////////////////////////// constructor

CLogisticOscSimpleTest::CLogisticOscSimpleTest() {
}

///////////////////////////////////////////////// test

void
CLogisticOscSimpleTest::Test(const char * outfile, double freq, double R, double initial_X, double duration) {
	CLogisticOsc *		osc = 0;		// logistic oscillator object
	int					err;			// error code
	PSF_PROPS			props;			// sound file properties
	int					sfd;			// sound file descriptor
	float				*buffer = 0;	// sound buffer
	long				numSamples;		// the number of samples
	
	// DEBUG
	std::cout	<< outfile << std::endl
				<< freq << std::endl
				<< R << std::endl
				<< initial_X << std::endl
				<< duration << std::endl;
	
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
	osc = new CLogisticOsc(kSampleRate);
	osc->SetFreq(freq);
	osc->SetR(R);
	osc->SetX(initial_X);
	osc->Init();
	osc->Process(buffer, numSamples);
	
	// write to the file
	psf_sndWriteFloatFrames(sfd, buffer, numSamples);
	
exit:
	if (osc) delete osc;
	
	// dispose the buffer
	if (buffer != NULL) free(buffer);
	
	// closing the sound file
	if (sfd >= 0) psf_sndClose(sfd);
	
	// closing portsf
	psf_finish();
}