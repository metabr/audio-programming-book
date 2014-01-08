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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <breakpoints.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

/* TODO define program argument list, excluding flags */
enum { ARG_PROGNAME, ARG_INFILE, ARG_OUTFILE, ARG_BRKFILE, ARG_NARGS };

int main(int argc, char *argv[])
{
/* STAGE 1 */
	PSF_PROPS inprops, outprops;
	long framesread;
	double height, width;
	BREAKPOINT leftpoint, rightpoint;
	/* init all dynamic resources to default states */
	int infile = -1, outfile = -1;
	int error = 0;
	PSF_CHPEAK *peaks = NULL;
	psf_format outformat = PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float *inframe = NULL;
	FILE* fp = NULL;
	unsigned long npoints;
	BREAKPOINT* points = NULL;
	/* TODO: define an output frame buffer if channel width different */

/* STAGE 2 */
	printf("SFENP: apply envelope to mono file\n");

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
		       "usage:\n"
		       "\tsfenv infile outfile envfile.brk\n");
		return 1;
	}
	/*  always startup portsf */
	if (psf_init()) {
		printf("unable to start portsf\n");
		return 1;
	}

	/* read and verify breakpoint file */
	fp = fopen(argv[ARG_BRKFILE], "r");
	if (fp == NULL) {
		printf("Error: unable to open breakpoint file %s\n",
		       argv[ARG_BRKFILE]);
		error++;
		goto exit;
	}

	points = get_breakpoints(fp, &npoints);
	if (points == NULL) {
		printf("No breakpoints read.\n");
		error++;
		goto exit;
	}

	if (npoints < 2) {
		printf("Error: at least two breakpoints required.\n");
		free(points);
		fclose(fp);
		return 1;
	}

	/* we require breakpoints to start at 0 */
	if (points[0].time != 0.0) {
		printf("Error in breakpoint data:"
		       "first time must be 0.0\n");
		error++;
		goto exit;
	}

	if (!inrange(points, 0, 1, npoints)) {
		printf("Error in breakpoint data:"
		       "values out of range -1 to +1\n");
		error++;
		goto exit;
	}

/* STAGE 3 */
	infile = psf_sndOpen(argv[ARG_INFILE], &inprops, 0);
	if (infile < 0) {
		printf("Error: unable to open infile %s\n",
		       argv[ARG_INFILE]);
		error++;
		goto exit;
	}

	if (inprops.chans != 1) {
		printf("Error: infile must be mono.\n");
		error++;
		goto exit;
	}

	/* allocate space for  sample frame buffer ... */
	inframe = (float *) malloc(nframes * inprops.chans * sizeof(float));
	if (inframe == NULL) {
		puts("No memory!\n");
		error++;
		goto exit;
	}
	/* check file extension of outfile name, so we use correct output file format */
	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if (outformat == PSF_FMT_UNKNOWN) {
		printf("outfile name %s has unknown format.\n"
		       "Use any of .wav, .aiff, .aif, .afc, .aifc\n",
		       argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	inprops.format = outformat;
	outprops = inprops;

/* STAGE 4 */
	peaks = (PSF_CHPEAK *) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if (peaks == NULL) {
		puts("No memory!\n");
		error++;
		goto exit;
	}

	outfile = psf_sndCreate(argv[ARG_OUTFILE], &outprops, 0, 0,
			  PSF_CREATE_RDWR);
	if (outfile < 0) {
		printf("Error: unable to create outfile %s\n",
		       argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
/* STAGE 5 */
	printf("processing....\n");
	/* TODO: init any loop-related variables */

	double incr = 1.0 / inprops.srate;
	double curpos = 0.0;
	unsigned long ileft = 0;
	unsigned long iright = 1;
	leftpoint = points[ileft];
	rightpoint = points[iright];
	
	width = rightpoint.time - leftpoint.time;
	height = rightpoint.value - leftpoint.value;

	int more_points = 1;

	while ((framesread = psf_sndReadFloatFrames(infile, inframe, nframes)) > 0) {
		unsigned long i;
		double frac, thisamp;

		for (i = 0; i < framesread; i++) {
			if (more_points) {
				if (width == 0.0) {
					thisamp = rightpoint.value;
				} else {
					frac = (curpos - leftpoint.time) / width;
					thisamp = leftpoint.value + (height * frac);
				}
				curpos += incr;

				if (curpos > rightpoint.time) {
					ileft++;
					iright++;

					if (iright < npoints) {
						leftpoint = points[ileft];
						rightpoint = points[iright];
						width = rightpoint.time - leftpoint.time;
						height = rightpoint.value - leftpoint.value;
					} else {
						more_points = 0;
					}
				}
			}
			inframe[i] = (float) (inframe[i] * thisamp);
		}	

		if (psf_sndWriteFloatFrames(outfile, inframe, framesread)
				!= framesread) {
			printf("Error writing to outfile\n");
			error++;
			break;
		}
	}

	if (framesread < 0) {
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	} else
		printf("Done: %d errors\n", error);
/* STAGE 6 */
	/* report PEAK values to user */
	if (psf_sndReadPeaks(outfile, peaks, NULL) > 0) {
		long i;
		double peaktime;
		printf("PEAK information:\n");
		for (i = 0; i < inprops.chans; i++) {
			peaktime =
			    (double) peaks[i].pos / (double) inprops.srate;
			printf("CH %ld:\t%.4f at %.4f secs\n", i + 1,
			       peaks[i].val, peaktime);
		}
	}
/* STAGE 7 */
	/* do all cleanup  */
      exit:
	if (infile >= 0)
		if (psf_sndClose(infile))
			printf("%s: Warning: error closing infile %s\n",
			       argv[ARG_PROGNAME], argv[ARG_INFILE]);
	if (outfile >= 0)
		if (psf_sndClose(outfile))
			printf("%s: Warning: error closing outfile %s\n",
			       argv[ARG_PROGNAME], argv[ARG_OUTFILE]);
	if (inframe)
		free(inframe);
	if (peaks)
		free(peaks);
	/*TODO: cleanup any other resources */

	psf_finish();
	return error;
}
