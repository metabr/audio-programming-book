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
/* conv.c :  simpel t/d convolution*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <portsf.h>
#include "fconv.h"


/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define NCOEFFS (22050)
/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_NARGS};

#ifndef unix
double log2(double val)
{
	return log(val) /log(2.0); 
}
#endif

int main(int argc, char* argv[])
{
/* STAGE 1 */	
	PSF_PROPS inprops,outprops;									
	long framesread;	
	/* init all dynamic resources to default states */
	int i,infile = -1,outfile = -1;
	clock_t starttime, endtime;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	CONV* conv= NULL;
	double expfac;
	unsigned long ncoeffs = NCOEFFS;
	double coeffs[NCOEFFS] = {0.0};
   	double sum = 0.0;
	double ascale = 1.0 / log2((double)ncoeffs);
	expfac = pow( 0.001,1.0/ncoeffs);
   /* make random table, get average */
   for(i=0; i < ncoeffs; i++){
	   double rval;
	   rval = (double) rand();
		coeffs[i] =  2.0 *  (rval / (double) RAND_MAX) - 1.0;
		sum += fabs(coeffs[i]);
   }  
   sum = sum / ncoeffs;
#ifdef DEMOCONV
   coeffs[0] = 0.5;
   coeffs[1] = -0.2;
   coeffs[2] = 0.1;
   coeffs[3] = 0.3;
#else
   for(i=0; i < ncoeffs; i++){   
		coeffs[i] *=  ascale * sum *  pow(expfac,(double)i);
   }
#endif
	/* TODO: define an output frame buffer if channel width different 	*/
	
/* STAGE 2 */
   
	printf("MAIN: generic process\n");						

	/* process any optional flags: remove this block if none used! */
	if(argc > 1){
		char flag;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			/*TODO: handle any  flag arguments here */
			case('\0'):
				printf("Error: missing flag name\n");
				return 1;
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
			"usage: MAIN infile outfile\n"
			);
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
/* STAGE 3 */																							
	infile = psf_sndOpen(argv[ARG_INFILE],&inprops,0);															  
	if(infile < 0){
		printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
		error++;
		goto exit;
	}
	/* TODO: verify infile format for this application */

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
	conv = new_conv(ncoeffs,coeffs);
	if(conv==NULL){
		puts("cannot crate CONV\n");
		error++;
		goto exit;
	}
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
		printf("Error: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
/* STAGE 5 */	
	printf("processing....\n");								
	/* TODO: init any loop-related variables */
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);
	starttime = clock();

	while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
		double input;
		unsigned long i;
		for(i=0; i < framesread; i++){
			input = (double) inframe[i];
			inframe[i] = (float) conv_tick(conv,input);
		}
		/* <--------  add buffer processing here ------>  */

		if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
			printf("Error writing to outfile\n");
			error++;
			break;
		}			
	}

	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else {
		endtime = clock();
		printf("Elapsed time =  %f secs\n",(endtime-starttime)/(double)CLOCKS_PER_SEC);
		printf("Done: %d errors\n",error);
	}
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
	if(conv){
		conv_free(conv);
		free(conv);
	}
	psf_finish();
	return error;
}
