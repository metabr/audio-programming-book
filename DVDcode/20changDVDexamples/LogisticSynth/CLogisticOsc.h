#ifndef __CLogisticOsc__
#define __CLogisticOsc__

#include "CLogisticMap.h"

///////////////////////////////////////////////// class definition

class CLogisticOsc {

protected:

	// logistic map object
	CLogisticMap *		mMap;
	
	// synthesis
	double			mFreq;
	double			mPeriod;
	double			mSampleRate;
	
	// interpolation
	double			mCurrent;
	double			mStart;
	double			mEnd;
	double			mDelta;
	double			mPeriodOffset;
	

public:

	// constructor & destructor
	CLogisticOsc(double sampleRate);
	~CLogisticOsc();
	
	// synthesis parameters
	double		GetFreq();
	void		SetFreq(double freq);

	// map parameters
	double		GetR();
	void		SetR(double r);
	double		GetX();
	void		SetX(double x);
	
	// synthesis control
	void		Init();
	void		Process(float *bufferPtr, long bufferSize);
};


#endif		// __CLogisticOsc__