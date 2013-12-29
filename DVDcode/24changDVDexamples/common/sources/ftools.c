/* TODO: replace switch block with func ptrs */
#include <stdlib.h>
#include <math.h>
#include "ftools.h"

static void  update_bp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);
static void update_bpq(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);
static void update_lp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);
static void update_hp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);
static void update_notch(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);
static void update_allpass(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q);


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
	a0 =   1.0 + alpha;
	a1 =  -2.0 * cosw;
	a2 =   1.0 - alpha;
	filter->ampscale = 1.0;
	switch(type){
	case(RBJFILT_BP):
		b0 =    alpha;
		b1 =   0;
		b2 =   -alpha;
		filter->updatefunc = update_bp;
		break;
	case(RBJFILT_BPQ):
		b0 =   Q * alpha;
		b1 =   0;
		b2 =   -Q * alpha;
		filter->updatefunc = update_bpq;
		filter->ampscale = 1.0 / Q;
		break;
	case(RBJFILT_LP):
		b0 =  (1.0 - cosw) * 0.5;
        b1 =   1.0 - cosw;
        b2 =  b0;
		filter->updatefunc = update_lp;
		//filter->ampscale = 1.0 / Q;
        break;
	case(RBJFILT_HP):
		b0 =  (1 + cosw) * 0.5;
        b1 = -(1 + cosw);
        b2 =  b0;
		filter->updatefunc = update_hp;
		filter->ampscale = 1.0 / Q;
		break;
	case(RBJFILT_NOTCH):
		b0 =   1.0;
        b1 =  -2.0 * cosw;
        b2 =   1.0;
		filter->updatefunc = update_notch;
		break;
	case(RBJFILT_ALLPASS):
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
	return filter;
}

static void update_lp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	*b0 =  (1.0 - cosw) * 0.5;
	*b1 =   1.0 - cosw;
	*b2 =  *b0;	
}

static void  update_bp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	*b0 =   alpha;
	*b1 =   0.0;
	*b2 =   -alpha;
}

static void update_bpq(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	
	*b0 =   Q * alpha;
	*b1 =   0.0;
	*b2 =   -Q * alpha;
	
}



static void update_hp(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	*b0 =  (1.0 + cosw) * 0.5;
    *b1 = -(1.0 + cosw);
    *b2 =  *b0;	
}

static void update_notch(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	*b0 =   1.0;
    *b1 =  -2.0 * cosw;
    *b2 =   1.0;	
}

static void update_allpass(double *b0,double *b1,double *b2,double alpha,double sinw,double cosw,double Q)
{
	*b0 =   1.0 - alpha;
	*b1 =  -2.0 * cosw;
	*b2 =   1.0 + alpha;	
}

double fx_filtertick(RBJ_FILTER* filter,double input, double freq, double Q)
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
#ifdef NOTDEF
		switch(filter->type){
		case(RBJFILT_BP):
			b0 =   alpha;
			b1 =   0.0;
			b2 =   -alpha;			
			break;
		case(RBJFILT_BPQ):
			b0 =   Q * alpha;
			b1 =   0;
			b2 =   -Q * alpha;			
			break;
		case(RBJFILT_LP):
			b0 =  (1.0 - cosw) * 0.5;
			b1 =   1.0 - cosw;
			b2 =  b0;			
			break;
		case(RBJFILT_HP):
			b0 =  (1.0 + cosw) * 0.5;
            b1 = -(1.0 + cosw);
            b2 =  b0;
			break;
		case(RBJFILT_NOTCH):
			b0 =   1.0;
            b1 =  -2.0 * cosw;
            b2 =   1.0;
			break;
		case(RBJFILT_ALLPASS):
			b0 =   1.0 - alpha;
			b1 =  -2.0 * cosw;
			b2 =   1.0 + alpha;
			break;
		default:
			return 0.0;
		}
#else
		filter->updatefunc(&b0,&b1,&b2,alpha,sinw,cosw,Q);
#endif
		diva0 =  1.0 / a0;
		filter->a1a0 = a1 * diva0;
		filter->a2a0 = a2 * diva0;
		filter->b0a0 = b0 * diva0;
		filter->b1a0 = b1 * diva0;
		filter->b2a0 = b2 * diva0;
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


