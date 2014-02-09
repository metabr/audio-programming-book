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

/* siggen.c : generate waveform */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <wave.h>

#include "breakpoints.h"

#define NFRAMES (1024)

enum { 	ARG_PROGNAME,
	ARG_OUTFILE,
	ARG_TYPE,
	ARG_DUR,
	ARG_SRATE,
	ARG_AMP,
	ARG_FREQ,
	ARG_NARGS };

enum {  WAVE_SINE,
	WAVE_TRIANGLE,
	WAVE_SQUARE,
	WAVE_SAWUP,
	WAVE_SAWDOWN,
	WAVE_NTYPES };

int main(int argc, char *argv[])
{
	PSF_PROPS outprops;
	psf_format outformat = PSF_FMT_UNKNOWN;
	PSF_CHPEAK *peaks = NULL;

	unsigned long nframes = NFRAMES;

	int outfile = -1;
	int error = 0;
	float *outframe = NULL;
	OSCIL *p_osc = NULL;
	double (*tickfunc)(OSCIL*, double);

	BRKSTREAM *ampstream = NULL;
	FILE *fpamp = NULL;
	unsigned long brk_amp_size = 0;
	double minval, maxval;

	int wavetype;
	float dur;
	float srate;
	float amp;
	float freq;

	printf("siggen: generate waveform of set frequency\n");

	/* process any optional flags: remove this block if none used! */
	if (argc > 1) {
		char flag;
		while (argv[1][0] == '-') {
			flag = argv[1][1];
			switch (flag) {
				/*TODO: handle any  flag arguments here */
			case ('\0'):
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
	if (argc < ARG_NARGS) {
		printf("insufficient arguments.\n"
		       "usage: siggen outfile wavetype dur srate amp freq\n"
		       "where wavetype:\n"
		       "0 - sine\n"
		       "1 - triangle\n"
		       "2 - saw up\n"
		       "3 - saw down\n"
		       "4 - square\n");
		return 1;
	}

	/* Reading user supplied arguments */
	wavetype = atoi(argv[ARG_TYPE]);
	if (wavetype < WAVE_SINE || wavetype > WAVE_NTYPES) {
		printf("Error: incorrect value for waveform type\n");
		return 1;
	}

	dur = atof(argv[ARG_DUR]);
	printf("Duration: %f\n", dur);

	srate = atof(argv[ARG_SRATE]);
	printf("Sample rate: %f\n", srate);

	freq = atof(argv[ARG_FREQ]);
	printf("Frequency: %f\n", freq);

	/* always startup portsf */
	if (psf_init()) {
		printf("unable to start portsf\n");
		return 1;
	}

	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if (outformat == PSF_FMT_UNKNOWN) {
		printf("outfile name %s has unknown format.\n"
		       "Use any of .wav, .aiff, .aif, .afc, .aifc\n",
		       argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}

	outprops.srate = srate;
	outprops.chans = 1;
	outprops.samptype = PSF_SAMP_16;
	outprops.chformat = STDWAVE;
	outprops.format = outformat;

	outfile = psf_sndCreate(argv[ARG_OUTFILE], &outprops, 0, 0, PSF_CREATE_RDWR);
	if (outfile < 0) {
		printf("Error: unable to create outfile %s\n",
		       argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}

	outframe = malloc(nframes * outprops.chans * sizeof(*outframe));
        if (outframe == NULL) {
                puts("No memory!\n");
                error++;
                goto exit;
        }

        peaks = malloc(outprops.chans * sizeof(*peaks));
        if (peaks == NULL) {
                puts("No memory!\n");
                error++;
                goto exit;
        }

	/* open breakpoint file, or set constant */
	fpamp = fopen(argv[ARG_AMP], "r");
	if (fpamp == NULL) {
		amp = atof(argv[ARG_AMP]);
		printf("Amplitude: %f\n", amp);
		if (amp <= 0.0 || amp > 1.0) {
			printf("Error: amplitude value out of range: "
			       "0.0 < amp < 1.0\n");
			error++;
			goto exit;
		}
	} else {
		ampstream = bps_newstream(fpamp, outprops.srate, &brk_amp_size);
		if (ampstream == NULL) {
			printf("Error reading ampstream from file %s\n", argv[ARG_AMP]);
			error++;
			goto exit;
		}

		if (bps_getminmax(ampstream, &minval, &maxval) != 0) {
			printf("Error reading range of breakpoints file %s\n",
			       argv[ARG_AMP]);
			error++;
			goto exit;
		}
		if (minval < 0.0 || minval > 1.0 || maxval < 0.0 || maxval > 1.0) {
			printf("Error: amplitude values out of range in file %s: "
			       "0.0 < amp < 1.0\n",
			       argv[ARG_AMP]);
			error++;
			goto exit;
		}

	}

	switch (wavetype) {
	case (WAVE_SINE):
		tickfunc = sinetick;
		break;
	case (WAVE_TRIANGLE):
		tickfunc = tritick;
		break;
	case (WAVE_SAWUP):
		tickfunc = sawuptick;
		break;
	case (WAVE_SAWDOWN):
		tickfunc = sawdowntick;
		break;
	case (WAVE_SQUARE):
		tickfunc = squaretick;
		break;
	}

	printf("processing....\n");

	p_osc = new_oscil(srate);

	unsigned long nbufs, outframes, remainder;
	outframes = (unsigned long) (dur * outprops.srate + 0.5);
	nbufs = outframes / nframes;
	remainder = outframes - nbufs * nframes;
	if (remainder > 0)
		nbufs++;

	/* main processing loop */
	for (int i = 0; i < nbufs; i++) {
		if (i == nbufs-1)
			nframes = remainder;

		for (int j = 0; j < nframes; j++) {
			if (ampstream)
				amp = bps_tick(ampstream);
			outframe[j] = (float) (amp * tickfunc(p_osc, freq));
		}

		if (psf_sndWriteFloatFrames(outfile, outframe, nframes) != nframes) {
			printf("Error writing to outfile\n");
			error++;
			break;
		}
	}

        if (psf_sndReadPeaks(outfile, peaks, NULL) > 0) {
                double peaktime;
                printf("PEAK information:\n");
                for (int i = 0; i < outprops.chans; i++) {
                        peaktime = (double) peaks[i].pos / (double) outprops.srate;
                        printf("CH %d:\t%.4f at %.4f secs\n", i + 1, peaks[i].val, peaktime);
                }
        }

exit:
	if (outfile >= 0)
		if (psf_sndClose(outfile))
			printf("%s: Warning: error closing outfile %s\n", argv[ARG_PROGNAME], argv[ARG_OUTFILE]);
	if (outframe)
		free(outframe);
	if (peaks)
		free(peaks);
	if (p_osc)
		free(p_osc);
	if (ampstream)
		free(ampstream);
	if (fpamp)
		fclose(fpamp);

	psf_finish();
	return error;
}
