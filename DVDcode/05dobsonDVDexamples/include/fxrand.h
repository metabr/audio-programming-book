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
