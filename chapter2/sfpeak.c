#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

enum {ARG_PROGNAME, ARG_INFILE, ARG_NARGS};

double maxsamp(float* buf, unsigned long blocksize)
{
	double absval, peak = 0.0;
	unsigned long i;

	for (i = 0; i < blocksize; i++) {
		absval = fabs(buf[i]);
		if (absval > peak) {
			peak = absval;
		}
	}

	return peak;
}

int main(int argc, char** argv)
{
	PSF_PROPS props;
	long framesread, totalread, blocksize;

	// init all resource vars to default states
	int ifd = -1;
	int error = 0;
	double dbval, inpeak = 0.0;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;
	
	const unsigned long nframes = 1024;

	printf("%s: prints peak value for a file\n", argv[ARG_PROGNAME]);

	if (argc < ARG_NARGS) {
		printf("insufficient arguments.\n"
			"usage:\n\t%s infile\n",
			argv[ARG_PROGNAME]);
		return 1;
	}
	
	// startup portsf
	if (psf_init()) {
		printf("unable to start portsf\n");
		return 1;
	}

	ifd = psf_sndOpen(argv[ARG_INFILE], &props, 0);
	if (ifd < 0) {
		printf("Error: unable to open infile %s\n",
				argv[ARG_INFILE]);
		return 1;
	}

	frame = (float *) malloc(nframes * props.chans * sizeof(float));
	if (frame == NULL) {
		printf("Error: no memory!\n");
		error++;
		goto exit;
	}

	// allocate space for PEAK info
	peaks = (PSF_CHPEAK*) malloc(props.chans * sizeof(PSF_CHPEAK));
	if (peaks == NULL) {
		printf("Error: no memory!\n");
		error++;
		goto exit;
	}

	// get peak info: scan file if required
	if(psf_sndReadPeaks(ifd, peaks, NULL) > 0) {
		for (long i = 0; i < props.chans; i++) {
			if (peaks[i].val > inpeak) {
				inpeak = peaks[i].val;
			}
		}
	}
	else {
		framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
		while (framesread > 0) {
			double thispeak;
			blocksize = framesread * props.chans;
			thispeak = maxsamp(frame, blocksize);
			if (thispeak > inpeak) {
				inpeak = thispeak;
			}
			framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
		}
	}

	if (framesread < 0) {
		printf("Error reading infile.\n");
		error++;
		goto exit;
	}
	
	if (inpeak == 0.0) {
		printf("Infile is silent!\n");
		error++;
		goto exit;
	}

	printf("Peak for %s is %f ", argv[ARG_INFILE], inpeak);
	printf("or %f dB\n", 20.0 * log10(inpeak));

// do all cleanup
exit:
	if (ifd >= 0)
		psf_sndClose(ifd);
	if (frame)
		free(frame);
	if (peaks)
		free(peaks);
	psf_finish();

	return error;
}
