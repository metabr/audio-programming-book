//
//	CSoundBugSimpleTest
//	version 1.0
//

#ifndef __CSoundBugSimpleTest__
#define __CSoundBugSimpleTest__

///////////////////////////////////////////////// class definition

class CSoundBugSimpleTest {
	
public:
	
	// constructor & destructor
	CSoundBugSimpleTest();
	~CSoundBugSimpleTest();
	
	// test
	void		Test(const char *outfile, double freqMin, double freqMax, double freedomFactor, double duration);
};


#endif		// __CSoundBugSimpleTest__