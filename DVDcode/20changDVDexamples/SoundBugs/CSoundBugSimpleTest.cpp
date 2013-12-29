//
//	CSoundBugSimpleTest
//	version 1.0
//

#include <iostream>

#include "CSoundBugSimpleTest.h"
#include "portsf.h"

#include "CSoundBug.h"

///////////////////////////////////////////////// constants

#define		kSampleRate		44100
#define		kNumBugs		10

///////////////////////////////////////////////// constructors & destructor

CSoundBugSimpleTest::CSoundBugSimpleTest() {
}

CSoundBugSimpleTest::~CSoundBugSimpleTest() {
}

///////////////////////////////////////////////// test

void
CSoundBugSimpleTest::Test(const char *outfile, double freqMin, double freqMax, double freedomFactor, double duration) {
	CSoundBug			*bugs[kNumBugs];	// sound bugs
	int					err;				// error code
	PSF_PROPS			props;				// sound file properties
	int					sfd = -1;			// sound file descriptor
	float				*buffer;			// sound buffer
	long				numSamples;			// the number of samples
	
	// clean up first
	for (int i = 0; i < kNumBugs; i++) {
		bugs[i] = NULL;
	}
	buffer = NULL;
	
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
	
	// create a sound file
	sfd = psf_sndCreate(outfile, &props, 0, 0, PSF_CREATE_RDWR);
	if (sfd < 0) {
		std::cout	<< "error : unable to create output sound file" << std::endl;
		goto exit;
	}
	
	// sound buffer setup
	numSamples = kSampleRate * duration * props.chans;		// in this code, props.chans is 1
	buffer = (float *) malloc(numSamples * sizeof(float));
	if (buffer == NULL) {
		std::cout	<< "error : unable to allocate memory" << std::endl;
		goto exit;
	}
	
	// fill the buffer with zero
	for (long index = 0; index < numSamples; index++) {
		buffer[index] = 0;
	}
	
	//================================================= core part : begin
	
	// create sound bugs
	bugs[0] = new CSoundBug(NULL, kSampleRate);			// the first bug has no leader
	for (int i = 1; i < kNumBugs; i++) {
		bugs[i] = new CSoundBug(bugs[i-1], kSampleRate);	// each bug will follow the previous bug
	}
	
	// init the bugs
	for (int i = 0; i < kNumBugs; i++) {
		bugs[i]->Init(freqMin, freqMax);
		bugs[i]->SetFreedomFactor(freedomFactor);
		bugs[i]->SetAmp(1. / kNumBugs);		// the cheapest way to prevent audio being clipped
	}

	// process
	for (long index = 0; index < numSamples; index++) {
		buffer[index] = 0;
		for (int i = 0; i < kNumBugs; i++) {
			buffer[index] += bugs[i]->ProcessOne();
		}
	}
	
	//================================================= core part : end
	
	// write to the file
	psf_sndWriteFloatFrames(sfd, buffer, numSamples);
	
exit:
	// kill the bugs
	for (int i = 0; i < kNumBugs; i++) {
		if (bugs[i]) delete bugs[i];
	}
	
	// dispose the buffer
	if (buffer != NULL) free(buffer);
	
	// close the sound file
	if (sfd >= 0) psf_sndClose(sfd);
	
	// close portsf
	psf_finish();
}