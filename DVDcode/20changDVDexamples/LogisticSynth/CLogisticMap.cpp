#include "CLogisticMap.h"

///////////////////////////////////////////////// constructor

CLogisticMap::CLogisticMap(double r, double x) {
	mR = r;
	mX = x;
}

///////////////////////////////////////////////// map parameters

double
CLogisticMap::GetR() {
	return mR;
}

void
CLogisticMap::SetR(double r) {
	mR = r;
}

double
CLogisticMap::GetX() {
	return mX;
}

void
CLogisticMap::SetX(double x) {
	mX = x;
}

///////////////////////////////////////////////// output

double
CLogisticMap::Step() {
	mX = mR * mX * (1.0 - mX);
	return mX;
}

