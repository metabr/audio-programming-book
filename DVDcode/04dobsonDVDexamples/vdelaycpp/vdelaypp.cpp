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

/* vdelay.cpp :  generic soundfile processing main function*/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <portsf.h>
#include "vdelay.h"
// NB this prog uses the vdelayb object
//To use derived sine_oscil:
#include "waveoscil.h"
// or for simple sine_oscil:
//#include "oscil.h"
// a simple class entirely defined in a header file
#include "irandosc.h"

/* define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_DELAYTIME_MS,ARG_MODFREQ,
		ARG_MODAMOUNT,ARG_FEEDBACK,ARG_WET,ARG_TAILTIME,ARG_NARGS};
/* set size of multi-channel frame-buffer */
static const long nFrames(1024);


int main(int argc, char* argv[])
{

// STAGE 1 	: Title																
	std::cout << "VDELAYPP: apply variable delay with feedback to mono soundfile\n";
	if(psf_init()){
		std::cout << "unable to start portsf\n";
		return 1;
	}
// STAGE 2: process command line
	// process any optional flags: remove this block if none used!	
	double delaygain(1.0);
	double rdepth(0.0);
	bool do_randmod(false);
	if(argc > 1){
		char flag;
		double gain;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			/*TODO: handle any  flag arguments here */
			case('\0'):
				std::cout << "Error: missing flag name\n";
				return 1;
			/* handle -gDGAIN flag */
			case('g'):
				if(argv[1][2]=='\0') {
					std::cout << "Error; -g flag requires parameter\n";
					return 1;
				}
				gain = atof(&(argv[1][2]));
				if(gain <= 0.0){
					std::cout << "Error: gain value must be positive\n";
					return 1;
				}
				delaygain = gain;
				break;
			case('r'):
				if(argv[1][2]=='\0') {
					std::cout << "Error; -r flag requires parameter\n";
					return 1;
				}
				rdepth = atof(&(argv[1][2]));
				if(rdepth <= 0.0 || rdepth > 100.0){
					std::cout << "Error: -rFREQ:  FREQ value out of range\n";
					return 1;
				}
				do_randmod = 1;
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
		std::cout << "insufficient arguments."
			<< std::endl
			<< "usage: VDELAYPP [-gDGAIN][-rFREQ] infile outfile delay freq mod" << std::endl		
			<< "                feedback wet/dry tailtime\n\n"
			<< "  -gDGAIN :  scales feedback signal level by DGAIN. Must be > 0.0." << std::endl
			<< "             may be needed with high feedback, to keep signal within range." << std::endl
			<< "  -rFREQ :  vary modulation depth randomly at rate FREQ Hz." << std::endl
            << "	            0.0 < FREQ <= 100.0" << std::endl
			<< "  delay   :  sets primary delay time in msecs. Must be > 0.0." << std::endl
			<< "                 Overall maximum delaytime is extended by delay * mod." << std::endl
			<< "  freq    :  modulation frequency (Hz). 0.0 < freq < Nyquist." << std::endl
			<< "  mod     :  percent deviation relative to delaytime. 0 <= mod <= 1.0." << std::endl
			<< "  feedback:  set feedback level. 0 <= feedback <= 1.0." << std::endl
			<< "  wet/dry :  set level of wet (delayed) signal. 0.0 < wet <= 1.0. " << std::endl
			<< "              The dry (source) level is set to  1.0 - wet." << std::endl
			<< "  tailtime:  extra time (secs) added to infile to accommodate decay tail." << std::endl
			<< std::endl;
		return 1;
	}
	// process arguments, prepare outfile properties, load any breakpoint files
	double vdelaytime = atof(argv[ARG_DELAYTIME_MS]);
	if(vdelaytime <= 0.0){
		std::cout << "VDELAYPP: Error: delaytime must be positive!" << std::endl;
		return 1;
	}
	
	double modfreq = atof(argv[ARG_MODFREQ]);
	if(modfreq < 0.0){
		std::cout << "VDELAYPP: Error: negative value for modfreq" << std::endl;
		return 1;
	}
	double modamount = atof(argv[ARG_MODAMOUNT]);
	if(modamount < 0.0 || modamount > 1.0){
		std::cout << "VDELAYPP: Error: modamount out of range 0 -- 1.0 " << std::endl;
		return 1;
	}

	// in this implementation we want to vary delaytime += modamount 
	// so we calculate max delaytime to embrace vdelaytime +- deviation. 
	double max_delaytime = vdelaytime + (vdelaytime * modamount);
	// convert times to seconds  
	max_delaytime *= 0.001;	
	vdelaytime *= 0.001;

	double feedback = atof(argv[ARG_FEEDBACK]);
	if(fabs(feedback) > 1.0){
		std::cout << "VDELAYPP: error: feedback must be within +- 1.0" << std::endl;
		return 1;
	}
	double wet = atof(argv[ARG_WET]);
	if(wet < 0.0 || wet > 1.0){
		std::cout << "VDELAYPP: error: wet must be within 0 to 1.0" << std::endl;
		return 1;
	}
	double dry = 1.0 - wet;
	/* need overall scale by 1/2 */
	dry *=0.5;
	/* also scale delay output by user's gain factor */
	wet *= 0.5 * delaygain;

	double tailtime = atof(argv[ARG_TAILTIME]);
	if(tailtime < 0.0){
		std::cout << "VDELAYPP: error: negative value for tailtime" << std::endl;
		return 1;
	}
// STAGE 3	open infile if appropriate	
	PSF_PROPS inprops;
	int infile = psf_sndOpen(argv[ARG_INFILE],&inprops,0);																	  
	if(infile < 0){
		std::cout << "VDELAYPP: error: unable to open infile " << argv[ARG_INFILE] << std::endl;
		return 1;
	}
	
	//  verify infile format for this application 
	// we only support mono signals here 
	if(inprops.chans != 1){
		std::cout << "VDELAYPP: infile must be mono for now" << std::endl;
		psf_sndClose(infile);
		return 1;
	}
	if(modfreq >= inprops.srate / 2){
		std::cout << "VDELAYPP: error: freq value too high: must be < " << inprops.srate << std::endl;
		psf_sndClose(infile);
		return 1;
	}
	// load any breakpoint data 

// STAGE 4: open outfile
	// check file extension of outfile name
	psf_format outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if(outformat == PSF_FMT_UNKNOWN){
		std::cout << "VDELAYPP errrr: outfile name "
		<< argv[ARG_OUTFILE]
		<< " has unknown format."
		<< std::endl
		<<	"Use any of .wav, .aiff, .aif, .afc, .aifc "
		<< std::endl;
		psf_sndClose(infile);
		return 1;
	}
	PSF_PROPS outprops(inprops);
	outprops.format = outformat;
	int outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		std::cout << "VDELAYPP: error: unable to create outfile "
		<< argv[ARG_OUTFILE]
		<< std::endl;			
		psf_sndClose(infile);
		return 1;
	}  

