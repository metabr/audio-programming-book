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

/* ftools.c : a set of first/second order recursive filters */
/* SOURCES:

  (1) Robert Bristow-Johnson "EQ Toolkit"  (music-dsp.org )

  (2) Dodge & Jerse "Computer Music" 2nd Ed, pp 209-219 

	All filter tick functions support time-varying parameters.
	There is no attempt here to optimise the code!

  Functions are provided to enable display of filter coefficients, 
  and to create a filter using externally supplied coefficients.
  No time-varying control possible in this case - but filter will run fast.
*/
/* NB: code adds gain compensation via "ampscale" */
/* BIG TODO: replace BW switch block with func ptrs! */

#include <stdlib.h>
#include <math.h>
#include <ftools.h>

/* standard RBJ filter update funcs */
static double  update_bp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
static double update_lp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
static double update_hp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
static double update_notch(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
static double update_allpass(double *b0,double *b1,double *b2,double alpha,double cosw,double Q);
/* for rbj shelf/eq filters */
static double update_lowshelf(double *a0, double* a1, double*a2,double *b0,double *b1,double *b2,double A,double alpha,double beta,double sinw,double cosw);
static double update_highshelf(double *a0, double* a1, double*a2,double *b0,double *b1,double *b2,double A,double alpha,double beta,double sinw,double cosw);
static double update_peakeq(double *a0, double* a1, double*a2, double *b0, double *b1, double *b2, double A, double alpha,double beta, double sinw,double cosw);
/* DJ butterworth filters */
static void bw_update_lp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr);
static void bw_update_hp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr);
static void bw_update_bp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr);
static void bw_update_notch(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr);


/****** generic biquad creation function using raw coefficients ***********  */

RBJ_FILTER* new_biquad(BQCOEFFS coeffs)
{
	RBJ_FILTER* filter = NULL;

	filter = (RBJ_FILTER*) malloc(sizeof(RBJ_FILTER));
	if(filter == NULL)
		return NULL;
	filter->ampscale = 1.0;
	filter->xnm1 = filter->xnm2 = filter->ynm1 = filter->ynm2 = 0.0;
	filter->coeffs = coeffs;
	return filter;
}

/****************  creation/init func for RBJ standard EQ filters. NB Q MUST be > 0 */

RBJ_FILTER* create_filter(RBJ_FILTERTYPE type, double srate,double freq,double Q)
{
	double alpha/*,beta*/,omega,sinw ,cosw;
	double a0,a1,a2,b0,b1,b2;
	RBJ_FILTER* filter = NULL;

	filter = (RBJ_FILTER*) malloc(sizeof(RBJ_FILTER));
	if(filter == NULL)
		return NULL;
	filter->omegafac = TWOPI / srate;
	
	omega = filter->omegafac * freq;
	sinw = sin(omega);
	cosw = cos(omega);
	alpha = sinw / (2.0 * Q);
	/* these are the same for each filter - donlt need to send them to update func */
	a0 =   1.0 + alpha;
	a1 =  -2.0 * cosw;
	a2 =   1.0 - alpha;
	filter->ampscale = 1.0;
	switch(type){
	case(RBJ_LP):
		b0 =  (1.0 - cosw) * 0.5;
        b1 =   1.0 - cosw;
        b2 =  b0;
		filter->updatefunc = update_lp;
		if(Q > 1.0)
			filter->ampscale = 1.0 / Q;
        break;
	case(RBJ_HP):
		b0 =  (1 + cosw) * 0.5;
        b1 = -(1 + cosw);
        b2 =  b0;
		filter->updatefunc = update_hp;
		if(Q > 1.0)
			filter->ampscale = 1.0 / Q;
		break;
	case(RBJ_NOTCH):
		b0 =   1.0;
        b1 =  -2.0 * cosw;
        b2 =   1.0;
		filter->updatefunc = update_notch;
		break;
	case(RBJ_BP):
		b0 =    alpha;
		b1 =   0;
		b2 =   -alpha;
		filter->updatefunc = update_bp;
		break;
	case(RBJ_ALLPASS):
		b0 =   1.0 - alpha;
        b1 =  -2.0 * cosw;
        b2 =   1.0 + alpha;
		filter->updatefunc = update_allpass;
		break;
	default:
		free(filter);
		return NULL;
	}

	filter->a1a0 = a1 / a0;
	filter->a2a0 = a2 / a0;
	filter->b0a0 = b0 / a0;
	filter->b1a0 = b1 / a0;
	filter->b2a0 = b2 / a0;
	filter->freq = freq;
	filter->Q    = Q;
	filter->xnm1 = filter->xnm2 = filter->ynm1 = filter->ynm2 = 0.0;
	filter->type = type;
	/* for user info only */
	filter->coeffs.a0 = filter->b0a0;
	filter->coeffs.a1 = filter->b1a0;
	filter->coeffs.a2 = filter->b2a0;
	filter->coeffs.b1 = filter->a1a0;
	filter->coeffs.b2 = filter->a2a0;
	return filter;
}

 /********** update funcs for basic filters*/
