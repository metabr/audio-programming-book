#include "CRandShaper.h"
#include <stdlib.h>

///////////////////////////////////////////////// constructor

CRandShaper::CRandShaper(double walkStepSize) {
	mWalkStepSize = walkStepSize;
	mWait = 0;
}

///////////////////////////////////////////////// settings

double
CRandShaper::GetWalkStepSize() {
	return mWalkStepSize;
}

void
CRandShaper::SetWalkStepSize(double size) {
	mWalkStepSize = size;
}

///////////////////////////////////////////////// processing

void
CRandShaper::Init() {
	// initialize the shaping function (make it linear)
	for (int index = 0; index < kNumPoints; index ++) {
		mPointArray[index].x = 2.0 * (index + 1.0) / (kNumPoints + 1.0) - 1.0;
		mPointArray[index].y = mPointArray[index].x;
	}
}

void
CRandShaper::Process(float * bufferPtr, long bufferSize) {
	short		index;
	double		value;
	
	while (bufferSize--) {
		value = *bufferPtr;
		
		/////// linear interpolation shaping ///////
		
		// input is between (-1, -1) and the first breakpoint
		if (value <= mPointArray[0].x) {
			*bufferPtr = (float) (value + 1.0) * (mPointArray[0].y + 1.0)
				/ (mPointArray[0].x + 1.0) - 1.0;
		}
		// input is between the last breakpoint and (+1, +1)
		else if (value > mPointArray[kNumPoints - 1].x) {
			*bufferPtr = (float) (value - mPointArray[kNumPoints - 1].x)
				* (1.0 - mPointArray[kNumPoints - 1].y)
				/ (1.0 - mPointArray[kNumPoints - 1].x) + mPointArray[kNumPoints - 1].y;
		}
		// input is between two breakpoints
		else {
			for (index = 1; index < kNumPoints; index ++) {
				if (value <= (mPointArray[index]).x) {
					*bufferPtr = (float) (value - mPointArray[index - 1].x)
						* (mPointArray[index].y - mPointArray[index - 1].y)
						/ (mPointArray[index].x - mPointArray[index - 1].x)
						+ mPointArray[index - 1].y;
					break;
				}
			}
		}
		
		// prepare next
		bufferPtr++;
		if(mWait < kWaitForShapingWalk) {
			mWait++;
		}
		else {
			for (index = 0; index < kNumPoints; index ++) {
				mPointArray[index].x += mWalkStepSize * (drand48() * 2.0 - 1.0);
				mPointArray[index].y += mWalkStepSize * (drand48() * 2.0 - 1.0);
				if (index == 0) {
					// the first point
					if (mPointArray[0].x < (-1.0 + kMinDistance)) mPointArray[0].x = -1.0 + kMinDistance;
					if (mPointArray[0].x > (mPointArray[1].x - kMinDistance)) mPointArray[0].x = mPointArray[1].x - kMinDistance;
					if (mPointArray[0].y < (-1.0 + kMinDistance)) mPointArray[0].y = -1.0 + kMinDistance;
					if (mPointArray[0].y > (mPointArray[1].y - kMinDistance)) mPointArray[0].y = mPointArray[1].y - kMinDistance;
				}
				else if (index == kNumPoints - 1) {
					// the last point
					if (mPointArray[index].x < (mPointArray[index - 1].x + kMinDistance)) mPointArray[index].x = mPointArray[index - 1].x + kMinDistance;
					if (mPointArray[index].x > (1.0 - kMinDistance)) mPointArray[index].x = 1.0 - kMinDistance;
					if (mPointArray[index].y < (mPointArray[index - 1].y + kMinDistance)) mPointArray[index].y = mPointArray[index - 1].y + kMinDistance;
					if (mPointArray[index].y > (1.0 - kMinDistance)) mPointArray[index].y = 1.0 - kMinDistance;
				}
				else {
					// the other points
					if (mPointArray[index].x < (mPointArray[index - 1].x + kMinDistance)) mPointArray[index].x = mPointArray[index - 1].x + kMinDistance;
					if (mPointArray[index].x > (mPointArray[index + 1].x - kMinDistance)) mPointArray[index].x = mPointArray[index + 1].x - kMinDistance;
					if (mPointArray[index].y < (mPointArray[index - 1].y + kMinDistance)) mPointArray[index].y = mPointArray[index - 1].y + kMinDistance;
					if (mPointArray[index].y > (mPointArray[index + 1].y - kMinDistance)) mPointArray[index].y = mPointArray[index + 1].y - kMinDistance;
				}
			}
			mWait = 0;
		}
	}
}
