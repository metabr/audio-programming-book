/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

/* dsptools.h */
/* NB: the same FXDELAY structure supports both time-varying single delay, 
 *  and multi-tapped delay.
 *  Both delay functions implement feedback internally
*/

#ifndef _DSPTOOLS_H_INCLUDED
#define _DSPTOOLS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
	
#endif

#include <stdio.h>

#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif 

/*user struct defining a single delay tap */
typedef struct _delaytap 
{
	double taptime;		/* seconds */
	double amp;			/* should not be negative */
} DELAYTAP;

/* used by FXDELAY to track tap indexes - srate-relative */
/* ideally we would like to keep this private, but that requires a much more complex design...*/

typedef struct _multitap
{
	unsigned long index;
	double amp;
} MULTITAP;


/******************  GENERIC DELAY LINE OBJECT ***********/
typedef struct fx_delay 
{
	float*			dl_buf;
	unsigned long	dl_length;
	unsigned  long	dl_input_index;
	double			dl_srate;
	/* for multi-tap delay */
	MULTITAP*		taps;
	unsigned long 	ntaps;
} FXDELAY;

/* creation function for delay object: */
FXDELAY* new_fxdelay(void);
/* init funcs: Return 0 on success, -1 on error*/
/* for variable delay:  */
int fxdelay_init(FXDELAY* delay,long srate,double length_secs);
/* for tapped delay: */
int fxdelay_mtinit(FXDELAY* delay,long srate,DELAYTAP taps[],int ntaps);
/* the respective tick functions, encapcsulating feedback */
float  fxdelay_tick(FXDELAY* delay, double delaytime_secs,double feedback,float input);
float  fxdelay_mttick(FXDELAY* delay, double feedback,float input);
/* destruction function - destroys internal delay*/
void fxdelay_free(FXDELAY* delay);
/* read time/amp pairs from text file */
int get_taps(FILE* fp, DELAYTAP** ptaps);
const char* taps_getLastError(void);

#ifdef __cplusplus
}	
#endif

#endif
