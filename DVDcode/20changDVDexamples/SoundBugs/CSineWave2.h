//
//	CSineWave2 class
//	version 1.0
//
//		This is a simple sinusoidal wave generator.
//		This is an advanced version of CSineWave class included in RandShaper project.
//		Now you won't get clicks when changing the frequency and the amplitude at any time.
//		Note, however, that you may still get clicks when changing the phase.
//

#ifndef __CSineWave2__
#define __CSineWave2__

///////////////////////////////////////////////// class definition

class CSineWave2 {

protected:

	// settings
	double		mFreq;
	double		mAmp;
	double		mPhase;
	double		mSampleRate;

	// for click-free
	double		mNextFreq;
	bool		mFreqChanged;
	double		mNextAmp;
	bool		mAmpChanged;
	
	// misc
	bool		mPeriodDone;

public:
	
	// constructor and destructor
	CSineWave2(double sampleRate);
	~CSineWave2();

	// properties
	double		GetFreq();
	void		SetFreq(double freq);
	double		GetAmp();
	void		SetAmp(double amp);
	double		GetPhase();
	void		SetPhase(double phase);
	
	// synthesis control
	void		Init();
	void		Process(double *bufferPtr, long bufferSize);	// synthesize many samples
	double		ProcessOne();									// synthesize just one sample
	bool		IsPeriodDone();			// use this right after ProcessOne()
};

#endif		// __CSineWave2__