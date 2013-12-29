#include "CLogisticOsc.h"

///////////////////////////////////////////////// constructor & destructor

CLogisticOsc::CLogisticOsc(double sampleRate) {
	mSampleRate = sampleRate;
	
	// create a logistic map object (CLogisticMap)
	mMap = new CLogisticMap();
}

CLogisticOsc::~CLogisticOsc() {
	delete mMap;
}

///////////////////////////////////////////////// synthesis parameters

double
CLogisticOsc::GetFreq() {
	return mFreq;
}

void
CLogisticOsc::SetFreq(double freq) {
	mFreq = freq;
	mPeriod = mSampleRate / ((double) mFreq);
}

///////////////////////////////////////////////// map parameters

double
CLogisticOsc::GetR() {
	return mMap->GetR();
}

void
CLogisticOsc::SetR(double r) {
	mMap->SetR(r);
}

double
CLogisticOsc::GetX() {
	return mMap->GetX();
}

void
CLogisticOsc::SetX(double x) {
	mMap->SetX(x);
}

///////////////////////////////////////////////// synthesis control

void
CLogisticOsc::Init() {
	mStart = mMap->GetX();					// fetch the initial X value
	mEnd = mMap->Step();					// fetch the next X value
	mDelta = (mEnd - mStart) / mPeriod;		// calculate the linear interpolation factor
	mCurrent = mStart;
}

void
CLogisticOsc::Process(float *bufferPtr, long bufferSize) {
	for (long index = 0; index < bufferSize; index ++) {
		bufferPtr[index] = mCurrent;		// current sample
		mCurrent += mDelta;					// calculate the next interpolated sample
		mPeriodOffset ++;

		// if a period is done, fetch the next logistic map value, and set up the interpolation
		if (mPeriodOffset > mPeriod) {
			mStart = mEnd;
			mEnd = mMap->Step();
			mDelta = (mEnd - mStart) / mPeriod;
			mCurrent = mStart;
			mPeriodOffset -= mPeriod;
		}
	}
}
