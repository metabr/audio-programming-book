#include <stdio.h>

/* we include "time.h" for the time() function */

#include <time.h>

char *random_waveform(void);
void chord(float *freqs, int members, int *vcount, float amplitude);
void major_triad(float basefreq, int *vcount, float amplitude);

main(int argc, char **argv)
{
	int vname_count = 1;
	srandom(time(0));
	
	major_triad(440.0, &vname_count, 2000.0);
}

/* randomly select a wavefom */

char *random_waveform(void)
{
	int rval;
	
	rval = random() % 5;
	
	/* rval must now be between 0 and 4 inclusive */
	
	switch( rval ){
		case 0: return "SINE";
		case 1: return "TRIANGLE";
		case 2: return "SQUARE";
		case 3: return "SAWTOOTH";
		case 4: return "PULSE";
	}
	
	/* We used the switch/case idiom to express the waveform more concisely than was shown in
	the text. Ordinarily we would follow each case statement with a "break;" however since the 
	outcome of each option is to return to the main program, breaks are not required here. */
}

void chord(float *freqs, int members, int *vcount, float amplitude)
{
	int i;
	
	for(i = 0; i < members; i++){
		printf ("OSC ga%d %.4f %s NONE NONE -1 1\n", (*vcount)++, freqs[i], random_waveform());
	}
	printf ("MIXOUT ");
	
	/* now that we know how many members there are in this chord, we crank out the
		appropriate number of Csound a-variables, and increment our vcount. */
	
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