static double update_lp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q)
{
	*b0 =  (1.0 - cosw) * 0.5;
	*b1 =   1.0 - cosw;
	*b2 =  *b0;
	return 1.0 / Q;
}
static double  update_bp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q)
{
	*b0 =   alpha;
	*b1 =   0.0;
	*b2 =   -alpha;
	return 1.0;
}

static double update_hp(double *b0,double *b1,double *b2,double alpha,double cosw,double Q)
{
	*b0 =  (1.0 + cosw) * 0.5;
    *b1 = -(1.0 + cosw);
    *b2 =  *b0;
	return 1.0 / Q;
}

static double update_notch(double *b0,double *b1,double *b2,double alpha,double cosw,double Q)
{
	*b0 =   1.0;
    *b1 =  -2.0 * cosw;
    *b2 =   1.0;
	return 1.0;
}
static double update_allpass(double *b0,double *b1,double *b2,double alpha,double cosw,double Q)
{
	*b0 =   1.0 - alpha;
	*b1 =  -2.0 * cosw;
	*b2 =   1.0 + alpha;
	return 1.0;
}

/* tick function for RBJ standard filters */

double rbj_filtertick(RBJ_FILTER* filter,double input, double freq, double Q)
{
	double alpha,omega,sinw,cosw;
	double a0,a1,a2,b0,b1,b2;
	double diva0,output;
	
	if(!(filter->Q == Q && filter->freq == freq)){
		omega = freq * filter->omegafac;
 		sinw = sin(omega);
		cosw = cos(omega);
		alpha = sinw/(2.0 * Q);
		filter->Q = Q;
		a0 =   1.0 + alpha;
		a1 =  -2.0 * cosw;
		a2 =   1.0 - alpha;
		filter->ampscale = filter->updatefunc(&b0,&b1,&b2,alpha,cosw,Q);
		diva0 =  1.0 / a0;
		filter->a1a0 = a1 * diva0;
		filter->a2a0 = a2 * diva0;
		filter->b0a0 = b0 * diva0;
		filter->b1a0 = b1 * diva0;
		filter->b2a0 = b2 * diva0;
	   /* only for user info */
		filter->coeffs.a0 = filter->b0a0;
		filter->coeffs.a1 = filter->b1a0;
		filter->coeffs.a2 = filter->b2a0;
		filter->coeffs.b1 = filter->a1a0;
		filter->coeffs.b2 = filter->a2a0;
	}
	
	/*
	y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
                        - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]
	*/
	output =  filter->b0a0 * input + filter->b1a0 * filter->xnm1 + filter->b2a0 * filter->xnm2
		- filter->a1a0 	* filter->ynm1 - filter->a2a0 * filter->ynm2;
	filter->ynm2 = filter->ynm1;
	filter->ynm1 = output;
	filter->xnm2 = filter->xnm1;
	filter->xnm1 = input;

	return output * filter->ampscale;
}

/********** raw biquad tick function - no variable parameters! */

double biquad_tick(RBJ_FILTER* filter,double input)
{
	double output;

	output =  filter->coeffs.a0 * input + filter->coeffs.a1 * filter->xnm1 + filter->coeffs.a2 * filter->xnm2
		- filter->coeffs.b1 * filter->ynm1 - filter->coeffs.b2 * filter->ynm2;
	filter->ynm2 = filter->ynm1;
	filter->ynm1 = output;
	filter->xnm2 = filter->xnm1;
	filter->xnm1 = input;

	return output * filter->ampscale;
}


