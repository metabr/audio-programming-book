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

/*filterbank.c*/
#include <math.h>
#include <stdlib.h>
#include "filterbank.h"

#ifndef M_PI
#define M_PI  (3.1415926535897932)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif
/* return 0 for success */
int rbj_createbank(RBJ_FILTERBANK* fbank,int nfilters)
{
	if(fbank == NULL || fbank->nfilters > 0)
		return 1;

	fbank->nfilters = nfilters;
	fbank->b0a0 =  (double*) malloc(nfilters * sizeof(double));
	fbank->b1a0 =  (double*) malloc(nfilters * sizeof(double));
	fbank->b2a0 =  (double*) malloc(nfilters * sizeof(double));
	fbank->a1a0 =  (double*) malloc(nfilters * sizeof(double));
	fbank->a2a0 =  (double*) malloc(nfilters * sizeof(double));
	fbank->xnm1 =  (double*) malloc(nfilters * sizeof(double));
	fbank->xnm2 =  (double*) malloc(nfilters * sizeof(double));
	fbank->ynm1 =  (double*) malloc(nfilters * sizeof(double));
	fbank->ynm2 =  (double*) malloc(nfilters * sizeof(double));
	fbank->freq =  (double*) malloc(nfilters * sizeof(double));
    /* all or nothing */
    /* but NB: memory leak if some filters are allocated but not all */
	if(fbank->b0a0 && fbank->b1a0 && fbank->b2a0 && fbank->a1a0 
		&& fbank->xnm1 && fbank->ynm1 && fbank->ynm2 && fbank->freq)
		return 0;

	return 1;

}

void rbj_filterbank_init(RBJ_FILTERBANK* fbank,double* freqs,double Q,double srate)
{
	int i, nfilters = fbank->nfilters;
	double alpha,omega,sinw ,cosw;
	double a0,a1,a2,b0,b1,b2;
	
	fbank->omegafac = TWOPI / srate;
	for(i=0;i < nfilters; i++){
		omega = fbank->omegafac * freqs[i];
		sinw = sin(omega);
		cosw = cos(omega);
		alpha = sinw / (2.0 * Q);
		/* these are the same for each filter - don't need to send them to update func */
		a0 =   1.0 + alpha;
		a1 =  -2.0 * cosw;
		a2 =   1.0 - alpha;
		/* BP coeffs */
		b0 =    alpha;
		b1 =   0;
		b2 =   -alpha;			
		fbank->a1a0[i] = a1 / a0;
		fbank->a2a0[i] = a2 / a0;
		fbank->b0a0[i] = b0 / a0;
		fbank->b1a0[i] = b1 / a0;
		fbank->b2a0[i] = b2 / a0;
		fbank->freq[i] = freqs[i];
		fbank->Q    = Q;
		fbank->xnm1[i] = fbank->xnm2[i] = fbank->ynm1[i] = fbank->ynm2[i] = 0.0;		
	}
}

void filterbank_initfreqs(int nfilters, double* freqs,double topfreq,double warp)
{
	int i;

	for(i = 1; i < nfilters; i++){
		double harmfreq,warpfreq;
		harmfreq = topfreq /(i+1);
		warpfreq =  topfreq - ((topfreq - harmfreq)  * warp);		
		freqs[i] = warpfreq;
	}
}

/* return the nuber of active filters for this freq setting */
int filterbank_updatefreqs(int maxfilters,double* freqs,double botfreq,double topfreq,double warp,double semitone)
{
	
	int k, nfilters = maxfilters;
	/* always at least one filter */
	freqs[0] = topfreq;	
	for(k=1; k < maxfilters; k++){
		double harmfreq,warpfreq;
		harmfreq = topfreq /(k+1);
		warpfreq =  topfreq - ((topfreq - harmfreq)  * warp);
		if(warpfreq < botfreq)	{						
			nfilters = k;
			break;
		}
        /* not point in filters very close together! */
		if(freqs[k-1] / warpfreq < semitone){
			nfilters = k;			
			break;		
		}
		freqs[k] = warpfreq;				
	}
	return nfilters;
}

double rbj_filterbank_tick(RBJ_FILTERBANK* fbank,double* freqs,int nfilters, double input, double Q)
{
	double alpha,omega,sinw,cosw;
	double a0,a1,a2,b0,b1,b2;
	double diva0,output = 0.0;
	int i;

	for(i=0;i < nfilters; i++ ){
		double foutput = 0.0;
        if((fbank->Q != Q) || (fbank->freq[i] != freqs[i])){
			fbank->freq[i] = freqs[i];	
			omega = freqs[i] * fbank->omegafac;
 			sinw = sin(omega);
			cosw = cos(omega);
			alpha = sinw/(2.0 * Q);
			a0 =   1.0 + alpha;
			a1 =  -2.0 * cosw;
			a2 =   1.0 - alpha;
			b0 =   alpha;
			b1 =   0.0;
			b2 =   -alpha;

			diva0 =  1.0 / a0;
			fbank->a1a0[i] = a1 * diva0;
			fbank->a2a0[i] = a2 * diva0;
			fbank->b0a0[i] = b0 * diva0;
			fbank->b1a0[i] = b1 * diva0;
			fbank->b2a0[i] = b2 * diva0;		   
		}
	    
		/*
		y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
		                    - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]
		*/
		foutput +=  fbank->b0a0[i] * input + fbank->b1a0[i] * fbank->xnm1[i] + fbank->b2a0[i] * fbank->xnm2[i]
			- fbank->a1a0[i] 	* fbank->ynm1[i] - fbank->a2a0[i] * fbank->ynm2[i];
		fbank->ynm2[i] = fbank->ynm1[i];
		fbank->ynm1[i] = foutput;
		fbank->xnm2[i] = fbank->xnm1[i];
		fbank->xnm1[i] = input;
		output += foutput;
	}
    fbank->Q = Q;
	return output;
}


void rbj_filterbank_free(RBJ_FILTERBANK* fbank)
{
	if(fbank){
		if(fbank->a1a0)
			free(fbank->a1a0);
		if(fbank->a2a0)
			free(fbank->a2a0);
		if(fbank->b0a0)
			free(fbank->b0a0);
		if(fbank->b1a0)
			free(fbank->b1a0);
		if(fbank->b2a0)
			free(fbank->b2a0);
		if(fbank->freq)
			free(fbank->freq);
		if(fbank->xnm1)
			free(fbank->xnm1);
		if(fbank->xnm2)
			free(fbank->xnm2);
		if(fbank->ynm2)
			free(fbank->ynm2);
        if(fbank->freq)
            free(fbank->freq);
	}
}