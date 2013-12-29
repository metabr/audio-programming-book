#include <iostream>
#include "CLogisticOscSimpleTest.h"

int main (int argc, char * const argv[]) {

	// help message
	std::cout << "logisticSynth: sound synthesis using the logistic map\n";
	if (argc < 6) {
		std::cout	<< "insufficient arguments." << std::endl
					<< "usage:" << std::endl
					<< "\tlogisticSynth outfile frequency R initial_X duration" << std::endl
					<< "\twhere frequency = Hz, R = [0,4], initial_X = (0,1), duration = seconds" << std::endl;
		return 1;
	}
	
	// simple test
	double		freq = atof(argv[2]);
	double		R = atof(argv[3]);
	double		initial_X = atof(argv[4]);
	double		duration = atof(argv[5]);
	
	CLogisticOscSimpleTest * mTest = new CLogisticOscSimpleTest();
	mTest->Test(argv[1], freq, R, initial_X, duration);

	return 0;
}