/********** generic access for coeffcient data ***************/

BQCOEFFS get_rbj_coefficients(const RBJ_FILTER* filter)
{
	BQCOEFFS coeffs = {0.0};
	if(filter){
		coeffs.a0 = filter->b0a0;
		coeffs.a1 = filter->b1a0;
		coeffs.a2 = filter->b2a0;
		coeffs.b1 = filter->a1a0;
		coeffs.b2 = filter->a2a0;
	}
	return coeffs;
}

/* for experimental purtposes only; or use with external filter design program */
void set_rbj_coefficients(RBJ_FILTER* filter,BQCOEFFS coeffs)
{
	if(filter)
		filter->coeffs = coeffs;
}

/*************** RBJ SHELVING / PEAKING EQ FILTERS **************/


/******* creation func ********** */

RBJ_SHELF* create_shelf(RBJ_SHELFTYPE type, double srate, double freq, double Q, double dBGain)
{
	double A,invA,alpha,beta,omega,sinw ,cosw;
	double a0,a1,a2,b0,b1,b2;
	RBJ_SHELF* filter = NULL;

	filter = (RBJ_SHELF*) malloc(sizeof(RBJ_SHELF));
	if(filter == NULL)
		return NULL;
	filter->omegafac = TWOPI / srate;
	
	A = pow(10.0, dBGain * 0.025);	 /* i.e. dBGain / 40 */
	omega = filter->omegafac * freq;
	sinw = sin(omega);
	cosw = cos(omega);
	alpha = sinw / (2.0 * Q);	/* NB needed only for peaking EQ */
	/* NB: if we fix Q to 1.0, we can optimize out the sqrt calc */
	
	beta = sqrt(A) / Q;
	filter->ampscale = 1.0;
	switch(type){
	case(RBJ_SHELF_LOW):
		invA = 1.0 / A;
		a0 = (A + 1.0) + (A - 1.0) * cosw + beta * sinw;
		a1 = -2.0 *((A - 1.0) + (A + 1.0) * cosw);
		a2 = (A + 1.0) + (A - 1.0) * cosw - beta * sinw;
		b0 = A * ((A + 1.0) - (A-1.0) * cosw + beta * sinw);
		b1 = 2.0 * A *((A - 1.0) - (A + 1.0) * cosw);
		b2 = A * ((A + 1.0) - (A - 1.0) * cosw - beta * sinw);
		filter->updatefunc = update_lowshelf;
		if(dBGain > 0.0)
			filter->ampscale = (invA) * (invA) ;
		break;
	case(RBJ_SHELF_HIGH):
		a0 = (A + 1.0) - (A - 1.0) * cosw + beta*sinw;
		a1 = 2.0 *((A - 1.0) - (A + 1.0) *cosw);
		a2 = (A + 1.0) -(A - 1.0) * cosw - beta * sinw;
		b0 = A * ((A + 1.0) + (A - 1.0) * cosw + beta * sinw);
		b1 = -2.0 * A *((A - 1.0) + (A + 1.0) * cosw);
		b2 = A *((A + 1.0) + (A - 1.0) * cosw - beta * sinw);
		filter->updatefunc = update_highshelf;
		break;
	case(RBJ_EQ_PEAKING):
		invA = 1.0 / A;
		a0 = 1.0 + alpha * invA;
		a1 = -2.0 * cosw;
		a2 = 1.0 - alpha * invA;
		b0 = 1.0 + alpha * A;
		b1 = -2.0 * cosw;
		b2 = 1.0 - alpha * A;
		filter->updatefunc = update_peakeq;
		/* RWD: normalize positive gain so output has same peak level as input */
		/* has the effect of reducing original source level to compensate.
		  so some may prefer to skip this! */
		/* NB: does not take account of Q! */
		if(dBGain > 0.0)
			filter->ampscale = (invA) * (invA);
		break;
	default:
		free(filter);
		return NULL;
	}
	filter->a1a0 = a1 / a0;
	filter->a2a0 = a2 / a0;
	filter->b0a0 = b0 / a0;
	filter->b1a0 = b1 / a0;
	filter->b2a0 = b2 / a0;

	filter->freq = freq;
	filter->Q    = Q;
	filter->dBGain = dBGain;
	filter->xnm1 = filter->xnm2 = filter->ynm1 = filter->ynm2 = 0.0;
	filter->type = type;
	/* for user info only */
	filter->coeffs.a0 = filter->b0a0;
	filter->coeffs.a1 = filter->b1a0;
	filter->coeffs.a2 = filter->b2a0;
	filter->coeffs.b1 = filter->a1a0;
	filter->coeffs.b2 = filter->a2a0;

	return filter;
}

