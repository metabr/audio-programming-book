#include <stdio.h>
#include <stdlib.h>

/* FUNCTION DECLARATIONS */

char *random_waveform(void);
float boundrand(float min, float max);
void sahphase(int *vcount, char *outvar);

/* MAIN PROGRAM */

main(int argc, char **argv)
{
  char synvar[128];
  int varcount = 1;
  float gain = 15000.0;

  srandom(time(0));
  printf("DURATION 30\n");
  sahphase(&varcount, synvar);
  printf("MIXOUT %s %f\n", synvar, gain );
}

/* FUNCTIONS */

float boundrand(float min, float max)
{
  return min + (max-min) * ((float) (random())/(float)RAND_MAX);
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

void sahphase(int *vcount, char *outvar)
{
  /* STORAGE FOR THE VARIABLE NAMES. */

  char v1[8];
  char v2[8];
  char v3[8];
  char v4[8];
  char v5[8];
  char v6[8];

  /* VARIABLES TO HOLD RANDOM VALUES */

  float seed;
  float minf, maxf;
  float speed1, speed2, speed3;

  /* PARAMETERS ARE GENERATED HERE. NOTE THAT speed2 AND speed3 ARE GENERATED AS VARIANTS OF speed1. */

  seed = boundrand(0.0, 1.0);
  minf = boundrand(50.0, 250.0);
  maxf = boundrand(400.0, 1500.);
  speed1 = boundrand(1.0, 13.0);
  speed2 = speed1 * boundrand(0.9, 1.1);
  speed3 = speed1 * boundrand(0.9, 1.1);

  /* VARIABLE NAMES ARE GENERATED AND STORED */

  sprintf(v1, "ga%d", (*vcount)++);
  sprintf(v2, "ga%d", (*vcount)++);
  sprintf(v3, "ga%d", (*vcount)++);
  sprintf(v4, "ga%d", (*vcount)++);
  sprintf(v5, "ga%d", (*vcount)++);
  sprintf(v6, "ga%d", (*vcount)++);

  /* THE PATCH SEGMENT IS PRINTED */

  printf("SAH %s %f %f %f %f\n",v1, speed1, seed, minf, maxf );
  printf("SAH %s %f %f %f %f\n",v2, speed2, seed, minf, maxf );
  printf("SAH %s %f %f %f %f\n",v3, speed3, seed, minf, maxf );
  printf("OSC %s %s %s NONE NONE -1 1\n", v4, v1, random_waveform());
  printf("OSC %s %s %s NONE NONE -1 1\n", v5, v2, random_waveform());
  printf("OSC %s %s %s NONE NONE -1 1\n", v6, v3, random_waveform());

  /* FINALLY THE SIGNAL OUTPUT SUM IS STORED IN outvar. NOTICE THAT WE MULTIPLY AMPLITUDE BY 1/3 TO COMPENSATE FOR ADDING THREE SIGNALS TOGETHER */

  sprintf(outvar, "(%s+%s+%s)*0.333", v4,v5,v6);
}
