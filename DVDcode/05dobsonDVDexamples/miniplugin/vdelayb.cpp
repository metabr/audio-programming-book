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

#include "vdelayb.h"
// VEDLAYB using internal buffer object

using namespace audiobook;

// copy constructors: NB may throw memory exception
vdelayb::vdelayb(const vdelayb& rhs) 
{
	buf_ =  rhs.buf_;
	length_ = buf_.size();
	input_index_ = rhs.input_index_;
	sr_ = rhs.sr_;
}

vdelayb& vdelayb::operator=(const vdelayb& rhs)
{
	if(this != &rhs){
		buf_ = rhs.buf_;
		length_ = rhs.length_;
		input_index_ = rhs.input_index_;
		sr_ = rhs.sr_;		
	}
	return *this;
}

// catches any memory exception internally, returns false
// modified code in this method, reset() and in destructor
// (which now does nothing)  but no other changes needed!
bool vdelayb::init(double srate, double dur)
{
    unsigned long len = static_cast<unsigned long>
                         (srate * dur + 0.5);
    try{
        buf_.init(len);
    }
    catch(...){
        return false;
    }
	length_ = buf_.size();
	sr_ = srate;
	reset();
	return true;
}


void vdelayb::reset()
{
	buf_.clear();
	input_index_ = 0;
}

float vdelayb::tick(float input,double vdelay_secs,double feedback)
{
    if(length_ == 0)
        return input;
	double dlength = static_cast<double>(length_);			
	// read pointer is vlength ~behind~ write pointer 
	double vlength = dlength -  (vdelay_secs  * sr_);
    // clip vdelayb to max del length
	vlength = vlength < dlength ? vlength : dlength;	   
	
	double readpos = input_index_ + vlength;
	unsigned long base_readpos = 
                     static_cast<unsigned long>(readpos);				  
	if(base_readpos >= length_)		 
		base_readpos -= length_;

	unsigned long next_index = base_readpos + 1;
	if(next_index >= length_)
		next_index -= length_;
	// basic interp of variable delay pos
	double frac = readpos - static_cast<int>(readpos);	
	double output = buf_[base_readpos]+((buf_[next_index] 
                            - buf_[base_readpos]) * frac);	
	// add in new sample + fraction of ouput, unscaled, 
    // for minimum decay at max feedback
	buf_[input_index_++] = static_cast<float>(input  
                        + (feedback * output));
	if(input_index_ == length_)
		input_index_ = 0;
	return static_cast<float>(output);
}
