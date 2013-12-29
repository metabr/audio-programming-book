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

/* filtsf.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <portsf.h>
#include <breakpoints.h>
#include <ftools.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (4096)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_FTYPE,ARG_CFREQ,ARG_NARGS};
#ifdef RWD_PRIVATE
/* an experimental variant of reson - ~probably~ not to be used for book */
enum {DJ_LOWPASS, DJ_HIGHPASS, DJ_ALLPOLE, DJ_POLEZERO, VRESON, DJ_FILTERTYPES};
#else
enum {DJ_LOWPASS, DJ_HIGHPASS, DJ_ALLPOLE, DJ_POLEZERO,  DJ_FILTERTYPES};
#endif

int main(int argc, char* argv[])
{
	PSF_PROPS inprops,outprops;									/* STAGE 1 */
	long framesread;	
	/* init all dynamic resources to default states */
	int infile = -1,outfile = -1;
	int error = 0;
	clock_t starttime, endtime;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	BRKSTREAM *freqstream = NULL;
	FILE* fpfreq = NULL;
	double freq_minval = 0.0,freq_maxval = 0.0;
	unsigned long brkfreqSize = 0;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	int ftype;
	unsigned int i,do_impulse = 0;
	double cfreq = 0.0, bw = 1.0;
	double Q = 1.0, invQ = 1.0;
	double Nyquist = 0.0;
	FX_RESON* reson = NULL;
	TONE* lphp = NULL;

	printf("FILTSF: apply simple filter to soundfile\n");								/* STAGE 2 */
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
	/*DJ_LOWPASS, DJ_HIGHPASS, DJ_BANDPASS,DJ_ALLPOLE,DJ_POLEZERO,DJ_FILTERTYPES*/
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: FILTSF infile outfile type cfreq [Q]  \n"
			"  filter types: 0 = LowPass\n"
			"                1 = HighPass\n"
			"                2 = allpole bandpass\n"
			"                3 = polezero bandpass\n"
#ifdef RWD_PRIVATE
			"                4 = variable resonator\n"
#endif
			"           cfreq  = centre/cutoff freq (0 < cfreq < Nyquist)\n"
			"           cfreq may also be a breakpoint file\n"
			"           Q      = resonance factor (1 - 100)\n"
			"           Q is required for bandpass options.\n"
			
			);
		return 1;
	}

	ftype = atoi(argv[ARG_FTYPE]);
	
	if(ftype < 0 || ftype >= DJ_FILTERTYPES ){
		printf("FILTSF: Error: unknown filter type\n");
		return 1;
	}
	if(ftype > DJ_HIGHPASS && argc == ARG_NARGS){
		printf("FILTSF: reson filter types require Q argument\n");
		return 1;
	}
	if(argc == ARG_NARGS + 1){
		Q = atof(argv[ARG_NARGS]);
		if(Q < 1.0 || Q > 100){
			printf("FILTSF: Error: Q value out of range (1 --- 100) \n");
			return 1;
		}
	}
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

		infile = psf_sndOpen(argv[ARG_INFILE],&inprops,0);							/* STAGE 3 */										  
		if(infile < 0){
			printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
			error++;
			goto exit;
		}
	}
	Nyquist = (double) (inprops.srate / 2);
	fpfreq = fopen(argv[ARG_CFREQ],"r");
	if(fpfreq== NULL){
		cfreq = atof(argv[ARG_CFREQ]);
		if(cfreq <= 0.0){
			printf("Error: freq must be positive\n");
			error++;
			goto exit;
		}
		if(cfreq > Nyquist){
			printf("Error: cfreq above Nyquist (%d)\n",inprops.srate/2 );
			error++;
			goto exit;
		}
	}
	else {
		freqstream = bps_newstream(fpfreq,inprops.srate,&brkfreqSize);
		if(freqstream == NULL) {
			printf("Error reading freq breakpoint file %s\n",argv[ARG_CFREQ]);
			error++;
			goto exit;
		}
		if(bps_getminmax(freqstream,&freq_minval,&freq_maxval)) {
			printf("Error reading range of breakpoint file %s\n",argv[ARG_CFREQ]);
			error++;
			goto exit;
		}
		if(freq_minval <= 0.0 || freq_maxval <= 0.0) {
			printf("Error: negative frequency values in breakpoint file %s\n",argv[ARG_CFREQ]);
			error++;
			goto exit;
		}
		if(freq_minval >= Nyquist  || freq_maxval >= Nyquist){
			printf("Error: frequency values above %lf in breakpoint file %s\n", Nyquist, argv[ARG_CFREQ]);
			error++;
			goto exit;
		}

		/* init cfreq wqith first value, then rewind stream ready for process loop */
		cfreq = bps_tick(freqstream);
		bps_rewind(freqstream);
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
	if(ftype >= DJ_ALLPOLE){
		reson = new_reson();
		if(reson == NULL){
			puts("FILTSF: error creating allpole filter\n");
			error++;
			goto exit;
		}
		if(ftype == DJ_ALLPOLE){
			if(allpole_init(reson,inprops.srate,cfreq,cfreq/Q)){
				puts("FILTSF: error initializing allpole filter\n");
				error++;
				goto exit;
			}
		}
		else if(ftype == DJ_POLEZERO){
			if(polezero_init(reson,inprops.srate,cfreq,cfreq/Q)){
				puts("FILTSF: error initializing polezero filter\n");
				error++;							 
				goto exit;
			}

		}
#ifdef RWD_PRIVATE
		else if(ftype == VRESON){
			if(vreson_init(reson,inprops.srate,cfreq,cfreq/Q)){
				puts("FILTSF: error initializing vreson filter\n");
				error++;							 
				goto exit;
			}
		}
#endif
	}
	else {
		lphp = new_tone();
		if(lphp == NULL){
			puts("FILTSF: error creating lphp filter\n");
			error++;
			goto exit;
		}
		tone_init(lphp,inprops.srate,cfreq,ftype);
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

	outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		printf("FILTSF: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	/* print filter coefficients */
	if(reson){
		BQCOEFFS coeffs = get_coefficients(reson);
		printf("Coefficients: (aN = past inputs, bN = past outputs)\n"
				"a0 = %f\n"
				"a1 = %f\n"
				"a2 = %f\n"
				"b1 = %f\n"
				"b2 = %f\n", coeffs.a0,coeffs.a1,coeffs.a2,coeffs.b1,coeffs.b2);
	}
	else if(lphp){
		BQCOEFFS coeffs = get_tone_coefficients(lphp);

		printf("Coefficients: (aN = past inputs, bN = past outputs)\n"
				"a0 = %f\n"
				"b1 = %f\n"
				, coeffs.a0,coeffs.b1);
	}

	printf("processing....\n");									/* STAGE 5 */
	/* TODO: init any loop-related variables */
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);
	invQ = 1.0 / Q;
	starttime = clock();
	if(do_impulse){
		long i;
		double sum = 0.0;
		framesread = NFRAMES;

		bw = cfreq * invQ;
		for(i=0;i < framesread;i++){
			float input;
			input =  inframe[i];
			
			if(ftype == DJ_ALLPOLE)	
				inframe[i] = (float) allpole_utick(reson,input,cfreq,bw);
			else if (ftype == DJ_POLEZERO)
				inframe[i] = (float) polezero_tick(reson,input,cfreq,bw);
#ifdef RWD_PRIVATE
			else if(ftype == VRESON)
				inframe[i] = (float) vreson_tick(reson,input,cfreq,bw);
#endif
			else
				inframe[i] = (float) tone_tick(lphp,input,cfreq);
			sum += fabs(inframe[i]);
				
		}
		if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
			printf("Error writing to outfile\n");
			error++;
			goto exit;
		}
		printf("sum of impulse response = %f\n",sum);
	}
	else {
		while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
			long i;
			 
			/* <--------  add buffer processing here ------>  */
			for(i=0;i < framesread;i++){
				float input;
				input =  inframe[i];
				if(freqstream)
					cfreq = bps_tick(freqstream);
				bw = cfreq * invQ;
				if(ftype == DJ_ALLPOLE){
/*
	*** alternative interface - see txt Ch 2.8. ****

					allpole_set_cf(reson, cfreq);
					allpole_set_bw(reson,bw);
					inframe[i] = (float) allpole_tick(reson,input);
*/
					inframe[i] = (float) allpole_utick(reson,input,cfreq,bw);
				}
				else if (ftype == DJ_POLEZERO)
					inframe[i] = (float) polezero_tick(reson,input,cfreq,bw);
#ifdef RWD_PRIVATE
				else if(ftype == VRESON)
					inframe[i] = (float) vreson_tick(reson,input,cfreq,bw);
#endif
				else
				    inframe[i] = (float) tone_tick(lphp,input,cfreq);	
			}
			if(psf_sndWriteFloatFrames(outfile,inframe,framesread) != framesread){
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
	else {
		endtime = clock();
		printf("Elapsed time =  %f secs\n",(endtime-starttime)/(double)CLOCKS_PER_SEC);
		printf("Done: %d errors\n",error);
	}
	/* report PEAK values to user */									/* STAGE 6 */
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
		psf_sndClose(infile);
	if(outfile >= 0)
		psf_sndClose(outfile);
	if(inframe)
		free(inframe);
	if(peaks)
		free(peaks);
	/*TODO: cleanup any other resources */
	if(reson)
		free(reson);
	if(lphp)
		free(lphp);
	psf_finish();
	return error;
}
