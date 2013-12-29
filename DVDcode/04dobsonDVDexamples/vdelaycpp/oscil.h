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

#ifndef __SINEOSCIL__
#define __SINEOSCIL__

#include <cmath>

#ifndef M_PI
#define M_PI (3.1415926535897932)
#endif

namespace audiobook {

    class sine_oscil {
    public:
            sine_oscil()  : twopiovrsr_(0.0), curphase_(0.0), 
                       incr_(0.0) ,curfreq_(0.),TWOPI_(2.0 * M_PI)  {} 
        virtual ~sine_oscil()	{}
        void init(double sr) {
            if(sr > 0.0) 
                twopiovrsr_ = TWOPI_ / sr;
        }
        double tick(double freq) {
            double val;
            val = sin(curphase_);
            if(curfreq_ != freq){
                curfreq_ = freq;
                incr_ = twopiovrsr_ * curfreq_;
            }
            curphase_ += incr_;
            wrapphase();
            return val;
        }
    private:
        void wrapphase() {
            if(curphase_ > TWOPI_)
                curphase_ -= TWOPI_;
            if(curphase_ < 0.0)
                curphase_ += TWOPI_;
        }
        double twopiovrsr_;
        double curphase_;
        double incr_;
        double curfreq_;
        const double TWOPI_;
    };
}
           
#endif
        