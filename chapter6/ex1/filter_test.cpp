#include <stdio.h>
#include <stdlib.h>
#include "snd_def.h"

int main(int argc, char **argv)
{
	SNDFILE *psf;
	float *buffer;
	int smps, cnt1=0, cnt2=0;
	int bytes = sizeof(float) * default_vsize;

	float dur, amp, freq, *wave, *comp, ndx=0;

	if (argc == 5) {
		amp = (float)atof(argv[2]);
		freq = (float)atof(argv[3]);
		dur = (float)atof(argv[4]);
		smps = (int)(dur * default_crate);

		buffer = new float[default_vsize];
		comp = new float[default_vsize];
		float del[2] = {0.0, 0.0}, del1[2] = {0.0, 0.0};
		wave = saw_table(30);

		if (!(psf = soundout_open(argv[1]))) {
			printf("error opening output file\n");
			exit(1);
		}

		for (int i = 0; i < smps; i++) {
			oscc(buffer,
				amp * adsr(1.0, dur, 0.05, 0.1, amp * 0.7, 0.2, &cnt1),
				freq,
				wave,
				&ndx);
			mempcpy(comp, buffer, bytes);
			resonator(buffer, exp_env(freq * 8, dur, freq * 4, &cnt2), 50, del);
			balance(buffer, comp, del1);
			soundout(psf, buffer);
		}

		soundout_close(psf);
		delete[] buffer;
		delete[] wave;

		return 0;
	} else {
		printf("usage: filter_test sndfile.wav amp freq dur\n");
		return 1;
	}
}
