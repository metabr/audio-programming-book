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
/* fconv.c*/
#include <stdlib.h>
#include <memory.h>
#include "fconv.h"

CONV* new_conv( unsigned long ncoeffs,double coeffs[])
{
	CONV* conv = NULL;

	if(ncoeffs == 0)
		return NULL;
	conv = (CONV*) malloc(sizeof(CONV));
	if(conv == NULL)
		return NULL;
	conv->coeffs = (double*) malloc(ncoeffs * sizeof(double));
	if(conv->coeffs == NULL){
		free(conv);
		return NULL;
	}
	conv->past = (double*) malloc(ncoeffs * sizeof(double));
	if(conv->past == NULL){
		free(conv->coeffs);
		free(conv);
		return NULL;
	}
	memcpy(conv->coeffs,coeffs,ncoeffs * sizeof(double));
	memset(conv->past,0, ncoeffs * sizeof(double));
	conv->nm1 = ncoeffs - 1;
	conv->pindex = 0;
	return conv;
}

double conv_tick(CONV* conv,double input)
{
	unsigned long i,p;
	long pindex = conv->pindex;
	double output;
	double* coeffs = conv->coeffs;
	double* past = conv->past;
	
	past[pindex] = input;
	p = pindex;
	output = 0.0;
	for(i=0; i <= conv->nm1; i++){
		output += coeffs[i] * past[p++];
		if(p > conv->nm1)
			p = 0;
	}
	if(--pindex < 0)
		pindex = conv->nm1;
	conv->pindex = pindex;
	return output;
}

void conv_free(CONV* conv)
{
	if(conv){
		if(conv->past)
			free(conv->past);
		if(conv->coeffs)
			free(conv->coeffs);
		conv->past = conv->coeffs = NULL;
	}
}