// STAGE 5: declare any other top-level vars, before entering try block						                 
	bool error(false);	
	PSF_CHPEAK* peaks = 0;
	clock_t starttime, endtime;
	float* inframe = 0;
	long tailsamps = static_cast<long> (tailtime * inprops.srate);
	// any other dynamic resources used in try block
    // a using declaration
    using audiobook::vdelayb;
    using audiobook::sine_oscil;
	vdelayb myvdelay;
	// use either wave_oscil.h OR oscil.h
    sine_oscil osc;
// STAGE 6: first try block	 : dynamic allocations
	try{
		// first: allocate all dynamic objects
		// allocate space for  sample frame buffer and PEAK data ...
		inframe = new float[nFrames * inprops.chans];
		peaks  =  new PSF_CHPEAK[outprops.chans];
		// TODO: define an output frame buffer if channel width different; modify processing loop accordingly				
	}
	catch(const std::bad_alloc&){  // if we don't use it, no need to give arg a name
		// can't recover from memory problem, but can try a puts call
		puts("No memory!\n");
		return 1;
	}
	if(!myvdelay.init(inprops.srate, max_delaytime)){
		puts("No memory for delay line\n");
		return 1;
	}
	osc.init(inprops.srate);
    irand_osc randosc;
    randosc.init(inprops.srate);
        
	double tickval, delay_delta;
	long framesread;
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);
// STAGE 7: second try block: processing loop
	std::cout << "processing...." << std::endl;											
	starttime = clock();
	try{
		while ((framesread = psf_sndReadFloatFrames(infile,inframe,nFrames)) > 0){
			/* <--------  add buffer processing here ------>  */
            float input;
            if(do_randmod){
                for(long i=0; i < framesread; i++){					
                    input =  inframe[i];
                    delay_delta = osc.tick(modfreq) * modamount * randosc.tick(rdepth);
                    delay_delta *= vdelaytime;
                    tickval =  myvdelay.tick(input,vdelaytime + delay_delta,feedback);					
                    inframe[i] = (float)(dry * input +  wet *tickval);								
                }
            }
            else{
                for(long i=0; i < framesread; i++){					
                    input =  inframe[i];
                    delay_delta = osc.tick(modfreq) * modamount ;
                    delay_delta *= vdelaytime;
                    tickval =  myvdelay.tick(input,vdelaytime + delay_delta,feedback);					
                    inframe[i] = (float)(dry * input +  wet *tickval);								
                }
                
            }
			if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
				throw std::runtime_error("Error writing to outfile\n");
			}			
			
		}
		if(framesread < 0)	{
			printf("VDELAYPP: error reading infile. Outfile is incomplete.\n");
			throw std::runtime_error("Error reading infile. Outfile is incomplete.\n");
		}	
		else if(tailsamps){	
			do {
				long remain =  tailsamps > nFrames ? nFrames : tailsamps;
                if(do_randmod){
                    for(long i = 0; i < remain; i++){
                        delay_delta = osc.tick(modfreq) * modamount * randosc.tick(rdepth);
                        delay_delta *= vdelaytime;
                        inframe[i] = (float)( wet * myvdelay.tick(0.0,vdelaytime + delay_delta,feedback));					
                    }
                }
                else{
                    for(long i = 0; i < remain; i++){
                        delay_delta = osc.tick(modfreq) * modamount;
                        delay_delta *= vdelaytime;
                        inframe[i] = (float)( wet * myvdelay.tick(0.0,vdelaytime + delay_delta,feedback));					
                    }
                }
				if(psf_sndWriteFloatFrames(outfile,inframe,remain) != remain){
					throw std::runtime_error("VDELAYPP: error writing to outfile\n");
				}
				tailsamps -= remain;
			} while (tailsamps);
		}
	}
