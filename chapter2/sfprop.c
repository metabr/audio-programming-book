#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

enum {ARG_PROGNAME, ARG_FILE, ARG_NARGS};

int main(int argc, char** argv)
{
	int error = 0;

	PSF_PROPS props;
	PSF_CHPEAK* peaks = NULL;
	int fd = -1;

	if (argc < ARG_NARGS) {
		printf("insufficient arguments.\n"
				"usage:\n"
				"\t%s file\n",
				argv[ARG_PROGNAME]);
		return 1;
	}

	if (psf_init()) {
		printf("unable to start portsf\n");
		return 1;
	}

	fd = psf_sndOpen(argv[ARG_FILE], &props, 0);
	if (fd < 0) {
		printf("Error: unable to open file %s\n",
				argv[ARG_FILE]);
		return 1;
	}

	printf("Sample rate: %d\n", props.srate);
	printf("Number of channels: %d\n", props.chans);

	printf("Sample type: ");
	switch (props.samptype) {
	case (PSF_SAMP_8):
		printf("8 bit\n");
		break;
	case (PSF_SAMP_16):
		printf("16 bit\n");
		break;
	case (PSF_SAMP_24):
		printf("24 bit\n");
		break;
	case (PSF_SAMP_32):
		printf("32 bit\n");
		break;
	case (PSF_SAMP_IEEE_FLOAT):
		printf("32 bit floating point\n");
		break;
	default:
		printf("unknown\n");
	}

	printf("Format: ");
	switch (props.format) {
	case (PSF_STDWAVE):
		printf("STDWAVE\n");
		break;
	case (PSF_WAVE_EX):
		printf("WAVE_EX\n");
		break;
	case (PSF_AIFF):
		printf("AIFF\n");
		break;
	case (PSF_AIFC):
		printf("AIFC\n");
		break;
	default:
		printf("unknown\n");
	}

	printf("Channel format: ");
	switch (props.chformat) {
	case (STDWAVE):
		printf("STDWAVE\n");
		break;
	case (MC_STD):
		printf("MC_STD\n");
		break;
	case (MC_MONO):
		printf("MC_MONO\n");
		break;
	case (MC_STEREO):
		printf("MC_STEREO\n");
		break;
	case (MC_QUAD):
		printf("MC_QUAD\n");
		break;
	case (MC_LCRS):
		printf("MC_LCRS\n");
		break;
	case (MC_BFMT):
		printf("MC_BFMT\n");
		break;
	case (MC_DOLBY_5_1):
		printf("MC_DOLBY_5_1\n");
		break;
	case (MC_SURR_5_0):
		printf("MC_SURR_5_0\n");
		break;
	case (MC_SURR_7_1):
		printf("MC_SURR_7_1\n");
		break;
	case (MC_WAVE_EX):
		printf("MC_WAVE_EX\n");
		break;
	default:
		printf("unknown\n");
	}

	peaks = (PSF_CHPEAK*) malloc(props.chans * sizeof(PSF_CHPEAK));
	if (peaks == NULL) {
		printf("Error allocating memory for peak data!\n");
		error++;
		goto exit;
	}

	if(psf_sndReadPeaks(fd, peaks, NULL) > 0) {
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

exit:
	if (fd > 0)
		psf_sndClose(fd);
	psf_finish();

	return error;
}
