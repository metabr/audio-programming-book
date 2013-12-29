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
/* vdelay.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portsf.h"
#include "fxrand.h"
#include "fxdelay.h"
#include "wave.h"

/* define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_DELAYTIME_MS,ARG_MODFREQ,ARG_MODAMOUNT,ARG_FEEDBACK,ARG_WET,ARG_TAILTIME,ARG_NARGS};
/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

int main(int argc, char* argv[])
{
/* STAGE 1 */
	PSF_PROPS inprops,outprops;									
	long i,framesread;	
	/* init all dynamic resources to default states */
	int infile = -1,outfile = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	/* TODO: define an output frame buffer if channel width different 	*/
	FXDELAY* delay = NULL;
	double feedback = 0.0;
	double vdelaytime = 0.0;
	double dry,wet,delaygain = 1.0;
	double max_delaytime;
	double tailtime = 0.0;
	long tailsamps;
	OSCIL* osc = NULL;
	double modfreq ,modamount;	
	double delay_delta;
	IRAND irand;
	int do_randmod = 0;
	double rdepth = 0.0;
/* STAGE 2 */	
	printf("VDELAY: apply variable delay with feedback to mono soundfile\n");								

	/* process any optional flags: remove this block if none used! */
	if(argc > 1){
		char flag;
		double gain;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			/*TODO: handle any  flag arguments here */
			case('\0'):
				printf("Error: missing flag name\n");
				return 1;
			/* handle -gDGAIN flag */
			case('g'):
				if(argv[1][2]=='\0') {
					printf("Error; -g flag requires parameter\n");
					return 1;
				}
				gain = atof(&(argv[1][2]));
				if(gain <= 0.0){
					printf("Error: gain value must be positive\n");
					return 1;
				}
				delaygain = gain;
				break;
			case('r'):
				if(argv[1][2]=='\0') {
					printf("Error; -r flag requires parameter\n");
					return 1;
				}
				rdepth = atof(&(argv[1][2]));
				if(rdepth <= 0.0 || rdepth > 100.0){
					printf("Error: -rDEPTH:  DEPTH value out of range\n");
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

	/* check rest of commandline */

	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			"usage: VDELAY [-gDGAIN][-rFREQ] infile outfile delay freq mod \n"
			"                feedback wet/dry tailtime\n\n"
			"  -gDGAIN :  scales feedback signal level by DGAIN. Must be > 0.0.\n"
			"             may be needed with high feedback, to keep signal within range.\n"
			"  -rDEPTH : vary modulation depth randomly at rate FREQ Hz.\n"
            "	            0.0 < FREQ <= 100.0\n"
			"  delay   :  sets primary delay time in msecs. Must be > 0.0.\n"
			"               Overall maximum delaytime is extended by delay * mod.\n"
			"  freq    :  modulation frequency (Hz). Must be > 0.0.\n"
			"  mod     :  percent deviation relative to delaytime. 0 <= mod <= 1.0.\n"
			"  feedback:  set feedback level. 0 <= feedback <= 1.0.\n"
			"  wet/dry :  set level of wet (delayed) signal. 0.0 < wet <= 1.0. \n"
			"               The dry (source) level is set to  1.0 - wet.\n"
			"  tailtime:  extra time (secs) added to infile to accommodate decay tail.\n"
			);
		return 1;
	}

	vdelaytime = atof(argv[ARG_DELAYTIME_MS]);
	if(vdelaytime <= 0.0){
		printf("VDELAY: Error: delaytime must be positive!\n");
		return 1;
	}	
	modfreq = atof(argv[ARG_MODFREQ]);
	if(modfreq < 0.0){
		printf("VDELAY: Error: negative value for modfreq\n");
		return 1;
	}
	modamount = atof(argv[ARG_MODAMOUNT]);
	if(modamount < 0.0 || modamount > 1.0){
		printf("VDELAY: Error: modamount out of range 0 -- 1.0 \n");
		return 1;
	}

	/* in this implementation we want to vary delaytime += modamount */
	/* so we calculate max delaytime to embrace vdelaytime +- deviation. */	
	max_delaytime = vdelaytime + (vdelaytime * modamount);
	/* convert times to seconds for FXDELAY */
	max_delaytime *= 0.001;	
	vdelaytime *= 0.001;

	feedback= atof(argv[ARG_FEEDBACK]);
	if(fabs(feedback) > 1.0){
		printf("VDELAY: Error: feedback must be within +- 1.0\n");
		return 1;
	}
	wet =  atof(argv[ARG_WET]);
	if(wet < 0.0 || wet > 1.0){
		printf("VDELAY: wet must be within 0 to 1.0\n");
		return 1;
	}
	dry = 1.0 - wet;
	/* need overall scale by 1/2 */
	dry *=0.5;
	/* also scale delay output by user's gain factor */
	wet *= 0.5 * delaygain;

	tailtime = atof(argv[ARG_TAILTIME]);
	if(tailtime < 0.0){
		printf("VDELAY: negative value for tailtime\n");
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("VDELAY: unable to start portsf\n");
		return 1;
	}