ALLPOLE_RESON* new_allpole()
{
	ALLPOLE_RESON* reson = NULL;
	reson = (ALLPOLE_RESON* ) malloc(sizeof(ALLPOLE_RESON));
	if(reson == NULL)
		return NULL;
	reson->a0 = reson->b1 =reson->b2 = reson->ynm1 = reson->ynm2 = 0.0;	
	reson->bw = reson->cf =  reson->srate = reson->srfac = 0.0;
	return reson;
}

int allpole_init(ALLPOLE_RESON* flt, unsigned long srate,double cf,double bw)
{
	if(flt == NULL)
		return -1;
	/* trust the params! */
	flt->srate = (double) srate;
	flt->cf = cf;
	flt->bw = bw;
	flt->srfac = TWOPI / flt->srate;
	flt->b2 = exp(-flt->srfac * flt->bw);
	flt->b1 = (-4.0* flt->b2)/ (1.0 + flt->b2) * cos(flt->srfac * flt->cf);
	flt->a0 = (1.0 - flt->b2) * sqrt(1.0 - ((flt->b1 * flt->b1) / (4.0 * flt->b2)));
	flt->ynm1 = flt->ynm2 = 0.0;
	return 0;	
}
float allpole_tick(ALLPOLE_RESON* flt, float input, double cf,double bw)
{
	double output;
	if(!(cf == flt->cf && bw  == flt->bw)){
		flt->cf = cf;
		flt->bw = bw;	
		flt->b2 = exp(-flt->srfac * flt->bw);
		flt->b1 = (-4.0* flt->b2)/ (1.0 + flt->b2) * cos(flt->srfac * flt->cf);
		flt->a0 = (1.0 - flt->b2) * sqrt(1.0 - ((flt->b1 * flt->b1) / (4.0 * flt->b2)));	
	}
	
	output = flt->a0 * input - flt->b1 * flt->ynm1 - flt->b2 * flt->ynm2;
	flt->ynm2 = flt->ynm1;
	flt->ynm1 = output;
	return (float) output;	
}

/* Dodge & Jerse Butterworth Filters */
BW_FILTER* new_bw_filter()
{
	BW_FILTER* bwf = NULL;
	bwf = (BW_FILTER* ) malloc(sizeof(BW_FILTER));
	if(bwf == NULL)
		return NULL;
	bwf->a0 = bwf->a1 = bwf->a2 = bwf->b1 =bwf->b2 = bwf->xnm1 = bwf->xnm2 = bwf->ynm1 = bwf->ynm2 = 0.0;	
	bwf->bw = bwf->cf =  bwf->srate = bwf->pidivsr = bwf->twopidivsr = 0.0;
	return bwf;
};
int bw_init(BW_FILTER* bwf, BW_FILTERTYPE ftype, unsigned long srate,double cf,double bw)
{
	double C,D;

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
		C =  1.0 / tan(bwf->pidivsr * cf);
		bwf->a0 = 1.0 / (1 + sqrt(2.0) * C + (C * C));
		bwf->a1 = 2.0 * bwf->a0;
		bwf->a2 = bwf->a0;
		bwf->b1 = 2.0 *bwf->a0 * (1.0 - (C*C));
		bwf->b2 = bwf->a0 * (1.0 - sqrt(2.0) * C + (C * C));
		break;
	case(BW_HIPASS):
		C =  tan(bwf->pidivsr * cf);
		bwf->a0 = 1.0 / (1 + sqrt(2.0) * C + (C * C));
		bwf->a1 = -2.0 * bwf->a0;
		bwf->a2 = bwf->a0;
		bwf->b1 = 2.0 *bwf->a0 * ((C*C) - 1.0);
		bwf->b2 = bwf->a0 * (1.0 - sqrt(2.0) * C + (C * C));
		break;
	case(BW_BANDPASS):
		C =  1.0 / tan(bwf->pidivsr * bwf->bw);
		D = 2.0 * cos(bwf->twopidivsr * bwf->cf);
		bwf->a0 = 1.0 / (1.0 + C);
		bwf->a1 = 0.0;
		bwf->a2 = - bwf->a0;
		bwf->b1 = - bwf->a0 * C *D;
		bwf->b2 = bwf->a0 * (C - 1.0);
		break;
	case(BW_NOTCH):
		C =  tan(bwf->pidivsr * bwf->bw);
		D =  2.0 * cos(bwf->twopidivsr * bwf->cf);
		bwf->a0 = 1.0 / (1.0 + C);
		bwf->a1 = -bwf->a0 * D;;
		bwf->a2 = bwf->a0;
		bwf->b1 = -bwf->a0 * D;
		bwf->b2 = bwf->a0 * (1.0 - C);
		break;
	default:
		break;
	}

	return 0;

	
}


