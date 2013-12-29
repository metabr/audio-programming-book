#include <stdio.h>
#include <time.h>

/* we need stdlib.h to get RAND_MAX */

#include <stdlib.h>

char *random_waveform(void);
void chord(float *freqs, int members, int *vcount, float amplitude);
void major_triad(float basefreq, int *vcount, float amplitude);
void minor_triad(float basefreq, int *vcount, float amplitude);
float boundrand(float min, float max);

main(int argc, char **argv)
{
	int vname_count = 1;
	int i;
	float rand_freq;
	
	/* seed the pseudo-random number generator with the time of day */
	
	srandom(time(0));
	
	/* set the duration to 30 seconds*/
	
	printf("DURATION 30\n");
	
	for(i = 0; i < 4; i++) {
		rand_freq = boundrand(100.0, 800.0);
		if( boundrand(0.0, 1.0) > 0.5) {
			major_triad( rand_freq, &vname_count, 2000.0);
		} else {
			minor_triad( rand_freq, &vname_count, 2000.0);
		}
	}
}

char *random_waveform(void)
{
	int rval;
	
	rval = random() % 5;
	
	switch( rval ){
		case 0: return "SINE";
		case 1: return "TRIANGLE";
		case 2: return "SQUARE";
		case 3: return "SAWTOOTH";
		case 4: return "PULSE";
	}
}

void chord(float *freqs, int members, int *vcount, float amplitude)
{
	int i;
	
	for(i = 0; i < members; i++){
		printf ("OSC ga%d %.4f %s NONE NONE -1 1\n", (*vcount)++, freqs[i], random_waveform());
	}
	printf ("MIXOUT ");
	
	for(i = 0; i < members-1; i++){
		printf ("ga%d+", *vcount - (members-i));
	}
	printf ("ga%d %f\n", *vcount - 1, amplitude);
}

void major_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.25;
	freqs[2] = basefreq * 1.5;
	chord(freqs, 3, vcount, amplitude);
}

void minor_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.2;
	freqs[2] = basefreq * 1.5;
	chord(freqs, 3, vcount, amplitude);
}

/* returns a float between min and max*/
float boundrand(float min, float max)
{
	return min + (max-min) * ((float) (random())/(float)RAND_MAX);
}



