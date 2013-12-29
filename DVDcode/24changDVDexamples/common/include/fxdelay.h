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
