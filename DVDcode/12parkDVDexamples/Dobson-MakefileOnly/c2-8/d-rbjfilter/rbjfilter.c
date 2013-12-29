/* rbjfilter.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "portsf.h"
#include "breakpoints.h"
#include "ftools.h"
#include "cfsamp.h"
/* set size of multi-channel frame-buffer */
#define NFRAMES (4096)
#ifdef RWD_PRIVATE
#define PLOTLEN (1024)
#endif
/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_FTYPE,ARG_CFREQ,ARG_Q,ARG_GAIN,ARG_NARGS};

int main(int argc, char* argv[])
{
	PSF_PROPS inprops,outprops;									/* STAGE 1 */
	long framesread;	
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
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
	double gain = 1.0;
	RBJ_FILTER* filter = NULL;
	RBJ_SHELF* shelf = NULL;
	double cfreq = 0.0;
	double Q = 1.0;
	double dBGain = 0.0;
	double Nyquist = 0.0;
#ifdef RWD_PRIVATE
	/* stuff for memplot */
	double phase[PLOTLEN];
	double mag[PLOTLEN];
	double phase_logf[PLOTLEN];
	double mag_logf[PLOTLEN];
#endif
	printf("RBJFILTER: apply RBJ filter to soundfile\n");								/* STAGE 2 */
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
	/*RBJ_LP,RBJ_HP,RBJ_BP,RBJ_NOTCH,RBJ_BPQ,RBJ_ALLPASS,RBJ_LOWSHELF,RBJ_HIGHPASS,RBJ_PEAKEQ*/
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: rbjfilter infile outfile type cfreq Q  gain [dBGAIN]\n"
			"  filter types: %d = LowPass\n"
			"                %d = HighPass\n"
			"                %d = BandPass\n"
			"                %d = Notch\n"		
			"                %d = Allpass\n"
			"                %d = low shelf	 (requires dBGain)\n"
			"                %d = high shelf  (requires dBGain)\n"
			"                %d = peaking EQ\n"
			"  cfreq            = centre/cutoff freq (0 - Nyquist)\n"
			"  Q                = resonance factor (0 < Q < 100)\n"
			"  gain             = gain factor applied to output. gain > 0.0.\n"
			"  dBGain			= cut/boost (dB) for shelf and peaking EQ filters\n"
			,RBJ_LP,RBJ_HP,RBJ_BP,RBJ_NOTCH,RBJ_ALLPASS,RBJ_LOWSHELF,RBJ_HIGHSHELF,RBJ_PEAKEQ);
		return 1;
	}

	ftype = atoi(argv[ARG_FTYPE]);
	
	if(ftype < 0 || ftype > RBJ_PEAKEQ ){
		printf("rbjfilter: Error: unknown filter type.\n");
		return 1;
	}
	Q = atof(argv[ARG_Q]);
	if(Q <= 0.0 || Q > 100){
		printf("rbjfilter: Error: Q value out of range (0  < Q < 100). \n");
		return 1;
	}
	gain = atof(argv[ARG_GAIN]);
	if(gain <= 0.0){
		printf("Error: gain must be positive.\n");
		return 1;
	}
	if(ftype >= RBJ_LOWSHELF){
		if(argc < ARG_NARGS){
			printf("insufficient arguments: filter requires dBGain.\n");
			return 1;
		}
		else{
			dBGain = atof(argv[ARG_NARGS]);
			/* set any limits? */
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
		ifd = psf_sndOpen(argv[ARG_INFILE],&inprops,0);							/* STAGE 3 */										  
		if(ifd < 0){
			printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
			error++;
			goto exit;
		}
	}
	Nyquist = (double)(inprops.srate / 2);
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
			printf("Error: frequency values above %ld in breakpoint file %s\n", Nyquist, argv[ARG_CFREQ]);
			error++;
			goto exit;
		}
		/* init cfreq wqith first value, then rewind stream ready for process loop */
		cfreq = bps_tick(freqstream);
		bps_rewind(freqstream);
	}
	/* NB: time-varying support for Q and dBGain is also possible, and is left as an exercise... */

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
		printf("rbjfilter: infile must be mono for now\n");
		error++;
		goto exit;
	}
	if (ftype > RBJ_ALLPASS){
		RBJ_SHELFTYPE type;

		type = (RBJ_SHELFTYPE) (ftype - (RBJ_ALLPASS + 1));
		shelf = create_shelf(type,inprops.srate,cfreq,Q,dBGain);
		if(shelf == NULL){
			puts("rbjfilter: error creating filter\n");
			error++;
			goto exit;
		}
	}
	
	else {
		filter = create_filter((RBJ_FILTERTYPE)ftype,inprops.srate,cfreq,Q);
		if(filter == NULL){
			puts("rbjfilter: error creating filter\n");
			error++;
			goto exit;
		}
	}
	/* handle outfile */
	/* TODO:  make any changes to outprops here  */

	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("rbjfilter: No memory!\n");
		error++;
		goto exit;
	}

	/* TODO: if outchans != inchans, allocate outframe, and modify main loop accordingly */

	ofd = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("rbjfilter: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	
	printf("processing....\n");									/* STAGE 5 */
	/* TODO: init any loop-related variables */	
	if(filter){
#ifdef RWD_PRIVATE
		double pi2 = M_PI / 2.0;
		double invPI2 = 1.0 / pi2;
		double maxval = 0.0;

		double lmin = 10.0;
		double lmax = 22050.0;
		double logrange = lmax - lmin;
		double logfac = pow(logrange/lmin,1.0 / PLOTLEN);
		double logval = lmin;
#endif

		BQCOEFFS coeffs = get_rbj_coefficients(filter);

		printf("Coefficients: (aN = past inputs, bN = past outputs)\n"
				"a0 = %.10f\n"
				"a1 = %.10f\n"
				"a2 = %.10f\n"
				"b1 = %.10f\n"
				"b2 = %.10f\n", coeffs.a0,coeffs.a1,coeffs.a2,coeffs.b1,coeffs.b2);
#ifdef RWD_PRIVATE
		/* only for diagnostics */
		for(i=0;i < PLOTLEN;i++){
			double freqincr = (inprops.srate / 2.0) / PLOTLEN;
			double absval;
			mag[i] =   cf_mag(/*i* freqincr*/logval, inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2);
			mag_logf[i] = 1.0 + (20.0 * log10(mag[i]) / 200.0);
			phase[i] = cf_phi(i* freqincr, inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2) - pi2;

			phase_logf[i] = cf_phi(logval,inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2) - pi2;
			logval *= logfac;

			absval = fabs(phase[i]);
			maxval = absval > maxval ? absval : maxval;
		}
		/* normalize to +-1.0 for display */
		phase[0] = phase[1];

		for(i=0;i < PLOTLEN;i++) {
			//phase[i] *=invPI2;
			phase[i] /= maxval;
			phase_logf[i] /= M_PI;
		}
#endif
	}
	else if(shelf){
#ifdef RWD_PRIVATE
		double pi2 = M_PI / 2.0;
		//double invPI2 = 1.0 / pi2;
		double lmin = 10.0;
		double lmax = 22050.0;
		double logrange = lmax - lmin;
		double logfac = pow(logrange/lmin,1.0 / PLOTLEN);
		double logval = lmin;
		double maxval = 0.0;
#endif
		BQCOEFFS coeffs = get_shelf_coefficients(shelf);

		printf("Coefficients: (aN = past inputs, bN = past outputs)\n"
				"a0 = %.10f\n"
				"a1 = %.10f\n"
				"a2 = %.10f\n"
				"b1 = %.10f\n"
				"b2 = %.10f\n", coeffs.a0,coeffs.a1,coeffs.a2,coeffs.b1,coeffs.b2);
#ifdef RWD_PRIVATE
		/* only for diagnostics */
		for(i=0;i < PLOTLEN;i++){
			double freqincr = (inprops.srate / 2.0) / PLOTLEN;
			double absval;
			mag[i] =   cf_mag(/* i* freqincr*/logval, inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2);
			mag_logf[i] = 1.0 + (20.0 * log10(mag[i]) / 200.0);
			phase[i] = cf_phi(/*i* freqincr*/logval, inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2) - pi2;
			phase_logf[i] = cf_phi(logval,inprops.srate,coeffs.a0,coeffs.a1,coeffs.a2,1.0,coeffs.b1,coeffs.b2) - pi2;
			logval *= logfac;
			absval = fabs(phase[i]);
			maxval = absval > maxval ? absval : maxval;
		}
		/* normalize to +-1.0 for display */
		for(i=0;i < PLOTLEN;i++) {
			//phase[i] *=invPI2;
			phase[i] /= maxval;
			phase_logf[i] /= M_PI;
		}
#endif
	}

	
	if(do_impulse){
		long i;
		framesread = NFRAMES;
		for(i=0;i < framesread;i++){
			float input;
			input =  inframe[i];
			
			if(ftype >= RBJ_LOWSHELF)
				inframe[i] = (float) (gain * rbj_shelftick(shelf,input, cfreq, Q,dBGain));			
			else
				inframe[i] = (float) (gain * rbj_filtertick(filter,input,cfreq,Q));		
		}
		if(psf_sndWriteFloatFrames(ofd,inframe,framesread) != framesread){
			printf("Error writing to outfile\n");
			error++;
			goto exit;
		}
	}
	else {
		//double invQ = 1.0 / Q;
		while ((framesread = psf_sndReadFloatFrames(ifd,inframe,nframes)) > 0){
			long i;
			 
			/* <--------  add buffer processing here ------>  */
			for(i=0;i < framesread;i++){
				float input;
				input =  inframe[i];
				if(freqstream)
					cfreq = bps_tick(freqstream);
				if(ftype >= RBJ_LOWSHELF)
					inframe[i] = (float) (gain * rbj_shelftick(shelf,input, cfreq, Q,dBGain));
				else
					inframe[i] = (float) (gain * rbj_filtertick(filter,input,cfreq,Q));
				
				
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
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
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
	if(shelf)
		free(shelf);
	psf_finish();
	return error;
}