//STAGE 8: catch block, report to user. 	
	catch(const std::runtime_error& e){
		error = true;
		std::cout << "VDELAYPP: error: Unable to perform task:" << std::endl;
		std::cout << e.what();			
	}			
	if(!error){
		std::cout << std::endl << "Done" << std::endl;
		endtime = clock();
		std::cout << "Processing time = "
			<< ((endtime-starttime)/(double)CLOCKS_PER_SEC)
			<< " secs" 
			<< std::endl;
		// report PEAK values to user 									
		if(psf_sndReadPeaks(outfile,peaks,NULL) > 0){
			std::cout << "PEAK information:\n";	
				for(long i=0; i < outprops.chans; i++){
					double peaktime = (double) peaks[i].pos / (double) outprops.srate;
					std::cout << "CH " 
						<< i+1 << ":\t" 
						<< peaks[i].val 
						<< " at " << peaktime <<" secs" 
						<< std::endl;
				}
		}		
	}	
//STAGE 9: finish up:	    												 	
	
	if(psf_sndClose(infile))
		std::cout << argv[ARG_PROGNAME] 
		<< "Warning: error closing infile "
		<< argv[ARG_INFILE]
		<< std::endl;
	
	if(psf_sndClose(outfile))
		std::cout << argv[ARG_PROGNAME] 
		<< "Warning: error closing outfile "
		<< argv[ARG_OUTFILE]
		<< std::endl;
	// delete is safe to call on a null pointer, no need to test
	delete [] inframe;
	delete [] peaks;
	// cleanup any other resources 
	psf_finish();
	return error ? 1 : 0;
}
