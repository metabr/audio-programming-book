#include <stdlib.h>
#include <fxrand.h>

static double inv_randmax = 1.0 / RAND_MAX;

/* triangular distribution: used for TPDF dither */
double trirand(void)
{
	double r1,r2;

	r1 = (double) rand() *  inv_randmax;
	r2 = (double) rand() *  inv_randmax;
	return (r1 + r2) - 1.0;
}

void irand_init(IRAND* p_irand, unsigned long srate)
{
	p_irand->inv_srate = 1.0 / srate;
	/* get first two random numbers */
	p_irand->r1 = rand() * inv_randmax;
	p_irand->r2 = rand() * inv_randmax;	
	p_irand->magnitude = p_irand->r2 - p_irand->r1;
	p_irand->phase = 0.0;
}

/* tick funcs use fractional normalized phase between 0 - 1.0 */
/* NB no attempt made here to prevent freq values above Nyquist	*/
double irand_tick(IRAND* p_irand, double freq)
{
	double val;
	
	val= p_irand->r1 + (p_irand->phase * p_irand->magnitude);
	p_irand->phase += freq * p_irand->inv_srate;
	if(p_irand->phase >= 1.0){
		p_irand->phase -= 1.0;
		p_irand->r1 = p_irand->r2;
		p_irand->r2 = rand() * inv_randmax;
		p_irand->magnitude = p_irand->r2 - p_irand->r1;
	}
	return val;
}

void shrand_init(SHRAND* p_rand,unsigned long srate)
{
	p_rand->inv_srate = 1.0 / srate;
	p_rand->r1 = rand() * inv_randmax;
	p_rand->phase = 0.0;
}

double shrand_tick(SHRAND* p_rand,double freq)
{
	double val;

	val= p_rand->r1;
	p_rand->phase += freq * p_rand->inv_srate;
	if(p_rand->phase >= 1.0){
		p_rand->phase -= 1.0;
		p_rand->r1 = rand() * inv_randmax;
	}
	return val;
}
