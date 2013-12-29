#include <stdio.h>
#include <stdlib.h>

/* FUNCTION DECLARATIONS */

char *random_waveform(void);
float boundrand(float min, float max);
void sahphase(int *vcount, char *outvar);
void fmfeedback(int *vcount, char *outvar);
void fmnote(float freq, float dev, int *vcount, char *outvar);
void fmtrio(int *vcount, char *outvar);
void ringmod(int *vcount, char *invar, char *outvar);

/* MAIN PROGRAM */

main(int argc, char **argv)
{
  char synvar[128];
  char procvar1[128];           /* FOR PROCESSOR OUTPUT 1 */
  char procvar2[128];           /* FOR PROCESSOR OUTPUT 2 */
  int varcount = 1;
  float gain = 15000.0;
  float rval;

  srandom(time(0));
  printf("DURATION 30\n");

  rval = boundrand(0.0, 1.0);
  if(rval < 0.333){
    sahphase(&varcount, synvar);
  }
  else if(rval < 0.666) {
    fmfeedback(&varcount, synvar);
  } else {
    fmtrio(&varcount, synvar);
  }

  ringmod(&varcount, synvar, procvar1);
  printf("MIXOUT %s %f\n", procvar1, gain);
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

void fmfeedback(int *vcount, char *outvar)
{
  float freq1, freq2, freq3, freq4;
  char v1[8];
  char v2[8];
  char v3[8];
  char v4[8];

  sprintf(v1, "ga%d", (*vcount)++);
  sprintf(v2, "ga%d", (*vcount)++);
  sprintf(v3, "ga%d", (*vcount)++);
  sprintf(v4, "ga%d", (*vcount)++);

  /* THESE PARAMETER BOUNDARIES ARE SOMEWHAT ARBITRARY BUT BASED ON THE MODEL PATCH */

  freq1 = boundrand(2.0, 600.0);
  freq2 = boundrand(0.5, 17.0);
  freq3 = boundrand(3.0, 11.0);
  freq4 = boundrand(30.0, 1200.0);

  printf ("OSC %s %f SINE NONE %s -1 1\n", v1, freq1, v4);
  printf ("OSC %s %f SINE NONE %s -1 1\n", v2, freq2, v1);
  printf ("OSC %s %f SINE NONE %s -1 1\n", v3, freq3, v2);
  printf ("OSC %s %f SINE NONE %s -1 1\n", v4, freq4, v3);
  sprintf(outvar, "%s", v1);
}

void fmnote(float freq, float dev, int *vcount, char *outvar)
{
  char v1[8];
  char v2[8];
  char v3[8];
  float freqdev;
  float hi_index, lo_index;
  float randspeed;
  float seed;

  sprintf(v1, "ga%d", (*vcount)++);
  sprintf(v2, "ga%d", (*vcount)++);
  sprintf(v3, "ga%d", (*vcount)++);

  hi_index = boundrand(12.0, 30.0);
  lo_index = boundrand(2.0, hi_index);
  freqdev = freq * boundrand(1.0-dev, 1.0+dev);
  randspeed = boundrand(0.05, 0.4);
  seed = boundrand(0.0, 1.0);

  /* INDEX */
  printf ("NOISE %s %f %f %f %f\n", v1, randspeed, seed, lo_index , hi_index);

  /* MODULATOR */
  printf ("OSC %s %f %s %s NONE -1 1\n", v2, freqdev, random_waveform(), v1);

  /* CARRIER */
  printf ("OSC %s %f %s NONE %s -1 1\n", v3, freq, random_waveform(), v2 );
  sprintf(outvar, "%s", v3);
}

/* Stack three FM notes */
void fmtrio(int *vcount, char *outvar)
{
  char fm1[16];
  char fm2[16];
  char fm3[16];
  float freq1, freq2, freq3;
  float dev;

  freq1 = boundrand(30., 800.);
  freq2 = boundrand(30., 800.);
  freq3 = boundrand(30., 800.);
  dev = boundrand(0.01, 0.8);

  fmnote(freq1, dev, vcount, fm1);
  fmnote(freq2, dev, vcount, fm2);
  fmnote(freq2, dev, vcount, fm3);

  sprintf(outvar, "(%s+%s+%s)*0.333", fm1, fm2, fm3);
}

void ringmod(int *vcount, char *invar, char *outvar)
{
  char v1[8];
  float rmodfreq;

  /* THIS VARIABLE WILL HOLD THE RINGMOD OSCILLATOR */
  sprintf(v1, "ga%d", (*vcount)++);

  /* THIS VARIABLE WILL HOLD THE RINGMOD-PROCESSED SIGNAL */
  sprintf(outvar, "ga%d", (*vcount)++);

  rmodfreq = boundrand(5.0, 1100.0);
  printf("OSC %s %f SINE NONE NONE -1 1\n", v1, rmodfreq);
  printf("RINGMOD %s %s %s\n", outvar, invar, v1);
}

