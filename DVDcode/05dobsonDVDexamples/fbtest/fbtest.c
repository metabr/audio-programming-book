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

/* fbtest.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <time.h>
#include <ftools.h>
#include <breakpoints.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (32768)
#define MAX_FILTERS (24)
#define SEMITONE (1.0594631)
/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_BOTFREQ,ARG_FREQ,ARG_Q,ARG_GAIN,ARG_WARP, ARG_NARGS};

int main(int argc, char* argv[])
{
/* STAGE 1 */	
	PSF_PROPS inprops,outprops;									
	long framesread;	
	/* init all dynamic resources to default states */
	int infile = -1,outfile = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;

	/* TODO: define an output frame buffer if channel width different 	*/
	RBJ_FILTER*  filterbank[MAX_FILTERS] = {NULL};
	double filter_freqs[MAX_FILTERS] = {0.0};
	int i,j,nfilters = 0;
	double nyquist,this_output,filter_botfreq,filter_topfreq,filter_Q,filter_gain,filter_warp = 1.0;
	BRKSTREAM* warpstream = NULL, *freqstream = NULL;
	FILE* fp_warp = NULL, *fp_freq = NULL;
	int new_freq = 0,new_warp = 0;
	clock_t starttime, endtime;
/* STAGE 2 */	
	printf("fbtest: subharmonic filterbank.\n");						

	

	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: fbtest infile outfile botfreq topfreq Q gain freqwarp\n"
			"    botfreq : lower frequency limit of filterbank: 100 <= botfreq < nyquist\n"
			"    topfreq : upper frequency limitm of filterbank: 0 < topfreq < nyquist\n"
			"    Q       : resonance factor for filters: 0 < Q < 1000\n"
			"    gain    : output amplitude factor: gain > 0\n"
			"    freqwarp: warp factor for filter frequencies: 1.0 = no warp\n"
			"              warp is relative to topfreq. \n"
			"              small deviations from 1.0 recommended. \n"
			"              deviations above 1.0 will force lower filters to cut out\n"
			""
			"    topfreq and freqwarp can vary over time\n"
			"    Up to 24 filters are available.\n"
			"    NB: minimum permitted interval between adjacent filters = 1 semitone.\n"
			);
		return 1;
	}

	filter_Q = atof(argv[ARG_Q]);
	if(filter_Q <= 0.0 || filter_Q > 1000){
		printf("Error: filter_Q value out of range\n");
		return 1;
	}
	filter_gain = atof(argv[ARG_GAIN]);
	if(filter_gain <= 0.0){
		printf("Error: gain value must be positive\n");
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
	if(inprops.chans > 1){
		printf("Sorry_ infile must be mono\n");
		error++;
		goto exit;
	}
	nyquist = (double) inprops.srate / 2.0;

	fp_warp = fopen(argv[ARG_WARP],"r");
	if(fp_warp == NULL){
		filter_warp = atof(argv[ARG_WARP]);
		if(filter_warp <= 0.0){
			printf("Error: freqwarp value must be positive\n");
			return 1;
		}
	}
	else {
		double minval, maxval;
		warpstream = bps_newstream(fp_warp,inprops.srate, NULL);
		if(warpstream == NULL){
			printf("Error: unable to read breakpoint data from %s\n",argv[ARG_WARP]);
			error++;
			goto exit;
		}
		fclose(fp_warp);
		bps_getminmax(warpstream,&minval,&maxval);
		if(minval <= 0.0 || maxval <= 0.0){
			printf("Error: out of range data in freqwarp breakpoint file %s\n",argv[ARG_WARP]);
			error++;
			goto exit;
		}
	}

	fp_freq = fopen(argv[ARG_FREQ],"r");
	if(fp_freq == NULL){
		filter_topfreq = atof(argv[ARG_FREQ]);
		if(filter_topfreq < 100.0 || filter_topfreq > nyquist){
			printf("Error: filter topfreq out of range\n");
			error++;
			goto exit;
		}
		
	}
	else {
		double minval, maxval;
		freqstream = bps_newstream(fp_freq,inprops.srate, NULL);
		if(freqstream == NULL){
			printf("Error: unable to read breakpoint data from %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		fclose(fp_freq);
		bps_getminmax(freqstream,&minval,&maxval);
		if(minval <= 0.0 || maxval <= 0.0 || minval >= nyquist || maxval >= nyquist ){
			printf("Error: out of range data in freqwarp breakpoint file %s\n",argv[ARG_WARP]);
			error++;
			goto exit;
		}

		filter_topfreq = bps_tick(freqstream);
		bps_rewind(freqstream);
	}

	filter_botfreq = atof(argv[ARG_BOTFREQ]);
	if(filter_botfreq < 100.0 || filter_botfreq > nyquist){
		printf("Error: filter botfreq out of range\n");
		error++;
		goto exit;
	}
	if(filter_topfreq <= filter_botfreq){
		printf("Error:  topfreq must be > botfreq\n");
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
	/*setup filterbank */
	/*at least one filter: */
	filter_freqs[0] = filter_topfreq;
	filterbank[0] = create_filter(RBJ_BP,inprops.srate,filter_freqs[0],filter_Q);
	if(filterbank[0] == NULL){
		puts("no memory for filterbank\n");
		error++;
		goto exit;
	}
	for(i = 1;i < MAX_FILTERS;i++){
		double harmfreq,warpfreq;
		harmfreq = filter_topfreq /(i+1);
		warpfreq =  filter_topfreq - ((filter_topfreq - harmfreq)  * filter_warp);
		if(warpfreq < filter_botfreq)	{
			nfilters = i;			
			break;
		}
		if(filter_freqs[i-1] / warpfreq < SEMITONE){
			nfilters = i;			
			break;		
		}

		filter_freqs[i] = warpfreq;
		filterbank[i] = create_filter(RBJ_BP,inprops.srate,warpfreq,filter_Q);
		if(filterbank[i] == NULL){
			puts("no memory for filterbank\n");
			error++;
			goto exit;
		}
	}

	printf("using %d filters\n",nfilters);

	printf("Filter frequencies:\n");
	for(i=0;i < nfilters; i++)
		printf("%d:\t%.4f\n",i+1,filter_freqs[i]);


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
	starttime = clock();
	while ((framesread = psf_sndReadFloatFrames(infile,inframe,nframes)) > 0){
		int current_nfilters = nfilters;
		double warpval;
	//	double this_dither;
		for(i=0; i < framesread; i++){
		//	this_dither = (float)((( 2.0 * rand()/ (double) RAND_MAX) - 1.0) * 1.0e-10);
			this_output = 0.0;
			if(freqstream){
				filter_topfreq = bps_tick(freqstream);
				filter_freqs[0] = filter_topfreq;
				new_freq = 1;
			}

			if(warpstream){
				warpval = bps_tick(warpstream);
				new_warp = 1;
			}
			else
				warpval = filter_warp;
			if(new_freq || new_warp){
				int k;
				current_nfilters = nfilters;
				for(k=1;k < nfilters;k++){
					double harmfreq,warpfreq;
					harmfreq = filter_topfreq /(k+1);
					warpfreq =  filter_topfreq - ((filter_topfreq - harmfreq)  * warpval);
					if(warpfreq < 50.0)	{						
						current_nfilters = k;
#ifdef _DEBUG
						printf("Using %d filters\r",current_nfilters);
#endif
						break;
					}					
					filter_freqs[k] = warpfreq;				
				}
				new_freq = 0; new_warp = 0;
			}
			for(j=0; j < current_nfilters; j++) 
				this_output	+= rbj_filtertick(filterbank[j],inframe[i], filter_freqs[j], filter_Q);
			inframe[i] = (float) (this_output * filter_gain);
		}
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
		printf("\nElapsed time =  %f secs\n",(endtime-starttime)/(double)CLOCKS_PER_SEC);
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
	
	for(i=0;i < nfilters; i++)
		if(filterbank[i])
			free(filterbank[i]);
	if(warpstream) {
		bps_freepoints(warpstream);
		free(warpstream);
	}
	psf_finish();
	return error;
}
