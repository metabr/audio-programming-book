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

//waveoscil.h: waveform oscillators derived from phasor
#ifndef _WAVEOSCIL_H
#define _WAVEOSCIL_H

#include "phasor.h"

namespace audiobook {
    class sine_oscil : public phasor
    {
    public:
        sine_oscil() {}
         ~sine_oscil() {}
        float tick(double freq);
    };

    class triangle_oscil : public phasor
    {
    public:
        triangle_oscil() {}
         ~triangle_oscil() {}
        float tick(double freq);
    };

    class square_oscil : public phasor
    {
    public:
        square_oscil() {}
         ~square_oscil() {}
        float tick(double freq);
    };

    class sawdown_oscil : public phasor
    {
    public:
        sawdown_oscil() {}
        ~sawdown_oscil(){}
        float tick(double freq);
    };

    class sawup_oscil : public phasor
    {
    public:
        sawup_oscil() {}
         ~sawup_oscil(){}
        float tick(double freq);
    };

    // extra-specialized form of phasor: implemented custom init, and extra internal state
    class buzz_oscil : public phasor
    {
    public:
        buzz_oscil();
        ~buzz_oscil() {}
        virtual	void init(double srate);	
        float tick(double freq);
    private:
        double srate_;
        double over2n_;
        unsigned long nharms_;
        unsigned long tnp1_;
    };
}


#endif
