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

#include "delay.h"

using namespace audiobook;

delay::delay(const delay& rhs)
{
    buf_ = new float[rhs.length_];
    length_ = rhs.length_;
    input_index_ = rhs.input_index_;
    for(unsigned long i = 0L; i < length_; i++)
        buf_[i] = rhs.buf_[i];
}

delay::~delay()
{
	delete [] buf_;
}

bool delay::init(unsigned long len)
{
	delete [] buf_;
    buf_ = new float[len];
	length_ = len;
    reset();
	return true;
}

bool delay::init(double srate, double dur)
{	
	unsigned long len = static_cast<unsigned long>(srate * dur);
	return init(len);
}

delay& delay::operator=(const delay& rhs)
{
    if(this != &rhs){
        delete [] buf_;
        length_ = rhs.length_;
        buf_ = new float[length_];
        for(unsigned long i = 0L; i < length_; i++)
			buf_[i] = rhs.buf_[i];
    }
    return *this;
} 

void delay::reset()
{
    for(unsigned long i = 0; i < length_; i++)
	    buf_[i] = 0.0f;
	input_index_ = 0L;
}

float delay::tick(float input, double feedback)
{
	if(length_ == 0)   // uninitialized object just echoes the input 
		return input;
	float output = buf_[input_index_];
	buf_[input_index_++] = static_cast<float>(input + feedback * output);
	if(input_index_ == length_)
		input_index_ = 0L;
	return output;
}

