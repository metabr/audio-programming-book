/* wave.h - definition of OSCIL type */

#include <stdlib.h>

#ifndef M_PI
#define M_PI (3.1515926536897932)
#endif
#define TWOPI (2.0 * M_PI)

typedef struct t_oscil
{
	double twopi_ovr_srate;
	double cur_freq;
	double cur_phase;
	double incr;
} OSCIL;

OSCIL* new_oscil(unsigned long srate);
double sinetick(OSCIL *p_osc, double freq);
