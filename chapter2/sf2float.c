#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

enum {ARG_PROGNAME, ARG_INFILE, ARG_OUTFILE, ARG_NARGS};

int main(int argc, char** argv)
{
	PSF_PROPS props;
	long framesread, totalread;
	// init all resource vars to default states
	int ifd = -1, ofd = -1;
	int error = 0;
	psf_format outformat = PSF_FMT_UNKNOWN;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;

	const int buf_size = 512;

	printf("SF2FLOAT: convert soundfile to floats format\n");

	if (argc < ARG_NARGS) {
		printf("insufficient arguments.\n"
			"usage:\n\tsf2float infile outfile\n");
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

	if (props.samptype == PSF_SAMP_IEEE_FLOAT) {
		printf("Info: infile is already in floats format.\n");
	}

	props.samptype = PSF_SAMP_IEEE_FLOAT;

	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if (outformat == PSF_FMT_UNKNOWN) {
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",
			argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	props.format = outformat;

	ofd = psf_sndCreate(argv[ARG_OUTFILE], &props, 0, 0, PSF_CREATE_RDWR);
	if (ofd < 0) {
		printf("Error: unable to create outfile %s\n",
			argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}

	// allocate space for sample frames of buf_size quantity
	frame = (float *) malloc(props.chans * buf_size * sizeof(float));
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
	printf("...copying\n");

	// single-frame loop to do copy, report any errors
	framesread = psf_sndReadFloatFrames(ifd, frame, buf_size);
	totalread = 0;
	printf("Progress: %ld samples copied to %s...",
			totalread, argv[ARG_OUTFILE]);
	while (framesread > 0) {
		totalread += framesread;
		if (psf_sndWriteFloatFrames(ofd, frame, framesread) != framesread) {
			printf("Error writing to outfile\n");
			error++;
			break;
		}
		// --- do any processing here ---
		printf("\rProgress: %ld samples copied to %s...",
				totalread, argv[ARG_OUTFILE]);
		framesread = psf_sndReadFloatFrames(ifd, frame, buf_size);
	}

	if (framesread < 0) {
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else {
		printf("\rDone: %ld sample frames copied to %s\n",
			totalread,
			argv[ARG_OUTFILE]);
	}

	// report PEAK values
	if(psf_sndReadPeaks(ofd, peaks, NULL) > 0) {
		long i;
		double peaktime;
		double peak_db;
		printf("PEAK information:\n");
		for (i = 0; i < props.chans; i++) {
			peaktime = (double) peaks[i].pos / props.srate;
			peak_db = 20.0 * log10(peaks[i].val);
			printf("CH: %ld:\t%.4fdB at %.4f secs\n",
				i + 1, peak_db, peaktime);
		}
	}

// do all cleanup
exit:
	if (ifd >= 0)
		psf_sndClose(ifd);
	if (ofd >= 0)
		psf_sndClose(ofd);
	if (frame)
		free(frame);
	if (peaks)
		free(peaks);
	psf_finish();

	return error;
}
