/* ftools.h */
#ifndef _FTOOLS_H_INCLUDED
#define _FTOOLS_H_INCLUDED

#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif 

#define SQRT2 (1.4142135623730951)

/**********   RBJ 2ND-ORDER FILTER *********/

typedef enum rbj_filtertype {RBJFILT_LP,RBJFILT_HP,RBJFILT_BP,RBJFILT_NOTCH,RBJFILT_BPQ,RBJFILT_ALLPASS} RBJ_FILTERTYPE;
typedef void (*rbj_updatefunc)(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);


typedef struct fx_filter {
	double b0a0,b1a0,b2a0,a1a0,a2a0;
	double xnm1,xnm2,ynm1,ynm2;
	double freq;
	double Q, ampscale;
	double omegafac;
	RBJ_FILTERTYPE type;
	rbj_updatefunc 	 updatefunc;
} RBJ_FILTER;


RBJ_FILTER* create_filter(RBJ_FILTERTYPE type,double srate,double freq,double Q);
double fx_filtertick(RBJ_FILTER* filter,double input, double freq, double Q);


typedef struct allpole_reson
{
	double ynm1,ynm2;
	double a0,b1,b2;
	double cf, srate,bw;
	double srfac;
} ALLPOLE_RESON;

ALLPOLE_RESON* new_allpole();
int allpole_init(ALLPOLE_RESON* flt, unsigned long srate,double cf,double bw);
float allpole_tick(ALLPOLE_RESON* flt, float input, double cf,double bw);

typedef enum bw_filtertype {BW_LOPASS,BW_HIPASS,BW_BANDPASS,BW_NOTCH} BW_FILTERTYPE;
/* Dodge & Jerse Butterworth filters */
typedef struct butterworth_filter 
{
	double xnm1,xnm2,ynm1,ynm2;
	double a0,a1,a2,b1,b2;
	double cf,srate,bw;
	double pidivsr,twopidivsr;
	BW_FILTERTYPE ftype;
} BW_FILTER;

BW_FILTER* new_bw_filter();
int bw_init(BW_FILTER* flt, BW_FILTERTYPE ftype, unsigned long srate,double cf,double bw);
float bw_tick(BW_FILTER* flt, float input, double cf,double bw);


/* simpel 1st order filter*/
typedef struct tone 
{
	double a1,a2;
	double lastout;
	double twopidivsr;
	double freq;
	int type;
} TONE;

enum {TONE_LOWPASS,TONE_HIGHPASS};
TONE* new_tone();
int tone_init(TONE* filter,double srate,double freq, int tonetype);
double tone_tick(TONE* p_tone,double input,double freq);


#endif
