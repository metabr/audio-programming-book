/* oscgen.c : generate waveform using additive synthesis */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <wave.h>

#include "breakpoints.h"

#define NFRAMES (1024)

enum { 	ARG_PROGNAME,
	ARG_OUTFILE,
	ARG_DUR,
	ARG_SRATE,
	ARG_AMP,
	ARG_FREQ,
	ARG_WAVETYPE,
	ARG_NOSCS,
	ARG_NARGS };

enum {  WAVE_SQUARE,
	WAVE_TRIANGLE,
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
	OSCIL **osc_bank = NULL;
	double *osc_amps = NULL;
	double *osc_freqs = NULL;

	BRKSTREAM *ampstream = NULL;
	FILE *fpamp = NULL;
	unsigned long brk_amp_size = 0;
	double minval, maxval;

	unsigned int osc_number;
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
		printf("usage: oscgen outfile dur srate amp freq wavetype noscs\n"
		       "  where wavetype:\n"
		       "	0 - square\n"
		       "	1 - triangle\n"
		       "	2 - saw up\n"
		       "	3 - saw down\n");
		return 1;
	}

	/* Reading user supplied arguments */
	osc_number = atoi(argv[ARG_NOSCS]);

	wavetype = atoi(argv[ARG_WAVETYPE]);
	if (wavetype < WAVE_SQUARE || wavetype > WAVE_NTYPES) {
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

	osc_amps = malloc(osc_number * sizeof(*osc_amps));
        if (osc_amps == NULL) {
                puts("No memory!\n");
                error++;
                goto exit;
        }

	osc_freqs = malloc(osc_number * sizeof(*osc_freqs));
        if (osc_freqs == NULL) {
                puts("No memory!\n");
                error++;
                goto exit;
        }

	osc_bank = malloc(osc_number * sizeof(*osc_bank));
        if (osc_bank == NULL) {
                puts("No memory!\n");
                error++;
                goto exit;
        }

	// create oscillators
	for (int i = 0; i < osc_number; i++) {
		osc_bank[i] = new_oscil(outprops.srate);
		if (osc_bank[i] == NULL) {
			puts("error creating oscillator\n");
			error++;
			goto exit;
		}
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

	printf("processing....\n");

	double freq_fac = 1.0;
	double amp_scale = 0.0;
	double amp_adjust = 0.0;
	switch (wavetype) {
	case (WAVE_SQUARE):
		for (int i = 0; i < osc_number; i++) {
			amp_scale = 1.0 / freq_fac;
			osc_amps[i] = amp_scale;
			osc_freqs[i] = freq_fac;
			freq_fac += 2.0;
			amp_adjust += amp_scale;
		}
		break;
	case (WAVE_TRIANGLE):
		for (int i = 0; i < osc_number; i++) {
			amp_scale = 1.0 / (freq_fac * freq_fac);
			osc_amps[i] = amp_scale;
			osc_freqs[i] = freq_fac;
			freq_fac += 2.0;
			amp_adjust += amp_scale;
		}
		break;
	case (WAVE_SAWUP):
	case (WAVE_SAWDOWN):
		for (int i = 0; i < osc_number; i++) {
			amp_scale = 1.0 / freq_fac;
			osc_amps[i] = amp_scale;
			osc_freqs[i] = freq_fac;
			freq_fac += 1.0;
			amp_adjust += amp_scale;
		}
		if (wavetype == WAVE_SAWUP) {
			amp_adjust = -amp_adjust; // iverting waveform;
		}
		break;
	}

	for (int i = 0; i < osc_number; i++) {
		osc_amps[i] /= amp_adjust;
	}

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
			double val = 0.0;
			for (int k = 0; k < osc_number; k++) {
				val += osc_amps[k] * sinetick(osc_bank[k], freq * osc_freqs[k]);
			}
			outframe[j] = (float) (amp * val);
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
	if (ampstream)
		free(ampstream);
	if (fpamp)
		fclose(fpamp);

	psf_finish();
	return error;
}
