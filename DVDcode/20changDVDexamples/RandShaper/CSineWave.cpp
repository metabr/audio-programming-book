//
//	CSineWave class
//	version 1.0
//
//		This is a very simple sinusoidal wave generator.
//		Note that you may get clicks when changing the frequency, the amplitude, or the phase.
//		To avoid it, you have to add some more codes. (see SoundBugs project.)
//

#include "CSineWave.h"
#include <math.h>

const double kTwoPi = 2.0 * M_PI;

///////////////////////////////////////////////// constructor

CSineWave::CSineWave(double sampleRate) {
	mFreq = 440.0;
	mAmp = 0;
	mPhase = 0;
	mSampleRate = sampleRate;
}

///////////////////////////////////////////////// synthesis parameters

double
CSineWave::GetFreq() {
	return(mFreq);
}

void
CSineWave::SetFreq(double freq) {
	mFreq = freq;
}

double
CSineWave::GetAmp() {
	return(mAmp);
}

void
CSineWave::SetAmp(double amp) {
	mAmp = amp;
}

double
CSineWave::GetPhase() {
	return(mPhase);
}

void
CSineWave::SetPhase(double phase) {
	mPhase = phase;
}

///////////////////////////////////////////////// synthesis control

void
CSineWave::Init() {
	// nothing to do in this method
	// NOTE :	the frequency, the amplitude, and the phase should be set before Process()
}

void
CSineWave::Process(float * bufferPtr, long bufferSize) {
	for (long index = 0; index < bufferSize; index ++) {
		// current sine wave value
		bufferPtr[index] = mAmp * sin(mPhase);

		// update phase
		mPhase += mFreq * kTwoPi / mSampleRate;
		if (mPhase > kTwoPi) mPhase -= kTwoPi;
	}
}