float bw_tick(BW_FILTER* bwf, float input, double cf,double bw)
{
	double output,C,D;

	if(!(bwf->cf == cf && bwf->bw == bw)){
		bwf->cf = cf;
		bwf->bw = bw;
		bwf->pidivsr = M_PI / bwf->srate;
		bwf->twopidivsr = 2.0 * bwf->pidivsr;
		switch(bwf->ftype){
		case(BW_LOPASS):
			C =  1.0 / tan(bwf->pidivsr * cf);
			bwf->a0 = 1.0 / (1 + SQRT2 * C + (C * C));
			bwf->a1 = 2.0 * bwf->a0;
			bwf->a2 = bwf->a0;
			bwf->b1 = 2.0 *bwf->a0 * (1.0 - (C*C));
			bwf->b2 = bwf->a0 * (1.0 - SQRT2 * C + (C * C));
			break;
		case(BW_HIPASS):
			C =  tan(bwf->pidivsr * cf);
			bwf->a0 = 1.0 / (1 + SQRT2 * C + (C * C));
			bwf->a1 = -2.0 * bwf->a0;
			bwf->a2 = bwf->a0;
			bwf->b1 = 2.0 *bwf->a0 * ((C*C) - 1.0);
			bwf->b2 = bwf->a0 * (1.0 - SQRT2 * C + (C * C));
			break;		
		case(BW_BANDPASS):
			C =  1.0 / tan(bwf->pidivsr * bwf->bw);
			D = 2.0 * cos(bwf->twopidivsr * bwf->cf);
			bwf->a0 = 1.0 / (1.0 + C);
			bwf->a1 = 0.0;
			bwf->a2 = - bwf->a0;
			bwf->b1 = - bwf->a0 * C *D;
			bwf->b2 = bwf->a0 * (C - 1.0);
			break;
		case(BW_NOTCH):
			C =  tan(bwf->pidivsr * bwf->bw);
			D =  2.0 * cos(bwf->twopidivsr * bwf->cf);
			bwf->a0 = 1.0 / (1.0 + C);
			bwf->a1 = -bwf->a0 * D;;
			bwf->a2 = bwf->a0;
			bwf->b1 = -bwf->a0 * D;
			bwf->b2 = bwf->a0 * (1.0 - C);
			break;
		default:
			break;
		}
	}
	output = bwf->a0 * input + bwf->a1 * bwf->xnm1 + bwf->a2 * bwf->xnm2
				- bwf->b1 * bwf->ynm1 - bwf->b2 * bwf->ynm2;
	bwf->xnm2 = bwf->xnm1;
	bwf->xnm1 = input;
	bwf->ynm2 = bwf->ynm1;
	bwf->ynm1 = output;

	return (float) output;
}

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
	double b;
	p_tone->freq = freq;
	p_tone->twopidivsr = TWOPI / srate;
	b = 2.0 - cos(p_tone->twopidivsr * freq);
	p_tone->a2 = sqrt((b*b) - 1.0) - b;
	p_tone->a1 = 1.0 + p_tone->a2;
	p_tone->type = tonetype;
	if(tonetype==TONE_HIGHPASS)
		p_tone->a2 *= -1.0;
	p_tone->lastout = 0.0;
	return 0;
}


double tone_tick(TONE* p_tone,double input, double freq)
{
	double output;
	if(freq != p_tone->freq){
		double b;
		b = 2.0 - cos(p_tone->twopidivsr * freq);
		p_tone->a2 = sqrt((b*b) - 1.0) - b;
		p_tone->a1 = 1.0 + p_tone->a2;
		
		if(p_tone->type==TONE_HIGHPASS)
			p_tone->a2 *= -1.0;
	}
	output  =  p_tone->a1*input - p_tone->a2 * p_tone->lastout;
	p_tone->lastout =  output;
	return output;
}


/*******
 
lowpass1::lowpass1(double filtgain)
{
	temp = output = 0.0;
	gain = filtgain;
}

lowpass1::~lowpass1()
{
}
 //IIR lowpass
inline double lowpass1::tick(double insam)
{	 
	output  = temp;
	temp = insam + (output * gain);
	return output;

}



  **************/


