#include <stdio.h>
#include <stdlib.h>
#include "snd_def.h"

int main(int argc, char **argv)
{
	SNDFILE *psf;
	float *buffer;
	int smps, cnt1=0, cnt2=0;
	float dur, amp, freq, *wave, ndx=0;

	if (argc == 5) {
		amp = (float)atof(argv[2]);
		freq = (float)atof(argv[3]);
		dur = (float)atof(argv[4]);
		smps = (int)(dur * default_crate);

		buffer = new float[default_vsize];
		wave = saw_table(30);

		if (!(psf = soundout_open(argv[1]))) {
			printf("error opening output file\n");
			exit(1);
		}

		for (int i = 0; i < smps; i++) {
			oscc(buffer,
				amp * adsr(1.0, dur, 0.05, 0.1, 0.7, 0.2, &cnt1),
				exp_env(freq, dur / 2, freq * 2, &cnt2),
				wave,
				&ndx);
			soundout(psf, buffer);
		}

		soundout_close(psf);
		delete[] buffer;
		delete[] wave;

		return 0;
	} else {
		printf("usage: envelope sndfile.wav amp freq dur\n");
		return 1;
	}
}
