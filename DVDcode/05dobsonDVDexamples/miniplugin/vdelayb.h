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

#ifndef _VDELAYB_H_
#define _VDELAYB_H_

// simple wrapper class for an array
namespace audiobook {
    class buffer {
    public:
        buffer() : buf_(0), length_(0)                   {}
        buffer(const buffer&);
        virtual ~buffer()                                { delete [] buf_; };
        void init(unsigned long size);
        buffer& operator=(const buffer& rhs);
        unsigned long size()                       const { return length_;}
        // return reference: support array read/write
        float& operator[](unsigned long pos)             {return buf_[pos];} 
        const float& operator[](unsigned long pos) const {return buf_[pos];}
        void clear();
    private:
        float* buf_;
        unsigned long length_;
};

class vdelayb 
{
public:
	vdelayb() : length_(0), input_index_(0),sr_(0.0) {}
	vdelayb(const vdelayb&);
	vdelayb& operator=(const vdelayb& rhs);
	bool init(double srate, double maxdur);
	virtual ~vdelayb() {}	// now, nothing to do, buf_ cleans up after itself
	unsigned long size() const {return length_;}
	void reset();
	float tick(float input, double vdelay_secs, double feedback);
private:
	buffer buf_;
	unsigned long length_;
	unsigned long input_index_;
	double	sr_;
};

}  // namespace audiobook

#endif