/************ update funcs   *****************/

double update_lowshelf(double *a0, double* a1, double*a2,double *b0,double *b1,double *b2,double A,double alpha, double beta,double sinw,double cosw)
{
	*a0 = (A + 1.0) + (A - 1.0) * cosw + beta * sinw;
	*a1 = -2.0 *((A - 1.0) + (A + 1.0) * cosw);
	*a2 = (A + 1.0) + (A - 1.0) * cosw - beta * sinw;
	*b0 = A * ((A + 1.0) - (A-1.0) * cosw + beta * sinw);
	*b1 = 2.0 * A *((A - 1.0) - (A + 1.0) * cosw);
	*b2 = A * ((A + 1.0) - (A - 1.0) * cosw - beta * sinw);
	return 1.0;
}

double update_highshelf(double *a0, double* a1, double*a2,double *b0,double *b1,double *b2,double A,double alpha, double beta,double sinw,double cosw)
{
	*a0 = (A + 1.0) - (A - 1.0) * cosw + beta*sinw;
	*a1 = 2.0 *((A - 1.0) - (A + 1.0) *cosw);
	*a2 = (A + 1.0) -(A - 1.0) * cosw - beta * sinw;
	*b0 = A * ((A + 1.0) + (A - 1.0) * cosw + beta * sinw);
	*b1 = -2.0 * A *((A - 1.0) + (A + 1.0) * cosw);
	*b2 = A *((A + 1.0) + (A - 1.0) * cosw - beta * sinw);
	return 1.0;
}

static double update_peakeq(double *a0, double* a1, double*a2, double *b0, double *b1, double *b2, double A, double alpha,double beta,double sinw,double cosw)
{
	double invA = 1.0 / A;

	*a0 = 1.0 + alpha * invA;
	*a1 = -2.0 * cosw;
	*a2 = 1.0 - alpha * invA;
	*b0 = 1.0 + alpha * A;
	*b1 = -2.0 * cosw;
	*b2 = 1.0 - alpha * A;
	return invA * invA;		/* = reciprocal of dBGain as an amplitude */
}

/********* tick function: variable freq, Q and gain */

double rbj_shelftick(RBJ_SHELF* filter,double input, double freq, double Q, double dBGain)
{
	double A,alpha,beta,omega,sinw ,cosw,diva0;
	double a0,a1,a2,b0,b1,b2;
	double output;

	if(!(filter->Q == Q && filter->dBGain == dBGain && filter->freq == freq)){
		A = pow(10.0, dBGain * 0.025);
		omega = freq * filter->omegafac;
 		sinw = sin(omega);
		cosw = cos(omega);
		alpha = sinw / (2.0 * Q);
		beta = sqrt(A) / Q;
		filter->Q = Q;
		filter->dBGain = dBGain;
		filter->freq = freq;
		filter->ampscale = filter->updatefunc(&a0, &a1,&a2,&b0,&b1,&b2,A,alpha,beta,sinw,cosw);
		diva0 =  1.0 / a0;
		filter->a1a0 = a1 * diva0;
		filter->a2a0 = a2 * diva0;
		filter->b0a0 = b0 * diva0;
		filter->b1a0 = b1 * diva0;
		filter->b2a0 = b2 * diva0;
	    /* only for user info */
		filter->coeffs.a0 = filter->b0a0;
		filter->coeffs.a1 = filter->b1a0;
		filter->coeffs.a2 = filter->b2a0;
		filter->coeffs.b1 = filter->a1a0;
		filter->coeffs.b2 = filter->a2a0;
	}
	output =  filter->b0a0 * input + filter->b1a0 * filter->xnm1 + filter->b2a0 * filter->xnm2
		- filter->a1a0 	* filter->ynm1 - filter->a2a0 * filter->ynm2;
	filter->ynm2 = filter->ynm1;
	filter->ynm1 = output;
	filter->xnm2 = filter->xnm1;
	filter->xnm1 = input;

	return output * filter->ampscale;
}

