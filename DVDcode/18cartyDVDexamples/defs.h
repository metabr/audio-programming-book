/* 
defs.h:
Copyright (C) Brian Carty 2009
Binaural sound source movement using magnitude interpolation and phase truncation: header file 
See license.txt for a disclaimer of all warranties and licensing information
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sndfile.h>
#include <fftw3.h>

/* correct length for mono impulse */
#define irlength 128	
/* padded impulse */
#define irpadlength 256
/* convolution overlap */
#define overlapsize 127
/* maxumum points in a trajectory */
#define maxbrkpts 101 
 
void bkpt(int *pers, double *els, double *angs, int *noofpoints, int maxpts);

