/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

//waveoscil.cpp
#include <cmath>
#include "waveoscil.h"

using namespace audiobook;

float sine_oscil::tick(double freq)
{	
	setPhasorFreq(freq);
	double val = std::sin(getPhase());
	updatePhasor();
	return static_cast<float>(val);
}

float triangle_oscil::tick(double freq)
{
	setPhasorFreq(freq);
	double val;
	val = (2.0 * getPhase() * (1.0 / TWOPI)) - 1.0;
	if(val < 0.0)
		val  = -val;
	val = 2.0 * (val - 0.5);
	updatePhasor();
	return static_cast<float>(val);
}

float square_oscil::tick(double freq)
{
    setPhasorFreq(freq);
	double val;
	val = getPhase() <= M_PI ? 1.0 : -1.0;
	updatePhasor();
	return static_cast<float>(val);
}

float sawdown_oscil::tick(double freq)
{
	setPhasorFreq(freq);
	double val;
	val = 1.0 - 2.0 * getPhase() * 1.0 / TWOPI;
	updatePhasor();
	return static_cast<float>(val);
}

float sawup_oscil::tick(double freq)
{
	setPhasorFreq(freq);
	double val;
	val = (2.0 * getPhase() * 1.0 / TWOPI) - 1.0;
	updatePhasor();
	return static_cast<float>(val);
}

buzz_oscil::buzz_oscil() : nharms_(0)
{
//	nharms_ = 0;
}
void buzz_oscil::init(double sr)
{
	phasor::init(sr);
	srate_ = sr;
	nharms_ = 1;
	tnp1_ = nharms_ * 2 + 1;
	over2n_ = 0.5 / nharms_;
}

float buzz_oscil::tick(double freq)
{
	double val,num,numphase;
	// set freq /= 2 for buzz algorithm
	if(setPhasorFreq(freq * 0.5)){
		nharms_ = static_cast<unsigned long>(srate_/freq/2.0);
		tnp1_ = nharms_ * 2 + 1;
		over2n_ = 0.5 / nharms_;
	}

	val = std::sin(getPhase());
	if (val > 0.00001 || val < -0.00001) {
		numphase = std::fmod(getPhase() * tnp1_,TWOPI);
        num = std::sin(numphase);  
        val = (num / val  - 1.0)  * over2n_;
     }
	 else
		val = 1.0;
	updatePhasor();
	return static_cast<float>(val); 

}


