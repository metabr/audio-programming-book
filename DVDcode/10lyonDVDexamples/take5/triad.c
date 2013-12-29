#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define OCTAVES (5)
#define NOTECOUNT (OCTAVES * 7)


char *random_waveform(void);
void chord(float *freqs, int members, int *vcount, float amplitude);
void major_triad(float basefreq, int *vcount, float amplitude);
void minor_triad(float basefreq, int *vcount, float amplitude);
float boundrand(float min, float max);
void set_diatonic( float *freqs, int octaves );

main(int argc, char **argv)
{
	int vname_count = 1;
	int i;
	float rand_freq;
	float diatonic_frequencies[NOTECOUNT];
	int f_index;
	
	srandom(time(0));
	
	set_diatonic(diatonic_frequencies, OCTAVES);
	
	printf("DURATION 30\n");
	
	for(i = 0; i < 4; i++) {
		
		/* Randomly select a base note for the chord */
		
		f_index = random() % NOTECOUNT;
		
		/* Now look up its frequency */
		rand_freq = diatonic_frequencies[f_index];
		
		/* Now randomly select either a major or a minor triad */
		if( boundrand(0.0, 1.0) > 0.5 ) {
			major_triad(rand_freq, &vname_count, 3000.0);
		} else {
			minor_triad(rand_freq, &vname_count, 3000.0);
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


float boundrand(float min, float max)
{
	return min + (max-min) * ((float) (random())/(float)RAND_MAX);
}

void set_diatonic(float *freqs, int octaves)
{
	int i, j;
	float freq = 1.0;
	
	freqs[0] = freq;
	
	/* Run through the cycle of fifths */
	
	for(i = 1; i < 7; i++){
		freqs[i] = freqs[i - 1] * 1.5;
	}
	/* Bring the values into the range 0.0 - 1.0 */
	
	for(i = 1; i < 7; i++){
		while( freqs[i] > 2.0){
			freqs[i] /= 2.0;
		}
	}
	
	/* Transpose the scale to 65.4064 Hz (2 octaves below middle-C) */
	
	for(i = 0; i < 7; i++){
		freqs[i] *= 65.4064;
	}
	
	/* GENERATE THE REMAINING OCTAVES */
	
	for(j = 1; j < octaves; j++){
		for(i = 0; i < 7; i++ ){
			freqs[i + j * 7] = freqs[i + (j - 1) * 7] * 2;
		}
	}
}



