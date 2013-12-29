//
//	CSineWave2 class
//	version 1.0
//
//		This is a simple sinusoidal wave generator.
//		This is an advanced version of CSineWave class included in RandShaper project.
//		Now you won't get clicks when changing the frequency and the amplitude at any time.
//		Note, however, that you may still get clicks when changing the phase.
//

#include <math.h>

#include "CSineWave2.h"

const double	kTwoPi = 2.0 * M_PI;

///////////////////////////////////////////////// constructor & destructor

CSineWave2::CSineWave2(double sampleRate) {
	mFreq = 440.0;
	mAmp = 0;
	mPhase = 0;
	mSampleRate = sampleRate;
	
	mFreqChanged = false;
	mAmpChanged = false;
}

CSineWave2::~CSineWave2() {
}

///////////////////////////////////////////////// properties

double
CSineWave2::GetFreq() {
	return(mFreq);
}

void
CSineWave2::SetFreq(double freq) {
	mNextFreq = freq;
	mFreqChanged = true;
}

double
CSineWave2::GetAmp() {
	return(mAmp);
}

void
CSineWave2::SetAmp(double amp) {
	mNextAmp = amp;
	mAmpChanged = true;
}

double
CSineWave2::GetPhase() {
	return(mPhase);
}

void
CSineWave2::SetPhase(double phase) {
	mPhase = phase;
}

///////////////////////////////////////////////// synthesis control

void
CSineWave2::Init() {
	// nothing to do in this method
	// NOTE :	the frequency, the amplitude, and the phase should be set before Process()
}

void
CSineWave2::Process(double *bufferPtr, long bufferSize) {
	for (long index = 0; index < bufferSize; index ++) {
		bufferPtr[index] = this->ProcessOne();
	}
}

double
CSineWave2::ProcessOne() {
	// current sine wave value
	double result = mAmp * sin(mPhase);

	// update phase
	mPhase += mFreq * kTwoPi / mSampleRate;

	// check if a period is over
	if (mPhase >= kTwoPi) {
		// wrapping the phase
		mPhase -= kTwoPi;
		
		// change the frequency if requested
		if (mFreqChanged) {
			mFreqChanged = false;
			mFreq = mNextFreq;
		}
		
		// change the amplitude if requested
		if (mAmpChanged) {
			mAmpChanged = false;
			mAmp = mNextAmp;
		}
		
		// set periodDone if requested
		mPeriodDone = true;
	}
	else
		mPeriodDone = false;

	return result;
}
bool
CSineWave2::IsPeriodDone() {
	return mPeriodDone;
}
