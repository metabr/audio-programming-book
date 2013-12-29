#include <stdio.h>
#include <time.h>
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
	
	srandom(time(0));
	
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

void amchord(float *freqs, int members, int *vcount, float amplitude)
{
  int i;
  float fdev;

/* Now we are creating an extra oscillator that we will use for LFO */

  for(i = 0; i < members; i++){
    printf ("OSC ga%d %f SINE NONE NONE -0.3 0.3\n", (*vcount)++, boundrand(0.25, 3.0));
    printf ("OSC ga%d %.4f %s 0.7+ga%d NONE -1 1\n", *vcount, freqs[i], random_waveform(), *vcount - 1);
    printf ("MIXOUT ga%d %f\n", *vcount, amplitude * boundrand(0.2,1.0));
    ++(*vcount);
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

/* modified to use amchord() */

void major_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.25;
	freqs[2] = basefreq * 1.5;
	amchord(freqs, 3, vcount, amplitude);
}

/* modified to use amchord() */

void minor_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.2;
	freqs[2] = basefreq * 1.5;
	amchord(freqs, 3, vcount, amplitude);
}

/* returns a float between min and max*/
float boundrand(float min, float max)
{
	return min + (max-min) * ((float) (random())/(float)RAND_MAX);
}