/* get coefficients: no need to provide a set~ function, as we can use RBJFILTER */

BQCOEFFS get_shelf_coefficients(const RBJ_SHELF* filter)
{
	BQCOEFFS coeffs = {0.0};
	if(filter){
		coeffs.a0 = filter->b0a0;
		coeffs.a1 = filter->b1a0;
		coeffs.a2 = filter->b2a0;
		coeffs.b1 = filter->a1a0;
		coeffs.b2 = filter->a2a0;
	}
	return coeffs;
}

/****************** DODGE/JERSE Basic filters **********/

FX_RESON* new_reson()
{
	FX_RESON* reson = NULL;
	reson = (FX_RESON* ) malloc(sizeof(FX_RESON));
	if(reson == NULL)
		return NULL;
	reson->coeffs.a0 = reson->coeffs.a1 = reson->coeffs.a2 
		= reson->coeffs.b1 = reson->coeffs.b2 = reson->ynm1 
        = reson->ynm2 = 0.0;	
	reson->bw = reson->cf =  reson->srfac = 0.0;
	return reson;
}

BQCOEFFS get_coefficients(const FX_RESON* flt)
{
	BQCOEFFS coeffs = {0.0};
	if(flt)
		coeffs = flt->coeffs;
	return coeffs;
}

void set_reson_coeffcients(FX_RESON* flt, BQCOEFFS coeffs)
{
	flt->coeffs =  coeffs;	
}

/* Second-order classic RESON filter with two poles 
   : Dodge & Jerse 2nd ed. p 211*/

int allpole_init(FX_RESON* flt, unsigned long srate,
                 double cf,double bw)
{
	double b1,b2;
	/* check the arguments */
	if(flt == NULL)
		return -1;
	if(srate == 0 || cf <= 0.0 || bw <= 0.0)
		return -1;
	flt->invsr = 1.0 / (double) srate;
	flt->srfac = TWOPI * flt->invsr;
	b2 = exp(-flt->srfac * bw);
	b1 = (-4.0 * b2) / (1.0 + b2) * cos(flt->srfac * cf);
	flt->coeffs.a0 = (1.0 - b2) 
        * sqrt(1.0 - (b1 * b1) / (4.0 * b2));
	flt->ynm1 = flt->ynm2 = 0.0;
	flt->coeffs.b1 = b1;
	flt->coeffs.b2 = b2;
	flt->cf = cf;
	flt->bw = bw;
	return 0;	
}

/* no argument checks in a tick function */
float allpole_utick(FX_RESON* flt, float input, 
                    double cf,double bw)
{
	double output,b1,b2;
	
	if(!(cf == flt->cf && bw  == flt->bw)){	
		b2 = exp(-flt->srfac * flt->bw);
		
		b1 = (-4.0 * b2) / (1.0 + b2)
            * cos(flt->srfac * cf);
		flt->coeffs.a0 = (1.0 - b2) 
            * sqrt(1.0 - (b1 * b1) / (4.0 * b2));
		flt->coeffs.b1 = b1;
		flt->coeffs.b2 = b2;
		flt->cf = cf;
		flt->bw = bw;
	}
	output = flt->coeffs.a0 * input - flt->coeffs.b1 
        * flt->ynm1 - flt->coeffs.b2 * flt->ynm2;
	flt->ynm2 = flt->ynm1;
	flt->ynm1 = output;
	return (float) output;	
}

float allpole_tick(FX_RESON* flt, float input)
{	
	double output;
	output = flt->coeffs.a0 * input - flt->coeffs.b1 
        * flt->ynm1 - flt->coeffs.b2 * flt->ynm2;
	flt->ynm2 = flt->ynm1;
	flt->ynm1 = output;
	return (float) output;	
}

void allpole_set_cf(FX_RESON* flt, double cf)
{
    double b1,b2;

	if(cf == flt->cf)
		return;
    /* bw is unchanged, so no need to recalculate b2 */
    b2 = flt->coeffs.b2; 
    b1 = (-4.0 * b2) / (1.0 + b2) * cos(flt->srfac * cf);
	flt->coeffs.a0 = (1.0 - b2) 
        * sqrt(1.0 - (b1 * b1) / (4.0 * b2));
    flt->coeffs.b1 = b1;
	flt->cf = cf;
}

