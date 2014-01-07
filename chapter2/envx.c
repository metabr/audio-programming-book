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

/* envx.c : 
 * extract amplitude envelope from mono soundfile */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

#define DEFAULT_WINDOW_MSECS (15)

#define max(x, y) ((x) > (y) ? (x) : (y))

enum { ARG_PROGNAME, ARG_INFILE, ARG_OUTFILE, ARG_NARGS };

double maxsamp(float* buf, unsigned long blocksize)
{
        double absval, peak = 0.0;
        unsigned long i;

        for (i = 0; i < blocksize; i++) {
                absval = fabs(buf[i]);
                peak = max(absval, peak);
        }

        return peak;
}

int main(int argc, char *argv[])
{
/* STAGE 1 */
	PSF_PROPS inprops;
	long framesread;
	/* init all dynamic resources to default states */
	int infile = -1, outfile = -1;
	int error = 0;
	PSF_CHPEAK *peaks = NULL;
	psf_format outformat = PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float *inframe = NULL;
	FILE* fp = NULL;
	double windur = DEFAULT_WINDOW_MSECS;

/* STAGE 2 */
	printf("ENVX: extract amplitude envelope from mono soundfile\n");

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
			case ('w'):
				windur = atof(&argv[1][2]);
				if (windur < 0.0) {
					printf("Bad value for Window Duration."
					       "Must be positive.\n");
					return 1;
				}
				break;
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
		       "usage: envx [-wN] insndfile outfile.brk\n"
		       "\t-wN: set extraction windows size to N msecs (default: 15)\n");
		return 1;
	}
	/*  always startup portsf */
	if (psf_init()) {
		printf("unable to start portsf\n");
		return 1;
	}
/* STAGE 3 */
	infile = psf_sndOpen(argv[ARG_INFILE], &inprops, 0);
	if (infile < 0) {
		printf("Error: unable to open infile %s\n",
		       argv[ARG_INFILE]);
		error++;
		goto exit;
	}

	if (inprops.chans > 1) {
		printf("Error: infile must be mono\n");
		error++;
		goto exit;
	}

	windur /= 1000; 
	unsigned long winsize = (unsigned long)(windur * inprops.srate);

	inframe = (float *) malloc(winsize * inprops.chans * sizeof(float));
	if (inframe == NULL) {
		puts("No memory!\n");
		error++;
		goto exit;
	}
/* STAGE 4 */
	/* create output breakpoint file */
	fp = fopen(argv[ARG_OUTFILE], "w");
	if (fp == NULL) {
		printf("Error: unable to create output file %s\n",
		       argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}

/* STAGE 5 */
	printf("processing....\n");

	double brktime = 0.0;
	unsigned long npoints = 0;
	while ((framesread = psf_sndReadFloatFrames(infile, inframe, winsize)) > 0) {
		double amp;
		amp = maxsamp(inframe, framesread);

		if (fprintf(fp, "%f\t%f\n", brktime, amp) < 2) {
			printf("Failed to write to breakpoint file %s\n", argv[ARG_OUTFILE]);
			error++;
			break;
		}
		brktime += windur;
		npoints++;
	}

	if (framesread < 0) {
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	} else
		printf("Done: %d errors\n", error);
/* STAGE 6 */
	printf("%ld breakpoints written to %s\n", npoints, argv[ARG_OUTFILE]);
/* STAGE 7 */
	/* do all cleanup  */
      exit:
	if (infile >= 0)
		if (psf_sndClose(infile))
			printf("%s: Warning: error closing infile %s\n",
			       argv[ARG_PROGNAME], argv[ARG_INFILE]);
	if (inframe)
		free(inframe);
	if (fp)
		if (fclose(fp))
			printf("%s: Warning: error closing ourfile %s\n",
			       argv[ARG_PROGNAME], argv[ARG_OUTFILE]);

	psf_finish();
	return error;
}
