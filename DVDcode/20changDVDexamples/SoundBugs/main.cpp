//
//	SoundBugs
//

#include <iostream>
#include <stdlib.h>

#include "CSoundBugSimpleTest.h"

///////////////////////////////////////////////// main

int main(int argc, char * const argv[]) {
	// help message
	std::cout << "soundBugs: sound synthesis using artificial life of sine waves\n";
	if (argc < 6) {
		std::cout	<< "insufficient arguments." << std::endl
		<< "usage:" << std::endl
		<< "\tsoundBugs outfile freqMin freqMax freedomFactor duration" << std::endl
		<< "\twhere duration = seconds" << std::endl;
		return 1;
	}
	
	// seed the random number generator with the current time
	srand((unsigned int) time(NULL));
	
	// simple test
	double	freqMin = atof(argv[2]);
	double	freqMax = atof(argv[3]);
	double	freedomFactor = atof(argv[4]);
	double	duration = atof(argv[5]);
	
	CSoundBugSimpleTest *mTest = new CSoundBugSimpleTest();
	mTest->Test(argv[1], freqMin, freqMax, freedomFactor, duration);

	return 0;
}