void allpole_set_bw(FX_RESON* flt, double bw)
{
	double b1,b2;
    
	if(bw == flt->bw)
		return;
    b2 = exp(-flt->srfac * bw);
    b1 = (-4.0 * b2) / (1.0 + b2) 
        * cos(flt->srfac * flt->cf);
	flt->coeffs.a0 = (1.0 - b2) 
        * sqrt(1.0 - (b1 * b1) / (4.0 * b2));
    flt->coeffs.b1 = b1;
	flt->coeffs.b2 = b2;
	flt->bw = bw;
}


/* RWD: VRESEON - NOT LISTED IN THE BOOK, See Exercises, Ch2.8. 
    modified reson filter using DJ version 2 for amp scaling, but
   the classic basic calc for b1 and b2 - faster! 
   Note output level much higher - still too hot for white noise! 
   This also adds the second-order zeros.
*/

/* NB: to explore the most basic reson (not gain scaled ), set flt->a0 to 1 */
int vreson_init(FX_RESON* flt, unsigned long srate,double cf,double bw)
{
	double R,theta,temp;
	if(flt == NULL)
		return -1;
	if(srate == 0 || cf <= 0.0 || bw <= 0.0)
		return -1;
	
	flt->invsr = 1.0 / (double)srate;
	R = exp(-M_PI * bw * flt->invsr);	
	flt->srfac = TWOPI * flt->invsr;
	theta = flt->srfac * cf;
	flt->coeffs.b1 = -2.0 * R * cos(theta);	
	flt->coeffs.b2 = (R * R);
/*
	flt->a0 = (1.0 - R*R) * sin(theta);
*/
	/* DJ option 2 - seems to need the extra -6dB scale factor too */
	temp = 1.0 + flt->coeffs.b2;
	flt->coeffs.a0 = 0.5 * sqrt( ((1.0 - flt->coeffs.b2) / temp) * (temp*temp - flt->coeffs.b1 * flt->coeffs.b1)  );
	flt->coeffs.a2 = -flt->coeffs.a0;
	flt->ynm1 = flt->ynm2 = flt->xnm1 = flt->xnm2 = 0.0;
	flt->cf = cf;
	flt->bw = bw;
	return 0;	
}

float vreson_tick(FX_RESON* flt, float input, double cf,double bw)
{
	double R,output,theta,temp;
	
	if(!(cf == flt->cf && bw  == flt->bw)){
		theta = flt->srfac * cf;
		R = exp(-M_PI * bw * flt->invsr);	
		flt->coeffs.b1 = -2.0 * R * cos(theta);		
		flt->coeffs.b2 = (R * R);		
/*
		flt->coeffs.a0 = (1.0 - R*R) * sin(theta);
*/
		temp = 1.0 + flt->coeffs.b2;
		flt->coeffs.a0 = 0.5 * sqrt( ((1.0 - flt->coeffs.b2) / temp) * (temp*temp - flt->coeffs.b1 * flt->coeffs.b1)  );
		flt->coeffs.a2 = -flt->coeffs.a0;
		flt->cf = cf;
		flt->bw = bw;
	}	
	output = flt->coeffs.a0 * input + flt->coeffs.a2 * flt->xnm2 - flt->coeffs.b1 * flt->ynm1 - flt->coeffs.b2 * flt->ynm2;
	flt->ynm2 = flt->ynm1;
	flt->ynm1 = output;
	flt->xnm2 = flt->xnm1;
	flt->xnm1 = input;

	return (float) output;	
}

/**********  "improved" second-order filter witth two zeroes : D&J pp 212/3**************/

int polezero_init(FX_RESON* flt, unsigned long srate,double cf,double bw)
{
	if(flt == NULL)
		return -1;
	/* trust the params! */
	flt->cf = cf;
	flt->bw = bw;
	flt->invsr = 1.0 / (double)srate;
	flt->srfac = TWOPI * flt->invsr;
	flt->coeffs.b2 = exp(-flt->srfac * flt->bw);
	flt->coeffs.b1 = (-4.0* flt->coeffs.b2)/ (1.0 + flt->coeffs.b2) * cos(flt->srfac * flt->cf);
	flt->coeffs.a0 = 1.0 - sqrt(flt->coeffs.b2);
	flt->coeffs.a2 = -flt->coeffs.a0;
	flt->ynm1 = flt->ynm2 = flt->xnm1 = flt->xnm2 = 0.0;
	return 0;	
}

