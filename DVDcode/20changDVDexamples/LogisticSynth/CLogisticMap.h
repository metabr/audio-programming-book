#ifndef __CLogisticMap__
#define __CLogisticMap__

//	logistic map :
//
//		x_new = R * x_old * (1.0 – x_old)

///////////////////////////////////////////////// class definition

class CLogisticMap {

protected:

	// map parameters
	double		mR;				// 'R' value
	double		mX;				// current X value

public:
	
	// constructor
	CLogisticMap(double r = 0, double x = 0);
	
	// map parameters
	double		GetR();
	void		SetR(double r);
	double		GetX();
	void		SetX(double x);
	
	// process
	double		Step();		// calculate & return the next value
};


#endif		// __CLogisticMap__