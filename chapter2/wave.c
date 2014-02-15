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

double cosinetick(OSCIL *p_osc, double freq)
{
	double val;

	val = cos(p_osc->cur_phase);
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

double squaretick(OSCIL *p_osc, double freq)
{
	double val;

	if (p_osc->cur_freq != freq) {
		p_osc->cur_freq = freq;
		p_osc->incr = p_osc->twopi_ovr_srate * freq;
	}

	if (p_osc->cur_phase <= M_PI)
		val = 1.0;
	else
		val = -1.0;

	p_osc->cur_phase += p_osc->incr;
	if (p_osc->cur_phase >= TWOPI)
		p_osc->cur_phase -= TWOPI;
	if (p_osc->cur_phase < 0.0)
		p_osc->cur_phase += TWOPI;

	return val;
}

double sawdowntick(OSCIL *p_osc, double freq)
{
	double val;

	if (p_osc->cur_freq != freq) {
		p_osc->cur_freq = freq;
		p_osc->incr = p_osc->twopi_ovr_srate * freq;
	}

	val = 1.0 - 2.0 * (p_osc->cur_phase * (1.0 / TWOPI));

	p_osc->cur_phase += p_osc->incr;
	if (p_osc->cur_phase >= TWOPI)
		p_osc->cur_phase -= TWOPI;
	if (p_osc->cur_phase < 0.0)
		p_osc->cur_phase += TWOPI;

	return val;
}

double sawuptick(OSCIL *p_osc, double freq)
{
	double val;

	if (p_osc->cur_freq != freq) {
		p_osc->cur_freq = freq;
		p_osc->incr = p_osc->twopi_ovr_srate * freq;
	}

	val = (2.0 * (p_osc->cur_phase * (1.0 / TWOPI))) - 1.0;

	p_osc->cur_phase += p_osc->incr;
	if (p_osc->cur_phase >= TWOPI)
		p_osc->cur_phase -= TWOPI;
	if (p_osc->cur_phase < 0.0)
		p_osc->cur_phase += TWOPI;

	return val;
}

double tritick(OSCIL *p_osc, double freq)
{
	double val;

	if (p_osc->cur_freq != freq) {
		p_osc->cur_freq = freq;
		p_osc->incr = p_osc->twopi_ovr_srate * freq;
	}

	/* rectified saw */
	val = (2.0 * (p_osc->cur_phase * (1.0 / TWOPI))) - 1.0;
	if (val < 0.0)
		val = -val;
	val = 2.0 * (val - 0.5);

	p_osc->cur_phase += p_osc->incr;
	if (p_osc->cur_phase >= TWOPI)
		p_osc->cur_phase -= TWOPI;
	if (p_osc->cur_phase < 0.0)
		p_osc->cur_phase += TWOPI;

	return val;
}