/* STAGE 3 */																							
	infile = psf_sndOpen(argv[ARG_INFILE],&inprops,0);																	  
	if(infile < 0){
		printf("VDELAY: unable to open infile %s\n",argv[ARG_INFILE]);
		error++;
		goto exit;
	}
	/*  verify infile format for this application */
	/* we only support mono signals here */
	if(inprops.chans != 1){
		printf("VDELAY: infile must be mono for now\n");
		error++;
		goto exit;
	}
	/* allocate space for  sample frame buffer ...*/
	inframe = (float*) malloc(nframes * inprops.chans * sizeof(float));
	if(inframe==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	inprops.format = outformat;
	outprops = inprops;

	/* STAGE 4 */
	
	/* TODO: any other argument processing and setup of variables, 		     
	   output buffer, etc., before creating outfile
	*/
	tailsamps = (unsigned long)(tailtime * inprops.srate);				                 
	
	/* create delayline  */
	delay = new_fxdelay();
	if(delay == NULL){
		puts("VDELAY: error creating delay line\n");
		error++;
		goto exit;
	}
	if(fxdelay_init(delay, inprops.srate,max_delaytime)){
		puts("VDELAY: error initializing delay line\n");
		error++;
		goto exit;
	}
	/* create modulation oscillator */
	osc = new_oscil(inprops.srate);
	if(osc == NULL){
		puts("VDELAY: unable to cretae modulation oscillator\n");
		error++;
		goto exit;
	}
	/* create irand osc */
	irand_init(&irand,inprops.srate);

	/* handle outfile */
	/* TODO:  make any changes to outprops here  */
	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}

	/* TODO: if outchans != inchans, allocate outframe, and modify main loop accordingly */
	outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		printf("VDELAY: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
/* STAGE 5 */															   
	printf("processing....\n");									
	/* TODO: init any loop-related variables */
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);

	while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
		/* <--------  add buffer processing here ------>  */
		float input;
		if(do_randmod){
			for(i=0;i < framesread;i++){	
				input =  inframe[i];
				delay_delta = sinetick(osc,modfreq) * modamount * irand_tick(&irand,rdepth);
				delay_delta *= vdelaytime;
				inframe[i] = (float)( dry * input + wet * fxdelay_tick(delay,vdelaytime + delay_delta,feedback,input));			
			}
		}
		else {
			for(i=0;i < framesread;i++){
				input =  inframe[i];
				delay_delta = sinetick(osc,modfreq) * modamount;
				delay_delta *= vdelaytime;
				inframe[i] = (float)( dry * input + wet * fxdelay_tick(delay, vdelaytime + delay_delta, feedback, input));			
			}
		}
		if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
			printf("VDELAY: error writing to outfile\n");
			error++;
			break;
		}			
	}

	if(framesread < 0)	{
		printf("VDELAY: error reading infile. Outfile is incomplete.\n");
		error++;
	}	
	else if(tailsamps){	
		long remain;
		do {
			remain =  tailsamps > NFRAMES ? NFRAMES : tailsamps;
			if(do_randmod){			
				for(i=0; i < remain; i++){
					delay_delta = sinetick(osc,modfreq) * modamount * irand_tick(&irand,rdepth);
					delay_delta *= vdelaytime;
					inframe[i] = (float)( wet * fxdelay_tick(delay,vdelaytime + delay_delta,feedback,0.0));
				}
			}
			else {
				for(i=0; i < remain; i++){
					delay_delta = sinetick(osc, modfreq) * modamount;
					delay_delta *= vdelaytime;
					inframe[i] = (float)( wet * fxdelay_tick(delay,vdelaytime + delay_delta,feedback,0.0));					
				}
			}
			if(psf_sndWriteFloatFrames(outfile,inframe,remain) != remain){
				printf("VDELAY: error writing to outfile\n");
				error++;
				goto exit;
			}
			tailsamps -= remain;
		} while (tailsamps);
	}

	printf("Done: %d errors\n",error);
/* STAGE 6 */		
	/* report PEAK values to user */									
	if(psf_sndReadPeaks(outfile,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < inprops.chans;i++){
				peaktime = (double) peaks[i].pos / (double) inprops.srate;
				printf("CH %ld:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	

/* STAGE 7 */
	/* do all cleanup  */    											
exit:	 	
	if(infile >= 0)
		if(psf_sndClose(infile))
			printf("%s: Warning: error closing infile %s\n",argv[ARG_PROGNAME],argv[ARG_INFILE]);
	if(outfile >= 0)
		if(psf_sndClose(outfile))
			printf("%s: Warning: error closing outfile %s\n",argv[ARG_PROGNAME],argv[ARG_OUTFILE]);
	if(inframe)
		free(inframe);
	if(peaks)
		free(peaks);
	/*TODO: cleanup any other resources */
	if(delay){
		fxdelay_free(delay);
		free(delay);
	}
	if(osc)
		free(osc);
	psf_finish();
	return error;
}
