/* wave.h: defintion of oscillator types and associated waveform generation functions  */
#ifndef __WAVE_H_INCLUDED__
#define	__WAVE_H_INCLUDED__

#ifdef __cplusplus
extern "C" {	
#endif


#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif

/* definition of the oscillator type */
typedef struct  t_oscil 
{
	double srate;
	double twopiovrsr;
	double curfreq;
	double curphase;
	double incr;
} OSCIL;

/* returns pointer to an OSCIL object created on the heap. Release the memory by passing the pointer to free().*/
OSCIL*  new_oscil(double srate);
/* waveform tick functions, called per sample at the stated srate */
double sinetick  (OSCIL* p_osc, double freq);
double sqtick    (OSCIL* p_osc, double freq);
double sawdtick  (OSCIL* p_osc, double freq);
double sawutick  (OSCIL* p_osc, double freq);
double trunctick (OSCIL* p_osc, double freq);
double tritick   (OSCIL* p_osc, double freq);
/* function pointer for tick funcs */
typedef double (*tickfunc)(OSCIL* osc,double);

#ifdef __cplusplus
}	
#endif


#endif
