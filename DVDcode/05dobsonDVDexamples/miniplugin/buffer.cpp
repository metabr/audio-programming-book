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