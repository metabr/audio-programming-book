#ifndef __CLogisticOscSimpleTest__
#define __CLogisticOscSimpleTest__

#include "CLogisticOsc.h"

///////////////////////////////////////////////// class definition

class CLogisticOscSimpleTest {

public:

	// constructor
	CLogisticOscSimpleTest();
	
	// test
	void	Test(const char * outfile, double freq, double R, double initial_X, double duration);
};


#endif		// __CLogisticOscSimpleTest__