float polezero_tick(FX_RESON* flt, float input, double cf,double bw)
{
	double output;
	if(!(cf == flt->cf && bw  == flt->bw)){
		flt->cf = cf;
		flt->bw = bw;	
		flt->coeffs.b2 = exp(-flt->srfac * flt->bw);
		flt->coeffs.b1 = ((-4.0* flt->coeffs.b2)/ (1.0 + flt->coeffs.b2)) * cos(flt->srfac * flt->cf);
		flt->coeffs.a0 = 1.0 - sqrt(flt->coeffs.b2);	
		flt->coeffs.a2 = -flt->coeffs.a0;
	}	
	output = flt->coeffs.a0 * input + flt->coeffs.a2 * flt->xnm2 - flt->coeffs.b1 * flt->ynm1 - flt->coeffs.b2 * flt->ynm2;
	flt->ynm2 = flt->ynm1;
	flt->ynm1 = output;
	flt->xnm2 = flt->xnm1;
	flt->xnm1 = input;
	return (float) output;	
}


/*********** DODGE/JERSE Butterworth filters: Lowpass, highpass, bandpass and notch ****************/

BW_FILTER* new_bw_filter()
{
	BW_FILTER* bwf = NULL;
	BQCOEFFS coeffs;
	bwf = (BW_FILTER* ) malloc(sizeof(BW_FILTER));
	if(bwf == NULL)
		return NULL;
	coeffs.a0 = coeffs.a1 = coeffs.a2 = coeffs.b1 = coeffs.b2 = bwf->xnm1 = bwf->xnm2 = bwf->ynm1 = bwf->ynm2 = 0.0;	
	bwf->bw = bwf->cf =  bwf->srate = bwf->pidivsr = bwf->twopidivsr = 0.0;
	bwf->coeffs = coeffs;
	return bwf;
}

BQCOEFFS get_bw_coefficients(const BW_FILTER* filter)
{
	BQCOEFFS coeffs = {0.0};
	if(filter)
	    return filter->coeffs;
	return coeffs;
}

int bw_init(BW_FILTER* bwf, BW_FILTERTYPE ftype, unsigned long srate,double cf,double bw)
{
	if(bwf == NULL)
		return -1;
	bwf->srate = (double) srate;
	bwf->cf = cf;
	bwf->bw = bw;
	bwf->pidivsr = M_PI / bwf->srate;
	bwf->twopidivsr = 2.0 * bwf->pidivsr;
	bwf->ftype = ftype;
	switch(ftype){
	case(BW_LOPASS):
		bwf->func = bw_update_lp;
		break;
	case(BW_HIPASS):
		bwf->func = bw_update_hp;
		break;
	case(BW_BANDPASS):
		bwf->func = bw_update_bp;
		break;
	case(BW_NOTCH):
		bwf->func = bw_update_notch;
		break;
	default:
		return -1;		
	}
	bwf->func(&bwf->coeffs,cf,bw,bwf->pidivsr,bwf->twopidivsr);
	return 0;	
}

void bw_update_lp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr)
{
	double C;
	C =  1.0 / tan(pidivsr * cf);
	coeffs->a0 = 1.0 / (1 + SQRT2 * C + (C * C));
	coeffs->a1 = 2.0 * coeffs->a0;
	coeffs->a2 = coeffs->a0;
	coeffs->b1 = 2.0 * coeffs->a0 * (1.0 - (C*C));
	coeffs->b2 = coeffs->a0 * (1.0 - SQRT2 * C + (C * C));
}

void bw_update_hp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr)
{
	double C;
	C =  tan(pidivsr * cf);
	coeffs->a0 = 1.0 / (1 + SQRT2 * C + (C * C));
	coeffs->a1 = -2.0 * coeffs->a0;
	coeffs->a2 = coeffs->a0;
	coeffs->b1 = 2.0 * coeffs->a0 * ((C*C) - 1.0);
	coeffs->b2 = coeffs->a0 * (1.0 - SQRT2 * C + (C * C));	
}

