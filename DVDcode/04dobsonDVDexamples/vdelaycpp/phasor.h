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

// phasor.h: definition of oscillator base class
#ifndef _PHASOR_H
#define _PHASOR_H

namespace audiobook {
#ifndef M_PI
static const double M_PI(3.141592653589793239);
#endif
static const double TWOPI(2.0 * M_PI);

class phasor
{
public:
	virtual ~phasor()  {}
	virtual void init(double srate)
               { twopiovrsr_ = (TWOPI / srate); }
	virtual float tick(double freq) = 0;
protected:
    
	// may only create a derived class
	phasor() : twopiovrsr_(0.0), curfreq_(0.0), 
                curphase_(0.0), incr_(0.0) {}	
	double getPhase(void)	const {return curphase_;}
    // we ~hope~ the compiler will actually inline these!
	bool   setPhasorFreq(double freq) {
        bool updated(false);
        if(curfreq_ != freq){
            curfreq_ = freq;
            incr_ = twopiovrsr_ * freq;
            updated = true;
        }
        return updated;
    }
	void   updatePhasor() {
        curphase_ += incr_;
        if(curphase_ >= TWOPI)
            curphase_ -= TWOPI;
        if(curphase_ < 0.0)
            curphase_ += TWOPI;
    }
private:
	double twopiovrsr_;
	double curfreq_;
	double curphase_;
	double incr_;
};

}
#endif
