/* filtsf.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "portsf.h"
#include "breakpoints.h"
#include "ftools.h"


/* set size of multi-channel frame-buffer */
#define NFRAMES (4096)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_FTYPE,ARG_CFREQ,ARG_Q,ARG_NARGS};

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
	BW_FILTER* filter = NULL;
	double cfreq = 0.0;
	double bw, Q = 1.0;
	double Nyquist = 0.0;
	FX_RESON* reson = NULL;


	printf("BWFILTER: apply Butterworth filter to soundfile\n");								/* STAGE 2 */
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
	/*FXFILT_LP,FXFILT_HP,FXFILT_BP,FXFILT_BPQ,FXFILT_NOTCH,FXFILT_ALLPASS*/
	if(argc < ARG_NARGS-1){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: BWFILTER infile outfile type cfreq [Q]  \n"
			"  filter types: 0 = LowPass\n"
			"                1 = HighPass\n"
			"                2 = BandPass\n"
			"                3 = Notch\n"
			"       cfreq    = centre/cutoff freq (0 - Nyquist)\n"
			"       Q        = quality factor for bandpass and notch filters (0 < Q <= 100)\n"
			"                  filter bandwidth set as cfreq / Q\n"
			"                (Q is required only for types 2 and 3)\n"
			);
		return 1;
	}

	ftype = atoi(argv[ARG_FTYPE]);
	
	if(ftype < 0 || ftype > 3 ){
		printf("FILTSF: Error: unknown filter type\n");
		return 1;
	}
	if(ftype > 1){
		if(argc < ARG_NARGS){
			printf("BWFILTER: Q parameter is required for bandpass and notch filters\n");
			return 1;
		}
		else{
			Q = atof(argv[ARG_NARGS-1]);
			if(Q < 1.0 || Q > 100){
				printf("BWFILTER: Error: Q value out of range (1 <= Q <= 100) \n");
				return 1;
			}
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
	if(fpfreq == NULL){
		cfreq = atof(argv[ARG_CFREQ]);
		if(cfreq <= 0.0){
			printf("Error: freq must be positive\n");
			error++;
			goto exit;
		}
		if(cfreq > Nyquist){
			printf("Error: cfreq above Nyquist (%d)\n",Nyquist );
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
			printf("Error: frequency values above %ld in breakpoint file %s\n", Nyquist, argv[ARG_CFREQ]);
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
	/* only required for bandpass and notch */
	bw  =  cfreq / Q;
	
	filter = new_bw_filter();
	if(filter == NULL){
		puts("FILTSF: error creating filter\n");
		error++;
		goto exit;
	}
	if(bw_init(filter, (BW_FILTERTYPE) ftype, inprops.srate,cfreq,bw)){
		puts("FILTSF: error initializing Buttewrworth filter\n");
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

	outfile = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(outfile < 0){
		printf("FILTSF: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	if(filter){
		BQCOEFFS coeffs = get_bw_coefficients(filter);
		printf("Coefficients: (aN = past inputs, bN = past outputs)\n"
				"a0 = %f\n"
				"a1 = %f\n"
				"a2 = %f\n"
				"b1 = %f\n"
				"b2 = %f\n", coeffs.a0,coeffs.a1,coeffs.a2,coeffs.b1,coeffs.b2);
	}

	printf("processing....\n");									/* STAGE 5 */
	/* TODO: init any loop-related variables */
	psf_sndSetDither(outfile,PSF_DITHER_TPDF);
	starttime = clock();

	if(do_impulse){
		long i;
		double sum = 0.0;
		framesread = NFRAMES;
		for(i=0;i < framesread;i++){
			float input;
			input =  inframe[i];
			inframe[i] = (float) bw_tick(filter,input,cfreq,Q);
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
		double invQ = 1.0 / Q;

		while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
			long i;
			 
			/* <--------  add buffer processing here ------>  */
			for(i=0;i < framesread;i++){
				float input;
				input =  inframe[i];
				if(freqstream) {
					cfreq = bps_tick(freqstream);
					bw = cfreq * invQ;
				}
				inframe[i] = (float) bw_tick(filter,input,cfreq,bw);				
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
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
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
	if(filter)
		free(filter);
	if(reson)
		free(reson);
	psf_finish();
	return error;
}
