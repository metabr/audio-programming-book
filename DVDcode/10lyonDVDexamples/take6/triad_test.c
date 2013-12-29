#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* define the number of octaves and then the
total number of notes, which is OCTAVES * 7, 
because there are 7 diatonic notes per octave. 
*/
#define OCTAVES (5)
#define NOTECOUNT (OCTAVES * 7)

void set_diatonic(float *freqs, int octaves);

main(int argc, char **argv)
{
	float diatonic_frequencies[NOTECOUNT];
	set_diatonic(diatonic_frequencies, OCTAVES);
}

void set_diatonic(float *freqs, int octaves)
{
	int i;
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
	
	/* TRANSPOSE SCALE TO 65.4064 Hz (2 octaves below middle-C) */
	
	for(i = 0; i < 7; i++){
		freqs[i] *= 65.4064;
	}
	
	/* NOW PRINT IT OUT, JUST FOR TESTING PURPOSES */
	
	for(i = 0; i < 7; i++){
		printf("%f\n", freqs[i]);
	}
	
}
