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

/* biquad.c :  generic filter test program that takes raw biquad coefficients */
/* TODO: finish support for specified duration */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <portsf.h>
#include <ftools.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (4096)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_A0,ARG_A1,ARG_A2,ARG_B1,ARG_B2,ARG_NARGS};


int main(int argc, char* argv[])
{
	PSF_PROPS inprops,outprops;									/* STAGE 1 */
	long framesread;	
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	unsigned int i,do_impulse = 0;
	BQCOEFFS coeffs;
	RBJ_FILTER* filter = NULL;
	double dur = 10.0;
	long remain;

	dur++;
	printf("BIQUAD: apply raw biquadratic filter to soundfile\n");								/* STAGE 2 */
	
	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: BIQUAD infile outfile a0 a1 a2 b1 b2  \n"
			);
		return 1;
	}

	coeffs.a0 = atof(argv[ARG_A0]);
	coeffs.a1 = atof(argv[ARG_A1]);
	coeffs.a2 = atof(argv[ARG_A2]);
	coeffs.b1 = atof(argv[ARG_B1]);
	coeffs.b2 = atof(argv[ARG_B2]);
	
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	
	if(strcmp("impulse",argv[ARG_INFILE])==0){
		do_impulse = 1;
		/* set up fixed sfprops */
		inprops.srate = 44100;
		inprops.chans = 1;
		inprops.samptype = PSF_SAMP_IEEE_FLOAT;
		inprops.format = PSF_STDWAVE;
		inprops.chformat = STDWAVE;
	}
	else{

		ifd = psf_sndOpen(argv[ARG_INFILE],&inprops,0);							/* STAGE 3 */										  
		if(ifd < 0){
			printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
			error++;
			goto exit;
		}
	}

	

	/* TODO: verify infile format for this application */

	/* allocate space for  sample frame buffer ...*/
	inframe = (float*) malloc(nframes * inprops.chans * sizeof(float));
	if(inframe==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
	if(do_impulse){
		inframe[0] = 1.0;
		for(i = 1;i < nframes;i++)
			inframe[i] = 0.0;
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

	/* create filter - mono only for now */
	if(inprops.chans != 1){
		printf("FILTSF: infile must be mono for now\n");
		error++;
		goto exit;
	}
	

	

	filter = new_biquad(coeffs);
	if(filter == NULL){
		puts("BIQUAD: error creating filter\n");
		error++;
		goto exit;
	}
	

	/* handle outfile */
	/* TODO:  make any changes to outprops here  */

	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("FILTSF: No memory!\n");
		error++;
		goto exit;
	}

	/* TODO: if outchans != inchans, allocate outframe, and modify main loop accordingly */

	ofd = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("FILTSF: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	
	printf("processing....\n");									/* STAGE 5 */
	/* TODO: init any loop-related variables */
	if(dur > 0.0){
		dur *= outprops.srate;
		remain  = (long) (dur + 0.5);
	}
	else
		remain = NFRAMES;
	if(do_impulse){
		long i;
		double sum = 0.0;
		framesread = remain > NFRAMES? NFRAMES : remain;
		while(framesread > 0){
			for(i=0;i < framesread;i++){
				float input;
				input =  inframe[i];
				inframe[i] = (float) biquad_tick(filter,input);
				sum += fabs(inframe[i]);
				
			}
			if(psf_sndWriteFloatFrames(ofd,inframe,framesread) != framesread){
				printf("Error writing to outfile\n");
				error++;
				goto exit;
			}
			/* clear data! */
			for(i=0;i < NFRAMES; i++)
				inframe[i] = 0.0;
			remain -= NFRAMES;
			framesread = remain > NFRAMES? NFRAMES : remain;
		}
		/*printf("sum of impulse response = %f\n",sum);*/
		framesread = 0;
	}
	else {
		while ((framesread = psf_sndReadFloatFrames(ifd,inframe,nframes)) > 0){
			long i;
			 
			/* <--------  add buffer processing here ------>  */
			for(i=0;i < framesread;i++){
				float input;
				input =  inframe[i];
				inframe[i] = (float) biquad_tick(filter,input);				
			}
			if(psf_sndWriteFloatFrames(ofd,inframe,framesread) != framesread){
				printf("Error writing to outfile\n");
				error++;
				break;
			}			
		}
	}
	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else
		printf("Done: %d errors\n",error);
		
	/* report PEAK values to user */									/* STAGE 6 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
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
	if(ifd >= 0)
		psf_sndClose(ifd);
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(inframe)
		free(inframe);
	if(peaks)
		free(peaks);
	/*TODO: cleanup any other resources */
	if(filter)
		free(filter);
	psf_finish();
	return error;
}
