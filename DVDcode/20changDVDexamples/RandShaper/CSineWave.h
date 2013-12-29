//
//	CSineWave class
//	version 1.0
//
//		This is a very simple sinusoidal wave generator.
//		Note that you may get clicks when changing the frequency, the amplitude, or the phase.
//		To avoid it, you have to add some more codes.
//

#ifndef __CSineWave__
#define __CSineWave__

///////////////////////////////////////////////// class definition

class CSineWave {

protected:

	// synthesis
	double		mFreq;
	double		mAmp;
	double		mPhase;
	double		mSampleRate;
	
public:

	// constructor
	CSineWave(double sampleRate);

	// settings
	double		GetFreq();
	void		SetFreq(double freq);
	double		GetAmp();
	void		SetAmp(double amp);
	double		GetPhase();
	void		SetPhase(double phase);
	
	// synthesis control
	void		Init();
	void		Process(float * bufferPtr, long bufferSize);
};

#endif		// __CSineWave__