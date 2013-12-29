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

//BreakStream.h
//use trailing underscores for member variables
#ifndef _BREAKSTREAM_H
#define _BREAKSTREAM_H
#include <fstream>
#include <vector>

// OTT as a class? or just use a struct! 
class Breakpoint
{
public:
	Breakpoint() : time_(0.0), value_(0.0) {};
	virtual ~Breakpoint()  {}
	Breakpoint(double time, double value) : time_(time), value_(value) {}
	double getTime(void)  const { return time_;}
	double getValue(void) const { return value_;}
private:
	double time_;
	double value_;
};




class BreakStream
{
public:
	BreakStream();	
	BreakStream(const char* filename);
	BreakStream(const BreakStream& stream);
	virtual ~BreakStream();
	long loadFromFile(const char* filename);
	bool init(double srate);	
	void rewind(void);
	double tick(void);
	unsigned int size(void)							const {return points_.size(); }
	bool inRange(double minval,double maxval)		const;
	void getMinMax(double *outmin,double *outmax)	const;
	const char* lastError()							const { return errstr_; }
protected:
	void reset();
	long readPoints();
private:
	Breakpoint		left_,right_;
	std::vector<Breakpoint> points_;
	std::ifstream brkfile_;
	double	srate_;
	double	curpos_;
	double	incr_;
	double	width_;
	double	height_;
	unsigned long   ileft_,iright_;
	int		more_points_;
	const char* errstr_;
};
#endif
