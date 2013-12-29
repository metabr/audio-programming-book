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
/* siggen.cpp  (from main.c) :  generic soundfile processing main function*/
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <portsf.h>
#include "BreakStream.h"
#include "waveoscil.h"


/* set size of multi-channel frame-buffer */
/* TODO: use unnamed namespace */
static const long nFrames = 1024;

/*  define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_OUTFILE,ARG_TYPE,ARG_DUR,ARG_SRATE,ARG_NCHANS, ARG_AMP,ARG_FREQ,ARG_NARGS};

enum { WAVE_SINE,WAVE_TRIANGLE,WAVE_SQUARE,WAVE_SAWUP,WAVE_SAWDOWN,WAVE_BUZZ,WAVE_NTYPES};

// our custom exception object, no reference here to std exception classes
class sfile_exception
{
public:
	 sfile_exception(const std::string& errmsg) : msg(errmsg) {}
	const char* message() const throw()  {return msg.c_str();} 
private:
    const std::string msg; 
};

int main(int argc, char* argv[])
{
	// STAGE 1 : title, any top-level vars
	std::cout << std::endl <<"SIGGENPP: generate simple waveforms: C++ version" 
		<< std::endl << std::endl;
	
	psf_stype psftype = PSF_SAMP_16;
	// check portsf is happy
	if(psf_init()){
		std::cout << "unable to start portsf\n";
		return 1;
	}
// STAGE 2: process command line
	// process any optional flags: remove this block if none used! 
	if(argc > 1){
		int samptype = 0;
		char flag;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			// TODO: handle any  flag arguments here 
			case('\0'):
				std::cout << "Error: missing flag name\n";
				return 1;
			case('s'):
				if(argv[1][2] =='\0'){
					std::cout << "Error: -s flag requires parameter\n";
					return 1;
				}
				samptype = atoi(&(argv[1][2]));
				switch(samptype){
				case(0):
					psftype = PSF_SAMP_16;
					break;
				case(1):
					psftype = PSF_SAMP_24;
					break;
				case(2):
					psftype = PSF_SAMP_32;
					break;
				case(3):
					psftype = PSF_SAMP_IEEE_FLOAT;
					break;
				default:
					std::cout << "Error: -s value out of range\n";
					return 1;
				}
				break;
			default:
				break;
			}
			argc--;
			argv++;
		}
	}

	// process rest of commandline 
	if(argc < ARG_NARGS){
		std::cout << "insufficient arguments.\n"
			// TODO: add required usage message 
			<< "usage: siggenpp [-sN] outfile wavetype dur srate nchans amp freq\n"
			<< "where wavetype =:\n"
			<< "                0: sine\n"
			<< "                1: triangle\n"
			<< "                2: square\n"
			<< "                3: sawtooth up\n"
			<< "                4: sawtooth down\n"
			<< "                5: band-limited pulse\n"
			<< "          -sN:     set sample type to one of:\n"				   
			<< "                0: 16-bit (default)\n"
			<< "                1: 24-bit\n"
			<< "                2: 32-bit integer\n"
			<< "                3: 32-bit float\n"
			;
		return 1;
	}
	// process arguments, prepare outfile properties, load any breakpoint files
	
	int wavetype = atoi(argv[ARG_TYPE]);
	if(wavetype < WAVE_SINE || wavetype > WAVE_NTYPES){
		std::cout << "Error: bad value for wave type\n";
		return 1;
	}

// STAGE 3	open infile if appropriate

	// define outfile format 
	PSF_PROPS outprops;	
	outprops.srate = atoi(argv[ARG_SRATE]);
	if(outprops.srate <=0){
		std::cout << "error: srate must be positive\n";
		return 1;
	}
	outprops.chans = atoi(argv[ARG_NCHANS]);
	if(outprops.chans <=0){
		std::cout << "error: nchans must be positive\n";
		return 1;
	}
	outprops.samptype = psftype;
	outprops.chformat = STDWAVE;
	// check file extension of outfile name, so we use correct output file format
	psf_format outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if(outformat == PSF_FMT_UNKNOWN){
		std::cout << "outfile name "
		<< argv[ARG_OUTFILE]
		<< " has unknown format."
		<< std::endl
		<<	"Use any of .wav, .aiff, .aif, .afc, .aifc "
		<< std::endl; 
		return 1;
	}
	outprops.format = outformat;

	double dur =  atof(argv[ARG_DUR]);
	if(dur <= 0.0){
		std::cout << "Error: dur must be positive\n";
		return 1;
	}
	double amp =  atof(argv[ARG_AMP]);
	if(amp <= 0.0){
		std::cout << "Error: amp must be positive\n";
		return 1;
	}
	// load any breakpoint data 
	double freq(0.0); // init ensures no gcc compiler warning!
	bool frqstream(false);
	
	BreakStream FreqStream;
	long nPoints = FreqStream.loadFromFile(argv[ARG_FREQ]);
	if(nPoints >=0){		
		std::cout << "read " <<nPoints << "points\n";
		if(nPoints < 2){
			std::cout << "Error in Freq file: need at least 2 points\n";
			return 1;
		}
		FreqStream.init(outprops.srate);
		double minval,maxval;
		FreqStream.getMinMax(&minval,&maxval);
		if(minval <= 0.0 || maxval <= 0.0){
			std::cout << "Error: bad frequency values in breakpoint file " 
				<< argv[ARG_FREQ] 
				<< std::endl;			
			return 1;
		}
		// check for no aliasing
		double Nyquist = outprops.srate/2.0;
		if(minval >= Nyquist  || maxval >= Nyquist){
			std::cout << "Error: frequency values above " 
				<< outprops.srate/2 
				<< "in breakpoint file " 
				<< argv[ARG_FREQ] 
				<< std::endl;			
			return 1;
		}
		frqstream = true;
	}
	else {
		// ... or read the arg as a constant
		char* strend;
        // TODO: convert this to use a C++ idiom
		freq = strtod(argv[ARG_FREQ],&strend);
		if(*strend== '\0'){ // valid number
			if(freq <= 0.0)	{		
				std::cout << "Error: freq must be positive\n";
				return 1;
			}
		}
		else {
			std::cout << "Error: unable to open breakpoint file " << argv[ARG_FREQ] << std::endl;
			return 1;
		}
	}
// STAGE 4: open outfile	
	int outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		std::cout << "Error: unable to create outfile "
		<< argv[ARG_OUTFILE]
		<< std::endl;		
		return 1;
	}

// STAGE 5: declare any other top-level vars, before entering try block	
	PSF_CHPEAK* peaks = 0;
	float* outframe = 0;	
	clock_t starttime, endtime;
	// any other dynamic resources used in try block
	// this will be used as base-class pointer to derived class
    using namespace audiobook;
	phasor* osc = 0;

//STAGE 6:  first try block  : dynamic allocations	
	try{		
		// first: allocate all dynamic objects
		// allocate space for  sample frame buffer and PEAK data ...
		outframe = new float[nFrames * outprops.chans];
		peaks  =  new PSF_CHPEAK[outprops.chans];
		// create any other objects required for processing loop
		switch(wavetype){
		case(WAVE_SINE):
			osc = new sine_oscil;
			break;
		case(WAVE_TRIANGLE):
			osc = new triangle_oscil;
			break;
		case(WAVE_SQUARE):
			osc = new square_oscil;
			break;
		case(WAVE_SAWUP):
			osc = new sawup_oscil;
			break;
		case(WAVE_SAWDOWN):
			osc = new sawdown_oscil;
            break;
		case(WAVE_BUZZ):
			osc = new buzz_oscil;
			break;		
		}
		osc->init(outprops.srate);				
	}
	catch(const std::bad_alloc&){
		// can't recover from memory problem, but can try a puts call
		puts("No memory!\n");
		return 1;
	}
	// prepare variables for processing loop
	// calculate buffer sizes
	unsigned long outframes = static_cast<unsigned long>(dur * outprops.srate + 0.5);
	unsigned long nbufs = outframes / nFrames;	
	long remainder = outframes;
	if(nbufs >0)
		remainder -= nbufs*nFrames;
// STAGE 7: second try block: processing loop 
	std::cout << "processing..." << std::endl;
	bool error(false);
	starttime = clock();
	try {
		for(unsigned long i=0; i < nbufs; i++){
			for(long j=0; j < nFrames; j++){
				if(frqstream)
					freq = FreqStream.tick();
				float val = static_cast<float>(amp * osc->tick(freq));
				for(long k=0; k < outprops.chans; k++)
					outframe[j*outprops.chans + k] = val;
			}
			if(psf_sndWriteFloatFrames(outfile,outframe,nFrames) != nFrames){
				throw sfile_exception(std::string("Error writing to outfile\n"));				
			}
		}		
					
		for(long j=0;j < remainder; j++){
			if(frqstream)
				freq = FreqStream.tick();
			float val = static_cast<float>(amp * osc->tick(freq));
			for(long k=0;k < outprops.chans;k++)
				outframe[j*outprops.chans + k] = val;
		}
		if(psf_sndWriteFloatFrames(outfile,outframe,remainder) != remainder){
			throw sfile_exception("Error writing to outfile\n");				
		}
	}
//STAGE 8: catch block, report to user
	catch(const sfile_exception& e){
		error = true;
		std::cout << e.message();
	}		
	if(!error){
		std::cout << std::endl << "Done" << std::endl;
		endtime = clock();
		std::cout << "Processing time = "
			<< ((endtime-starttime)/static_cast<double>(CLOCKS_PER_SEC))
			<< " secs" 
			<< std::endl;

		// report PEAK values to user									
		if(psf_sndReadPeaks(outfile,peaks,NULL) > 0){
			std::cout << "PEAK information:\n";	
				for(long i=0; i < outprops.chans; i++){
					double peaktime = static_cast<double>(peaks[i].pos) / static_cast<double>(outprops.srate);
					std::cout << "CH " 
						<< i+1 << ":\t" 
						<< peaks[i].val 
						<< " at " << peaktime <<"secs" 
						<< std::endl;
				}
		}		
	}
//STAGE 7: finish up: 	   														
	if(outfile >= 0)
		if(psf_sndClose(outfile))
			std::cout << argv[ARG_PROGNAME] 
			<< "Warning: error closing outfile "
			<< argv[ARG_OUTFILE]
			<< std::endl;
	// delete is safe to call on a null pointer, no need to test
	delete [] outframe;
	delete [] peaks;
	// cleanup any other resources	
	delete osc;
	psf_finish();
	// NB: Freqstream on the stack destroys itself automatically via its destructor: nothing to do here!
	return error ? 1 : 0;
}
