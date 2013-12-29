//
//	CSoundBug class
//	version 1.0
//

#ifndef __CSoundBug__
#define __CSoundBug__

#include "CSineWave2.h"

///////////////////////////////////////////////// class definition

class CSoundBug {
	
protected:

	// leader bug
	CSoundBug		*mLeader;
	double			mFreedomFactor;
	
	// sound
	double			mFreqMin;
	double			mFreqMax;
	CSineWave2		*mSineWave;
		
public:
	
	// constructor and destructor
	CSoundBug(CSoundBug *leader, double sampleRate);
	~CSoundBug();

	// properties
	double		GetFreedomFactor();
	void		SetFreedomFactor(double freedomFactor);
	double		GetFreq();
	void		SetFreq(double freq);
	double		GetAmp();
	void		SetAmp(double amp);
	
	// synthesis control
	void		Init(double freqMin, double freqMax);
	double		ProcessOne();
};

#endif		// __CSoundBug__