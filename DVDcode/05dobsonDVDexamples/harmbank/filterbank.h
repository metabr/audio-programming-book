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

#ifndef __FILTERBANK_H_INCLUDED
#define __FILTERBANK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
typedef struct rbj_filterbank {
	int nfilters;
	double* b0a0;
	double* b1a0;
	double* b2a0;
	double* a1a0;
	double* a2a0;
	double* xnm1;
	double* xnm2;
	double* ynm1;
	double* ynm2;
	double* freq;
	double Q, ampscale;
	double omegafac;
} RBJ_FILTERBANK;
	
int rbj_createbank(RBJ_FILTERBANK* fbank,int nfilters);

void rbj_filterbank_init(RBJ_FILTERBANK* fbank,double* freqs,double Q,double srate);

double rbj_filterbank_tick(RBJ_FILTERBANK* fbank,double* freqs,int nfilters, double input, double Q);

void filterbank_initfreqs(int nfilters, double* freqs,double topfreq,double warp);

int filterbank_updatefreqs(int maxfilters,double* freqs,double botfreq,double topfreq,double warp,double semitone);



void rbj_filterbank_free(RBJ_FILTERBANK* fbank);

#ifdef __cplusplus
}
#endif

#endif