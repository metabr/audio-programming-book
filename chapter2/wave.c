#include <math.h>
#include <wave.h>

OSCIL* new_oscil(unsigned long srate)
{
	OSCIL *p_osc;

	p_osc = malloc(sizeof(*p_osc));
	if (p_osc == NULL)
		return NULL;

	p_osc->twopi_ovr_srate = TWOPI / (double) srate;
	p_osc->cur_freq = 0.0;
	p_osc->cur_phase = 0.0;
	p_osc->incr = 0.0;

	return p_osc;
}

double sinetick(OSCIL *p_osc, double freq)
{
	double val;

	val = sin(p_osc->cur_phase);
	if (p_osc->cur_freq != freq) {
		p_osc->cur_freq = freq;
		p_osc->incr = p_osc->twopi_ovr_srate * freq;
	}
	p_osc->cur_phase += p_osc->incr;
	if (p_osc->cur_phase >= TWOPI)
		p_osc->cur_phase -= TWOPI;
	if (p_osc->cur_phase < 0.0)
		p_osc->cur_phase += TWOPI;

	return val;
}
