#include <iostream>
#include <stdlib.h>

#include "CRandShaperSimpleTest.h"

int main (int argc, char * const argv[]) {
    
	// help message
	std::cout << "randShaper: sound synthesis using sine wave and random waveshaper\n";
	if (argc < 4) {
		std::cout	<< "insufficient arguments." << std::endl
					<< "usage:" << std::endl
					<< "\trandShaper outfile frequency stepsize duration" << std::endl
					<< "\twhere frequency = Hz, stepsize = [0, 0.01], duration = seconds" << std::endl;
		return 1;
	}
	
	// seed the random number generator with the current time
	srand((unsigned int) time(NULL));
	
	// simple test
	float	freq = atof(argv[2]);
	float	step = atof(argv[3]);
	float	dur = atof(argv[4]);

	CRandShaperSimpleTest * mTest = new CRandShaperSimpleTest();
	mTest->Test(argv[1], freq, step, dur);

	return 0;
}
