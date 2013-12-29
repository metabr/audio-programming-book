/* fxrand.h: random stream generators */
#ifndef __FXRAND_H_INCLUDED
#define __FXRAND_H_INCLUDED

#ifdef __cplusplus
extern "C" {	
#endif


typedef struct interp_rand
{
	double magnitude;
	double r1,r2;
	double phase;
	double inv_srate;
} IRAND;

typedef struct sample_hold
{
	double r1;
	double phase;
	double inv_srate;
} SHRAND;

double trirand(void);
void irand_init(IRAND* p_irand,unsigned long srate);
void shrand_init(SHRAND* p_irand,unsigned long srate);
double irand_tick(IRAND* p_irand,double freq);
double shrand_tick(SHRAND* p_rand,double freq);

#ifdef __cplusplus
}	
#endif

#endif
