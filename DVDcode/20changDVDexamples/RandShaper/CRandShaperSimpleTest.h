#ifndef __CRandShaperSimpleTest__
#define __CRandShaperSimpleTest__

///////////////////////////////////////////////// class definition

class CRandShaperSimpleTest
{
	/******* METHODS *******/
	public:
	
		// constructor & destructor
		CRandShaperSimpleTest();
		~CRandShaperSimpleTest();
		
		// test
		void		Test(const char * outfile, double freq, double stepsize, double duration);
};


#endif		// __CRandShaperSimpleTest__