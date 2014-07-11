#include "snd_def.h"

float line_env(float pos1, float dur, float pos2, int *cnt, float crate)
{
	int durs = (int)(dur * crate);
	if ((*cnt)++ < durs)
		return pos1 + *cnt * (pos2 - pos1) / durs;
	else
		return pos2;
}

float exp_env(float pos1, float dur, float pos2, int *cnt, float crate)
{
	int durs = (int)(dur * crate);
	if ((*cnt)++ < durs)
		return (float)(pos1 * pow((double)pos2 / pos1, (double) *cnt / durs));
	else
		return pos2;
}

float interp_env(float pos1, float dur, float pos2, double alpha, int *cnt, float crate)
{
	int durs = (int)(dur * crate);
	if ((*cnt)++ < durs)
		return (float) (pos1 + (pos2 - pos1) * pow((double) *cnt / durs, alpha));
	else
		return pos2;
}

float adsr(float maxamp, float dur, float at, float dt, float sus, float rt, int *cnt, float crate)
{
	float a;
	at *= crate;
	dt *= crate;
	rt *= crate;
	dur *= crate;

	if (*cnt < dur) {
		/* attack */
		if (*cnt < at)
			a = *cnt * (maxamp / at);
		/* decay */
		else if (*cnt <= (at + dt))
			a = ((sus - maxamp) / dt) * (*cnt - at) + maxamp;
		/* sustain */
		else if (*cnt <= (dur - rt))
			a = sus;
		/* release */
		else if (*cnt > (dur - rt))
			a = -(sus / rt) * (*cnt - (dur - rt)) + sus;
	} else {
		a = 0.0;
	}

	/* update time counter */
	(*cnt)++;
	return a;
}
