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
#include "BreakStream.h"


BreakStream::BreakStream() : srate_(0.0), curpos_(0.0),incr_(0.0),
						width_(0.0),height_(0.0),ileft_(0),iright_(0),
						more_points_(0),errstr_("")
{
	// NB: C++ calls the default constructor for the points_ vector automatically	
}


BreakStream::~BreakStream()
{
	// don't need to do this:
	// C++ will automatically call the vector and file destructor
	points_.clear();   // empty the vector
	brkfile_.close();
}
/* copy constructor:  NB does not copy brkfile_ : should be blank  */

BreakStream::BreakStream(const BreakStream& stream) 
{
	srate_	= stream.srate_;
	curpos_ = stream.curpos_;
	incr_	= stream.incr_;
	width_	= stream.width_;
	height_ = stream.height_;
	ileft_	= stream.ileft_;
	iright_ = stream.iright_;
	more_points_ = stream.more_points_;
	points_.clear();
	points_ = stream.points_;
}

void BreakStream::reset()
{
	points_.clear();
	if(brkfile_.is_open()){
		brkfile_.close();
		brkfile_.clear();
	}
	srate_ = 0.0;
	incr_= 0.0;
	curpos_= 0.0;
	width_= 0.0;
	height_= 0.0;
	ileft_= 0;
	iright_= 0;
	more_points_ = 0;
	errstr_ = "";
}

long BreakStream::loadFromFile(const char* filename) {
	reset();
	brkfile_.open(filename);
	long nPoints(-1);
	if(brkfile_.is_open())	{
		nPoints = readPoints();
		if(nPoints >=0){
			brkfile_.close();
			brkfile_.clear();
		}
	}
	return nPoints;
}

long BreakStream::readPoints()
{
	bool goodpoints = true;
	double Time,Value;
	double lastTime = 0.0;
	
	if(!brkfile_.is_open())	 {
		errstr_ = "no input file.\n";
		return -1;
	}
	while(brkfile_ >> Time){
		if(Time < lastTime){
			goodpoints = false;
			errstr_ =  "error in breakpoint data: times not increasing\n";
			break;
		}
		lastTime = Time;		
		if(!(brkfile_ >> Value)){
			goodpoints = false;
			errstr_ =  "error in breakpoint data: incomplete point\n";
			break;	
		}
		Breakpoint point(Time,Value);
		points_.push_back(point);
	}	
	if(!goodpoints){
		brkfile_.close();
		brkfile_.clear();
		return -1;
	}
	//NB: must call init with valid srate before first use			
	rewind();
	return static_cast<long>(points_.size());
}


bool BreakStream::init(double srate) { 
	if(points_.size()==0 || srate <= 0.0) 
		return false; 
	srate_ = srate; 
	incr_ = 1.0 / srate;
	rewind(); 
	return true;
}

void BreakStream::rewind(void)
{
	if(points_.size()>0){
		ileft_ = 0;
		iright_ = 1;
		left_	= points_[ileft_];
		right_	= points_[iright_];
		width_	= right_.getTime() - left_.getTime(); 
		height_	= right_.getValue() - left_.getValue();
		curpos_	= 0.0;
		more_points_ = 1;
	}
}

double BreakStream::tick(void)
{
	double thisval,frac;

	/* beyond end of brkdata? */
	if(more_points_ == 0)
		return right_.getValue();
	if(width_ == 0.0) 
		thisval = right_.getValue();
	else {
		/* get value from this span using linear interpolation */
		frac = (curpos_ - left_.getTime()) / width_;
		thisval = left_.getValue() + ( height_ * frac);
	}
	/* move up ready for next sample */
	curpos_ += incr_;
	if(curpos_ > right_.getTime()){  /* need to go to next span? */
		ileft_++; iright_++;
		if(iright_ < points_.size()) {
			left_ = points_[ileft_];
			right_ = points_[iright_];
			width_	= right_.getTime() - left_.getTime(); 
			height_	= right_.getValue() - left_.getValue();	
		}
		else
			more_points_ = 0;
	}
	return thisval;
}

bool BreakStream::inRange(double minval,double maxval)	const
{
    bool range_OK = true;
#ifdef USE_INDEX_TO_ITERATE
    // "better C" version
	for(unsigned long i=0; i < points_.size(); i++){	    
		if(points_[i].getValue() < minval || points_[i].getValue() > maxval)   {
			range_OK = false;		
            break;
		}
   }
#else
   // use STL iterator ; has to be const_iterator as function is declared const
    std::vector<Breakpoint>::const_iterator it;
    for(it = points_.begin(); it != points_.end(); it++){
        if((*it).getValue() < minval || (*it).getValue() > maxval) {
            range_OK = false;
            break;
        }
    }
#endif
   return range_OK;
}

#ifdef _MSC_VER
# if _MSC_VER <= 1200
namespace std {
	template <class T> const T& min(const T& x, const T&y) { return x < y ? x : y;}
	template <class T> const T& max(const T& x, const T&y) { return x > y ? x : y;}
}
# endif
#endif

void BreakStream::getMinMax(double *outmin,double *outmax)	const
{
	double val,minval,maxval;
	unsigned long i;
	
	if(size() < 2)
		return;
												  
	minval = maxval = points_[0].getValue();
	for(i=1;i < size();i++){
		val = points_[i].getValue();
		//minval = val < minval ? val : minval;
		minval = std::min(val,minval);
		//maxval = maxval > val ? maxval : val;
		maxval = std::max(val,maxval);
	}
	*outmin = minval;
	*outmax = maxval;
}