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

/* ftools.h : a set of first/second order recursive filters */
/* SOURCES:

  (1) Robert Bristow-Johnson "EQ Cookbook"  (ref. music-dsp.org )

  (2) Dodge & Jerse "Computer Music" 2nd Ed, pp 209-219 

	All filter tick functions support time-varying parameters.
	There is no attempt here to optimise the code!

  Functions are provided to enable display of filter coefficients, 
  and to create a filter using externally supplied coefficients.
  No time-varying control possible in this case!
*/

#ifndef _FTOOLS_H_INCLUDED
#define _FTOOLS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif 

#define SQRT2 (1.4142135623730951)
/* just for ease of informing techie user. */
typedef struct bq_coeffs
{
	double a0,a1,a2,b1,b2;
} BQCOEFFS;

/**********   RBJ 2ND-ORDER FILTERS *********/
/* enum for commandline arg */
typedef enum rbj_filtertype {RBJ_LP,RBJ_HP,RBJ_BP,RBJ_NOTCH,RBJ_ALLPASS,RBJ_LOWSHELF,RBJ_HIGHSHELF,RBJ_PEAKEQ} RBJ_FILTERTYPE;
/* shelf/peakeq filters have to be dealt with separately, sadly */
typedef enum rbj_shelf_filtertype {RBJ_SHELF_LOW, RBJ_SHELF_HIGH,RBJ_EQ_PEAKING } RBJ_SHELFTYPE;
/* we can use a compact func for the basic filters */
/* we would prefer the user did not have direct access to these functions! */
typedef double (*rbj_updatefunc)(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
/* returns ampscale factor (for peakeq, relative to dBGain and hence to A )*/
typedef double (*rbj_shelf_updatefunc)(double* a0, double*a1,double* a2,double *b0,double *b1,double *b2,double A,double alpha,double beta,double sinw,double cosw);

typedef struct rbj_filter {
	double b0a0,b1a0,b2a0,a1a0,a2a0;
	double xnm1,xnm2,ynm1,ynm2;
	double freq;
	double Q, ampscale;
	double omegafac;
	RBJ_FILTERTYPE type;
	rbj_updatefunc 	 updatefunc;
	BQCOEFFS coeffs;			   /* only used to give info to user */
} RBJ_FILTER;

RBJ_FILTER* new_biquad(BQCOEFFS coeffs);
RBJ_FILTER* create_filter(RBJ_FILTERTYPE type,double srate,double freq,double Q);
double		rbj_filtertick(RBJ_FILTER* filter,double input, double freq, double Q);
double		biquad_tick(RBJ_FILTER* filter,double input);
BQCOEFFS	get_rbj_coefficients(const RBJ_FILTER* filter);
void		set_rbj_coefficients(RBJ_FILTER* filter,BQCOEFFS coeffs);


typedef struct rbj_shelf {
	double A,dBGain;
	double b0a0,b1a0,b2a0,a1a0,a2a0;
	double xnm1,xnm2,ynm1,ynm2;
	double freq;
	double Q, ampscale;
	double omegafac;
	RBJ_SHELFTYPE type;
	rbj_shelf_updatefunc updatefunc;
	BQCOEFFS coeffs;			   /* only need to give info to user */
}  RBJ_SHELF;

RBJ_SHELF*	create_shelf(RBJ_SHELFTYPE type, double srate, double freq,double Q, double dBGain);
double		rbj_shelftick(RBJ_SHELF* filter,double input, double freq, double Q, double dBGain);
BQCOEFFS	get_shelf_coefficients(const RBJ_SHELF* filter);

/* Dodge/Jerse  examples */
typedef struct fx_reson
{
	BQCOEFFS coeffs;
	//double a0,a1,a2,b1,b2;
	double ynm1,ynm2,xnm1,xnm2;
	double cf, bw;
	double invsr,srfac;
} FX_RESON;

FX_RESON* new_reson();
int		allpole_init(FX_RESON* flt, unsigned long srate,double cf,double bw);
float	allpole_utick(FX_RESON* flt, float input, double cf,double bw);
float allpole_tick(FX_RESON* flt, float input);
void allpole_set_cf(FX_RESON* flt, double cf);
void allpole_set_bw(FX_RESON* flt, double bw);

int		polezero_init(FX_RESON* flt, unsigned long srate,double cf,double bw);
float	polezero_tick(FX_RESON* flt, float input, double cf,double bw);

/* extras! see text */
int		vreson_init(FX_RESON* flt, unsigned long srate,double cf,double bw);
float	vreson_tick(FX_RESON* flt, float input, double cf,double bw);


BQCOEFFS get_coefficients(const FX_RESON* flt);
void			set_reson_coeffcients(FX_RESON* flt, BQCOEFFS coeffs);
typedef enum	bw_filtertype {BW_LOPASS,BW_HIPASS,BW_BANDPASS,BW_NOTCH} BW_FILTERTYPE;
/* Dodge & Jerse Butterworth filters */
typedef  void (*bw_updatefunc)(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr);


typedef struct butterworth_filter 
{
	BQCOEFFS coeffs;
	double xnm1,xnm2,ynm1,ynm2;
	double cf,srate,bw;
	double pidivsr,twopidivsr;
	BW_FILTERTYPE ftype;
	bw_updatefunc func;
} BW_FILTER;




BQCOEFFS get_bw_coefficients(const BW_FILTER* filter);

BW_FILTER* new_bw_filter();
int		bw_init(BW_FILTER* flt, BW_FILTERTYPE ftype, unsigned long srate,double cf,double bw);
float	bw_tick(BW_FILTER* flt, float input, double cf,double bw);


/* simple 1st order IIR filter*/
typedef struct tone 
{
	double	a0,b1;
	double	lastout;
	double	twopidivsr;
	double	freq;
	int		type;
} TONE;

BQCOEFFS get_tone_coefficients(const TONE* p_tone);

enum {TONE_LOWPASS,TONE_HIGHPASS};
TONE*	new_tone();
int		tone_init(TONE* filter,double srate,double freq, int tonetype);
double	tone_tick(TONE* p_tone,double input,double freq);


#ifdef __cplusplus
}
#endif

#endif