void bw_update_bp(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr)
{
	double C,D;

	C =  1.0 / tan(pidivsr * bw);
	D = 2.0 * cos(twopidivsr * cf);
	coeffs->a0 = 1.0 / (1.0 + C);
	coeffs->a1 = 0.0;
	coeffs->a2 = - coeffs->a0;
	coeffs->b1 = - coeffs->a0 * C * D;
	coeffs->b2 = coeffs->a0 * (C - 1.0);	
}


void bw_update_notch(BQCOEFFS* coeffs, double cf, double bw,double pidivsr,double twopidivsr)
{
	double C,D;

	C =  tan(pidivsr * bw);
	D =  2.0 * cos(twopidivsr * cf);
	coeffs->a0 = 1.0 / (1.0 + C);
	coeffs->a1 = -coeffs->a0 * D;;
	coeffs->a2 = coeffs->a0;
	coeffs->b1 = -coeffs->a0 * D;
	coeffs->b2 = coeffs->a0 * (1.0 - C);
	
}

float bw_tick(BW_FILTER* bwf, float input, double cf,double bw)
{
	double output;

	if(!(bwf->cf == cf && bwf->bw == bw)){
		bwf->cf = cf;
		bwf->bw = bw;
		bwf->pidivsr = M_PI / bwf->srate;
		bwf->twopidivsr = 2.0 * bwf->pidivsr;
		/* update coefficients */
		bwf->func(&bwf->coeffs,cf,bw,bwf->pidivsr,bwf->twopidivsr);
	}
	output = bwf->coeffs.a0 * input + bwf->coeffs.a1 * bwf->xnm1 + bwf->coeffs.a2 * bwf->xnm2
				- bwf->coeffs.b1 * bwf->ynm1 - bwf->coeffs.b2 * bwf->ynm2;
	bwf->xnm2 = bwf->xnm1;	bwf->xnm1 = input;
	bwf->ynm2 = bwf->ynm1;	bwf->ynm1 = output;
	return (float) output;
}

/************* D/J single-pole lowpass/higpass filters **********/

TONE* new_tone()
{
	TONE* p_tone = NULL;
	p_tone = (TONE*) malloc(sizeof(TONE));
	if(p_tone == NULL)
		return NULL;
	
	return p_tone;
}

int tone_init(TONE* p_tone,double srate,double freq,int tonetype)
{
	double c;
	p_tone->freq = freq;
	p_tone->twopidivsr = TWOPI / srate;
	if(tonetype == TONE_LOWPASS){
		c = 2.0 - cos(p_tone->twopidivsr * freq);
		p_tone->b1 = sqrt((c*c) - 1.0) - c;
		p_tone->a0 = 1.0 + p_tone->b1;
	}
	else{
		c = 2.0 + cos(p_tone->twopidivsr * freq);
		p_tone->b1 = c -  sqrt((c*c) -1.0);
		p_tone->a0 = 1.0 - p_tone->b1;		
	}
	p_tone->type = tonetype;
	p_tone->lastout = 0.0;
	return 0;
}


double tone_tick(TONE* p_tone,double input, double freq)
{
	double output;
	if(freq != p_tone->freq){
		double c;
		if(p_tone->type == TONE_LOWPASS){
			c = 2.0 - cos(p_tone->twopidivsr * freq);
			p_tone->b1 = sqrt((c*c) - 1.0) - c;
			p_tone->a0 = 1.0 + p_tone->b1;
		}
		else{
			c = 2.0 + cos(p_tone->twopidivsr * freq);
			p_tone->b1 = c -  sqrt((c*c) -1.0);
			p_tone->a0 = 1.0 - p_tone->b1;		
		}
	}
	output  =  p_tone->a0 * input - p_tone->b1 * p_tone->lastout;
	p_tone->lastout =  output;
	return output;
}

BQCOEFFS get_tone_coefficients(const TONE* p_tone)
{
	BQCOEFFS coeffs = {0.0};
	if(p_tone){
		coeffs.a0 = p_tone->a0;
		coeffs.b1 = p_tone->b1;
	}
	return coeffs;
}

