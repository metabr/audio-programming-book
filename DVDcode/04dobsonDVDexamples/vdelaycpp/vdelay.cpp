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
// delay.cpp
// based on Ch 2 section 7

#include "vdelay.h"

using namespace audiobook;

//all these three may throw bad_alloc exception
void buffer::init(unsigned long size)
{
	delete [] buf_;
	buf_ = new float[size];
	length_ = size;
}

buffer::buffer(const buffer& rhs)
{
	buf_ = new float[rhs.length_];
	for(unsigned long i = 0; i < rhs.length_; i++)
		buf_[i] = rhs.buf_[i];
	length_ = rhs.length_;
}

buffer& buffer::operator=(const buffer& rhs)
{
	if(this != &rhs){
		delete [] buf_;
		buf_ = new float[rhs.length_];
		length_ = rhs.length_;
		for(unsigned long i = 0; i < length_; i++)
			buf_[i] = rhs.buf_[i];
	}
	return *this;
}

/* note this could equally be handled externally via operator[] */
void buffer::clear()
{	
	for(unsigned long i = 0; i < length_; i++)
		buf_[i] = 0.0;
}

// VDELAY using raw floats buffer

// copy constructors: NB may throw memory exception
vdelay::vdelay(const vdelay& rhs) : buf_(0), length_(0), input_index_(0)
{
	length_      = rhs.length_;
	input_index_ = rhs.input_index_;
	sr_          = rhs.sr_;
	if(length_){
		buf_ = new float[length_];
		for(unsigned long i = 0; i < length_; i++)
			buf_[i] = rhs.buf_[i];
	}
}

vdelay& vdelay::operator=(const vdelay& rhs)
{
	if(this != &rhs){
		delete [] buf_;
		buf_ = new float[rhs.length_];
		length_ = rhs.length_;
		input_index_ = rhs.input_index_;
		sr_ = rhs.sr_;
		for(unsigned long i = 0; i < length_; i++)
			buf_[i] = rhs.buf_[i];
	}
	return *this;
}

bool vdelay::init(double srate, double dur)
{
	unsigned long len = static_cast<unsigned long>(srate * dur);
	if(len){  // allow zero size!
        if(buf_){
            delete [] buf_;
            length_ = 0;
        }
	
        try{
            buf_ = new float[len];
        }
        catch(...){
            return false;
        }
    }
    length_ = len;
	sr_ = srate;
	return true;
}

vdelay::~vdelay()
{
	delete [] buf_;
}
// TODO make two funcs: clear() and reset()
void vdelay::reset()
{
	for(unsigned long i = 0; i < length_; i++)
		buf_[i] = 0.0;
	input_index_ = 0;
}

float vdelay::tick(float input, double vdelay_secs, double feedback)
{	
	if(length_ == 0)   // uninitialized object has zero length:  no-op tick function just echoes the input
		return input;
	double dlength = static_cast<double>(length_);					// get maxlen, save a cast later on 
	// read pointer is vlength ~behind~ write pointer 
	double vlength = dlength -  (vdelay_secs  * sr_);
	vlength = vlength < dlength ? vlength : dlength;	  // clip vdelay to max del length 
	
	double readpos = input_index_ + vlength;
	unsigned long base_readpos = static_cast<unsigned long>( readpos);				  
	if(base_readpos >= length_)					  // wrap dl indices 
		base_readpos -= length_;

	unsigned long next_index = base_readpos + 1;
	if(next_index >= length_)
		next_index -= length_;
	// basic interp of variable delay pos
	double frac = readpos - static_cast<int>(readpos);	
	double output = buf_[base_readpos]+((buf_[next_index] - buf_[base_readpos]) * frac);	
	// add in new sample + fraction of ouput, unscaled, for minimum decay at max feedback
	buf_[input_index_++] = static_cast<float>(input   + (feedback * output));
	if(input_index_ == length_)
		input_index_ = 0;
	return static_cast<float>(output);
}
