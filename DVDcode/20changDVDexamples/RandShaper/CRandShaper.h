#ifndef __CRandShaper__
#define __CRandShaper__

///////////////////////////////////////////////// constants

#define		kNumPoints				5			// the number of breakpoints
#define		kWaitForShapingWalk		10			// waiting time before making the shaping function walk
#define		kMinDistance			0.05		// the minimum distance between breakpoints

///////////////////////////////////////////////// structure definition

struct BreakPoint {
	double		x;
	double		y;
};
typedef struct BreakPoint		BreakPoint;

///////////////////////////////////////////////// class definition

class CRandShaper {

protected:

	// settings
	BreakPoint		mPointArray[kNumPoints];	// the list of breakpoints
	double			mWalkStepSize;				// the maximum step size
	int				mWait;

public:

	// constructor
	CRandShaper(double walkStepSize = 0.0001);
	
	// settings
	double		GetWalkStepSize();
	void		SetWalkStepSize(double size);
	
	// processing
	void		Init();
	void		Process(float * bufferPtr, long bufferSize);
};


#endif		// __CRandShaper__