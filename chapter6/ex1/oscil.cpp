float osc(float *output, float amp, float freq, float *table, float *index, int length, int vsize, float srate)
{
	float incr = freq * length / srate;

	/* processing loop */
	for (int i = 0; i < vsize; i++) {
		output[i] = amp * table[(int)(*index)];
		*index += incr;
		while (*index >= length) {
			*index -= length;
		}
	}

	return *output;
}

float osci(float *output, float amp, float freq, float *table, float *index, float phase, int length, int vsize, float srate)
{
	float incr = freq * length / srate;
	float frac, pos, a, b;
	if (phase < 0) {
		phase++;
	}
	int offset = (int)(phase * length) % length;

	/* processing loop */
	for (int i = 0; i < vsize; i++) {
		pos = *index + offset;

		/* linear interpolation */
		frac = pos - (int)pos;
		a = table[(int)pos];
		b = table[(int)pos + 1];
		output[i] = amp * (a + frac * (b - a));
		*index += incr;
		while (*index >= length) {
			*index -= length;
		}
		while (*index < 0) {
			*index += length;
		}
	}

	return *output;
}

float oscc(float *output, float amp, float freq, float *table, float *index, float phase, int length, int vsize, float srate)
{
	float incr = freq * length / srate;
	float frac, fracsq, fracb;
	float pos, a, b, c, d, tmp;
	if (phase < 0) {
		phase++;
	}
	int offset = (int)(phase * length) % length;

	/* processing loop */
	for (int i = 0; i < vsize; i++) {
		pos = *index + offset;

		/* cubic interpolation */
		frac = pos - (int)pos;
		if ((int)pos > 0) {
			a = table[(int)pos - 1];
		} else {
			a = table[length - 1];
		}
		b = table[(int)pos];
		c = table[(int)pos + 1];
		d = table[(int)pos + 2];
		
		tmp = d + 3.0 * b;
		fracsq = frac * frac;
		fracb = frac*fracsq;

		output[i] = amp * 
			(fracb * (-a - 3.0 * c + tmp) / 6.0 +
			 fracsq * ((a + c) / 2.0 - b) +
			 frac * (c + (-2.0 * a - tmp)/6.0) +
			 b);

		*index += incr;

		while (*index >= length)
			*index -= length;
		while (*index < 0)
			*index += length;
	}

	return *output;
}
