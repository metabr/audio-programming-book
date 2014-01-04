#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

enum {ARG_PROGNAME, ARG_INFILE, ARG_OUTFILE, ARG_DBVAL, ARG_NARGS};

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

int main(int argc, char** argv)
{
	PSF_PROPS props;
	long framesread, totalread, blocksize;

	// init all resource vars to default states
	int ifd = -1;
	int ofd = -1;
	int error = 0;
	double dbval, inpeak = 0.0;
	float ampfac, scalefac;
	psf_format outformat = PSF_FMT_UNKNOWN;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;
	
	const unsigned long nframes = 1024;

	printf("sfnorm: change level of soundfile\n");

	if (argc < ARG_NARGS) {
		printf("insufficient arguments.\n"
			"usage:\n\t%s infile outfile dBval\n"
			"where dBval must be <= 0.0\n"
			"recommend to use values lower than -1.0 dB to avoid clipping\n",
			argv[ARG_PROGNAME]);
		return 1;
	}
	
	dbval = atof(argv[ARG_DBVAL]);
	if (dbval > 0.0) {
		printf("Error: dBval cannot be positive.\n");
		return 1;
	}
	ampfac = (float) pow(10.0, dbval/20.0);

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

	printf("...copying\n");

	// get peak info: scan file if required
	if(psf_sndReadPeaks(ifd, peaks, NULL) > 0) {
		for (long i = 0; i < props.chans; i++) {
			inpeak = max(peaks[i].val, inpeak);
		}
	}
	else {
		framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
		while (framesread > 0) {
			double thispeak;
			blocksize = framesread * props.chans;
			thispeak = maxsamp(frame, blocksize);
			inpeak = max(thispeak, inpeak);
			framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
		}

		if (psf_sndSeek(ifd, 0, PSF_SEEK_SET) < 0) {
			printf("Error: unable to rewind infile.\n");
			error++;
			goto exit;
		}
	}
	
	if (inpeak == 0.0) {
		printf("Infile is silent! Outfile not created.\n");
		goto exit;
	}

	// single-frame loop to do copy, report any errors
	scalefac = (float) (ampfac / inpeak);
	framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
	totalread = 0;
	while (framesread > 0) {
		totalread += framesread;
		blocksize = framesread * props.chans;

		for (unsigned long i = 0; i < blocksize; i++) {
			frame[i] *= scalefac;
		}

		if (psf_sndWriteFloatFrames(ofd, frame, framesread) != framesread) {
			printf("Error writing to outfile\n");
			error++;
			break;
		}

		framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
	}

	if (framesread < 0) {
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else {
		printf("Done %ld sample frames copied to %s\n",
			totalread,
			argv[ARG_OUTFILE]);
	}

	// report PEAK values
	if(psf_sndReadPeaks(ofd, peaks, NULL) > 0) {
		long i;
		double peaktime;
		printf("PEAK information:\n");
		for (i = 0; i < props.chans; i++) {
			peaktime = (double) peaks[i].pos / props.srate;
			printf("CH: %ld:\t%.4f at %.4f secs\n",
				i + 1, peaks[i].val, peaktime);
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
