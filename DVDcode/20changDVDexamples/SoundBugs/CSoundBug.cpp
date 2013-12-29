//
//	CSoundBug class
//	version 1.0
//

#include <stdlib.h>

#include "CSoundBug.h"

///////////////////////////////////////////////// constructor & destructor

CSoundBug::CSoundBug(CSoundBug *leader, double sampleRate) {
	mLeader = leader;
	
	// create a sine wave oscillator
	mSineWave = new CSineWave2(sampleRate);
}

CSoundBug::~CSoundBug() {
	if (mSineWave) delete(mSineWave);
}

///////////////////////////////////////////////// properties

double
CSoundBug::GetFreedomFactor() {
	return(mFreedomFactor);
}

void
CSoundBug::SetFreedomFactor(double freedomFactor) {
	mFreedomFactor = freedomFactor;
}

double
CSoundBug::GetFreq() {
	return(mSineWave->GetFreq());
}

void
CSoundBug::SetFreq(double freq) {
	mSineWave->SetFreq(freq);
}

double
CSoundBug::GetAmp() {
	return(mSineWave->GetAmp());
}

void
CSoundBug::SetAmp(double amp) {
	mSineWave->SetAmp(amp);
}

///////////////////////////////////////////////// synthesis control

void
CSoundBug::Init(double freqMin, double freqMax) {
	mFreqMin = freqMin;
	mFreqMax = freqMax;
	
	// set initial frequency
	if (mLeader == NULL)		// the first leader?
		mSineWave->SetFreq(mFreqMin + (mFreqMax - mFreqMin) * ((double) rand()) / (RAND_MAX + 1.0));
	else
		mSineWave->SetFreq(mLeader->GetFreq());		// same as the leader's freq
}

double
CSoundBug::ProcessOne() {
	double		result = mSineWave->ProcessOne();

	// change frequency
	if (mLeader == NULL) {
		// I'm the first one
		double newFreq = mSineWave->GetFreq()
		+ 0.05 * (mFreqMax - mFreqMin) * ((((double) rand()) / ((double) RAND_MAX)) - 0.5);
		
		// check the boundary
		if (newFreq < mFreqMin)
			newFreq = mFreqMin;
		else if (newFreq > mFreqMax)
			newFreq = mFreqMax;
		
		mSineWave->SetFreq(newFreq);
	}
	else if (mSineWave->IsPeriodDone()) {
		if (((double) rand()) / (RAND_MAX + 1.0) > 0.9 + 0.1 * mFreedomFactor) {
			mSineWave->SetFreq(mLeader->GetFreq());
		}
	}

	return(result);
}
