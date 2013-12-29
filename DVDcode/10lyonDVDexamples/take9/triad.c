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
void amchord(float *freqs, int members, int *vcount, float amplitude);
void amfmchord(float *freqs, int members, int *vcount, float amplitude);
void vibfmchord(float *freqs, int members,int *vcount,float amplitude);
void set_diatonic(float *freqs, int octaves);

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
		f_index = random() % NOTECOUNT;
		rand_freq = diatonic_frequencies[f_index];
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

/* modified to use vibfmchord() */

void major_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.25;
	freqs[2] = basefreq * 1.5;
	vibfmchord(freqs, 3, vcount, amplitude);
}

/* modified to use vibfmchord() */

void minor_triad(float basefreq, int *vcount, float amplitude)
{
	float freqs[3];
	freqs[0] = basefreq;
	freqs[1] = basefreq * 1.2;
	freqs[2] = basefreq * 1.5;
	vibfmchord(freqs, 3, vcount, amplitude);
}


float boundrand(float min, float max)
{
	return min + (max-min) * ((float) (random())/(float)RAND_MAX);
}

void amfmchord(float *freqs, int members, int *vcount, float amplitude)
{
	int i;
	float fdev;
	float fm_index;
	
	for(i = 0; i < members; i++){
		fm_index = boundrand(0.5, 8.0) ;
		
		printf ("OSC ga%d %f SINE NONE NONE -0.3 0.3\n", (*vcount)++, boundrand(0.15, 0.5)); 
		printf ("OSC ga%d %f SINE NONE NONE -1 1\n", (*vcount)++, boundrand(0.05, 0.3)); 
		printf ("OSC ga%d %f SINE ga%d NONE  %f %f\n", (*vcount)++, freqs[i]  * (1.0 + boundrand(-.01,.01)), *vcount - 2, -fm_index, fm_index); 
		printf ("OSC ga%d %.4f %s 0.7+ga%d ga%d -1 1\n", *vcount, freqs[i], random_waveform(), *vcount - 3, *vcount - 1);
		printf ("MIXOUT ga%d %f\n", *vcount, amplitude * boundrand(0.2,1.0) );
		++(*vcount);
	}
}

void vibfmchord(float *freqs, int members,int *vcount,float amplitude)
{
	int i;
	float fm_index;
	float modified_freq;
	float vibrato_depth;
	float vibrato_speed;
	
	for(i = 0; i < members; i++){
		fm_index = boundrand(0.5, 8.0) ;
		modified_freq = freqs[i]  * (1.0 + boundrand(-.01,.01));
		vibrato_depth = boundrand(0.005, 0.02);
		vibrato_speed = boundrand(0.5, 6.0);
		
		/* AM oscillator */
		
		printf ("OSC ga%d %f SINE NONE NONE -0.3 0.3\n", (*vcount)++, boundrand(0.15, 0.5));
		
		/* FM Index LFO */
		
		printf ("OSC ga%d %f SINE NONE NONE 0 4\n", (*vcount)++, boundrand(0.05, 0.3));
		
		/* LFO oscillator */
		
		printf ("OSC ga%d %f SINE NONE NONE -1 1\n", (*vcount)++, boundrand(0.05, 0.3));
		
		
		/* Modulation oscillator */
		
		printf ("OSC ga%d %f SINE ga%d NONE  %f %f\n", (*vcount)++, modified_freq, *vcount - 2, -fm_index, fm_index);
		
		/* Vibrator oscillator */
		
		printf ("OSC ga%d %f SINE NONE NONE  %f %f\n", (*vcount)++, vibrato_speed,  -vibrato_depth, vibrato_depth);
		
		/* Carrier oscillator */
		
		printf ("OSC ga%d %.4f %s 0.7*ga%d ga%d+ga%d -1 1\n", *vcount, freqs[i], random_waveform(), *vcount - 4, *vcount - 2, *vcount - 1 );
		printf ("MIXOUT ga%d %f\n", *vcount, amplitude * boundrand(0.2,1.0) );
		++(*vcount);
	}
}

void set_diatonic(float *freqs, int octaves)
{
	int i, j;
	float freq = 1.0;
	
	freqs[0] = freq;
	
	
	/* RUN THROUGH THE CYCLE OF FIFTHS */

	for(i = 1; i < 7; i++){
		freqs[i] = freqs[i - 1] * 1.5;
	}

	/* NOW BRING THE VALUES INTO RANGE FROM 0.0 - 1.0 */

	for(i = 1; i < 7; i++){
		while( freqs[i] > 2.0){
			freqs[i] /= 2.0;
		}
	}
	
	/* Now do the upper octaves */
	
	for(j = 1; j < octaves; j++){
		for(i = 0; i < 7; i++){
			freqs[i + j * 7] = freqs[i + (j - 1) * 7] * 2;
		}
	}
	
	/* TRANSPOSE SCALE TO 65.4064 Hz (2 octaves below middle-C) */
	
	for(i = 0; i < 7 * octaves; i++){
		freqs[i] *= 65.4064;
	}		
}


