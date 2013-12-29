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

/* tdelay.c:  example implementation of tapped delay line using FXDELAY (DVD Chapter 1) */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portsf.h"
#include "fxdelay.h"


/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_TAPDATA,ARG_FEEDBACK,ARG_WET,ARG_TAILTIME,ARG_NARGS};

int main(int argc, char* argv[])
{
	PSF_CHPEAK* peaks = NULL;                                            /* STAGE 1 */
    FILE* tapfile = NULL;
	DELAYTAP* taps = NULL;
    float* inframe = NULL;
	/* TODO: define an output frame buffer if channel width different 	*/
	FXDELAY* delay = NULL;
    PSF_PROPS inprops,outprops;									          
	psf_format outformat;
	unsigned long nframes = NFRAMES;
	double feedback = 0.0;
	double dry,wet,delaygain = 1.0;
	double tailtime = 0.0;
	long tailsamps;
    long framesread;	
	int ntaps, infile = -1,outfile = -1;
	int error = 0;
	
	printf("TDELAY: apply multi-tap delay to soundfile\n");		          /* STAGE 2 */

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
			/* TODO: add required usage message */
			"usage: TDELAY [-gDGAIN] infile outfile tapdata.txt feedback wet/dry tailtime \n"
			"      -gDGAIN:	      scales feedback signal level by DGAIN. Must be > 0.0.\n"
			"       tapdata.txt:  text file giving delay taps as time/amp pairs,\n"
			"                       separated by space or Tab. One tap per line.\n"
			"                     Tap times (in seconds) must be in ascending order.\n"
			"                     There is no upper limit to the number of taps.\n"
			"                     Amplitude values must be > 0.0.\n"
			"       feedback:     amount of delay output returned to input. \n"
                        "                        0 <= feedback <= 1.0.\n"
			"       wet/dry:      sets level of wet (delayed) signal. 0.0 < wet <= 1.0.\n"
			"                     The dry (source) level is set to  1.0 - wet.\n"
			"       tailtime:     Extra time (secs) added to accommodate decay tail.\n"
			);
		return 1;
	}

	feedback= atof(argv[ARG_FEEDBACK]);
	if(fabs(feedback) > 1.0){
		printf("TDELAY: Error: feedback must be within +- 1.0\n");
		return 1;
	}
	wet =  atof(argv[ARG_WET]);
	if(wet < 0.0 || wet > 1.0){
		printf("TDELAY: wet must be within 0 to 1.0\n");
		return 1;
	}
	dry = 1.0 - wet;
	/* need overall scale by 1/2 */
	dry *=0.5;
	wet *= 0.5 * delaygain;
	tailtime = atof(argv[ARG_TAILTIME]);
	if(tailtime < 0.0){
		printf("TDELAY: error: negative value for tailtime\n");
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}																							
	infile = psf_sndOpen(argv[ARG_INFILE],&inprops,0);							/* STAGE 3 */										  
	if(infile < 0){
		printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
		error++;
		goto exit;
	}
	/* TODO: verify infile format for this application */
	if(inprops.chans != 1){
		printf("TDELAY: infile must be mono for now\n");
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
	/* create delayline. NB: mono only for now */
	delay = new_fxdelay();
	if(delay == NULL){
		puts("TDELAY: error creating delay line\n");
		error++;
		goto exit;
	}
	/* load delay tap data. Note we can't process this until we have opened the infile
	   as we need to know the srate*/
	tapfile = fopen(argv[ARG_TAPDATA],"r");
	if(tapfile == NULL){
		perror("FILE error");
		printf("TDELAY: Error: unable to open tap data file %s\n",argv[ARG_TAPDATA]);
		error++;
		goto exit;
	}
	ntaps = get_taps(tapfile,&taps);
	if(ntaps < 0){
		/* might be memroy error so we use puts */
		puts("TDELAY: error reading tapfile data: ");
		/* tell user what went wrong! */
		puts(taps_getLastError());
		error++;
		goto exit;
	}
	else if(ntaps == 0){
		printf("TDELAY: tapfile has no data!\n");
		error++;
		goto exit;
	}

	if(fxdelay_mtinit(delay, inprops.srate,taps,ntaps)){
		puts("TDELAY: error initializing delay line\n");
		error++;
		goto exit;
	}
	/* handle outfile */
	/* TODO:  make any changes to outprops here  */
	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("TDELAY: No memory!\n");
		error++;
		goto exit;
	}

	/* TODO: if outchans != inchans, allocate outframe, and modify main loop accordingly */
	outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		printf("TDELAY: Error: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	
	printf("processing....\n");									                 /* STAGE 5 */
	/* TODO: init any loop-related variables */
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);
	while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
		long i;
		
		for(i=0;i < framesread;i++){
			float input;
			input =  inframe[i];
			inframe[i] = (float)( dry * input + wet * fxdelay_mttick(delay,feedback,input));			
		}
		if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
			printf("TDELAY: Error writing to outfile\n");
			error++;
			break;
		}			
	}

	if(framesread < 0)	{
		printf("TDELAY: Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else if(tailsamps){
		long remain, i;
		do {
			remain =  tailsamps > NFRAMES ? NFRAMES : tailsamps;
			for(i=0; i < remain; i++)
				inframe[i] = (float) ( wet * fxdelay_mttick(delay,feedback,0.0));
			if(psf_sndWriteFloatFrames(outfile,inframe,remain) != remain){
				printf("TDELAY: Error writing to outfile\n");
				error++;
				goto exit;
			}
			tailsamps -= remain;
		} while (tailsamps);
	}
	
	printf("Done: %d errors\n",error);
		
	/* report PEAK values to user */									           /* STAGE 6 */
	if(psf_sndReadPeaks(outfile,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
		for(i=0;i < inprops.chans;i++){
			peaktime = (double) peaks[i].pos / (double) inprops.srate;
			printf("CH %ld:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
		}
	}	
	/* do all cleanup  */    														/* STAGE 7 */
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
	if(tapfile)
		if(0 != fclose(tapfile))
			printf("%s: Warning: error closing data file %s\n",argv[ARG_PROGNAME],argv[ARG_TAPDATA]);
	psf_finish();
	return error;
}
