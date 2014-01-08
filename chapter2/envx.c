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
#include <breakpoints.h>

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
	int infile = -1;
	int error = 0;
	float *inframe = NULL;
	FILE* fp = NULL;
	double windur = DEFAULT_WINDOW_MSECS;
	BREAKPOINT *breakpoints = NULL;
	int normalize = 0;
	unsigned long npoints, winsize;

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
			case ('n'):
				normalize = 1;
				break;
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
		       "\t-n: enable output normalization"
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

	int infile_size = psf_sndSize(infile);
	if (infile_size < 0) {
		printf("Error: cannot get infile size\n");
		error++;
		goto exit;
	}

	windur /= 1000; 
	winsize = (unsigned long) (windur * inprops.srate);
	npoints = (unsigned long) ((infile_size / winsize) + 1);

	inframe = malloc(npoints * sizeof(*inframe));
	if (inframe == NULL) {
		puts("No memory!\n");
		error++;
		goto exit;
	}

	breakpoints = malloc(npoints * sizeof(*breakpoints));
	if (breakpoints == NULL) {
		puts("No memory!\n");
		error++;
		goto exit;
	}
	printf("%ld breakpoints allocated\n", npoints);

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
	npoints = 0;
	while ((framesread = psf_sndReadFloatFrames(infile, inframe, winsize)) > 0) {
		double amp;
		amp = maxsamp(inframe, framesread);

		if (normalize) {
			breakpoints[npoints].time = brktime;
			breakpoints[npoints].value = amp;
		} else {
			if (fprintf(fp, "%f\t%f\n", brktime, amp) < 2) {
				printf("Failed to write to breakpoint file %s\n", argv[ARG_OUTFILE]);
				error++;
				break;
			}
		}
		brktime += windur;
		npoints++;
	}

	if (framesread < 0) {
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	} else {
		printf("Done: %d errors\n", error);
	}

	if (normalize) {
		BREAKPOINT max = maxpoint(breakpoints, npoints);
		double normscale = 1.0 / max.value;
		unsigned long i;
		for (i = 0; i < npoints; i++) {
			if (fprintf(fp, "%f\t%f\n", breakpoints[i].time, (breakpoints[i].value * normscale)) < 2) {
				printf("Failed to write to breakpoint file %s\n", argv[ARG_OUTFILE]);
				error++;
				break;
			}
		}
	}

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
	if (breakpoints)
		free(breakpoints);
	if (fp)
		if (fclose(fp))
			printf("%s: Warning: error closing ourfile %s\n",
			       argv[ARG_PROGNAME], argv[ARG_OUTFILE]);

	psf_finish();
	return error;
}
