#include "snd_def.h"

float lowpass(float *sig, float freq, float *del, int vsize, float srate)
{
	double costh, coef;
	costh = 2.0 - cos(2 * M_PI * freq / srate);
	coef = sqrt(costh * costh - 1.0) - costh;

	for (int i = 0; i < vsize; i++) {
		sig[i] = (float)(sig[i] * (1 + coef) - *del * coef);
		*del = sig[i];
	}

	return *sig;
}

float highpass(float *sig, float freq, float *del, int vsize, float srate)
{
	double costh, coef;
	costh = 2.0 - cos(2 * M_PI * freq / srate);	
	coef = costh - sqrt(costh*costh - 1.0);

	for (int i = 0; i < vsize; i++) {
		sig[i] = (float)(sig[i] * (1 - coef) - *del * coef);
		*del = sig[i];
	}

	return *sig;
}

float resonator(float *sig, float freq, float bw, float *del, int vsize, float srate)
{
	double r, rsq, rr, costh, a, w;
	r = 1.0 - M_PI * (bw / srate);
	rr = 2 * r;
	rsq = r * r;
	costh = (rr / (1.0 + rsq)) * cos(2 * M_PI * freq / srate);
	a = (1 - rsq) * sin(acos(costh));

	for (int i = 0; i < vsize; i++) {
		sig[i] = (float)(sig[i] * a + rr * costh * del[0] - rsq * del[1]);
		del[1] = del[0];
		del[0] = sig[i];
	}

	return *sig;
}

float bandpass(float *sig, float freq, float bw, float *del, int vsize, float srate)
{
	double r, rsq, rr, costh, a, w;
	r = 1.0 - M_PI * (bw / srate);
	rr = 2 * r;
	rsq = r * r;
	costh = (rr / (1.0 + rsq)) * cos(2 * M_PI * freq / srate);
	a = (1 - r);

	for (int i = 0; i < vsize; i++) {
		w = a * sig[i] + rr * costh * del[0] - rsq * del[1];
		sig[i] = (float)(w - r * del[1]);
		del[1] = del[0];
		del[0] = (float)w;
	}

	return *sig;
}

float balance(float *sig, float *cmp, float *del, float freq, int vsize, float srate)
{
	double costh, coef;
	costh = 2.0 - cos(2 * M_PI * freq / srate);
	coef = sqrt(costh * costh - 1.0) - costh;

	for (int i = 0; i < vsize; i++) {
		del[0] = (float)(fabsf(sig[i]) * (1 + coef) - del[0] * coef);
		del[1] = (float)(fabsf(cmp[i]) * (1 + coef) - del[1] * coef);
		sig[i] *= (float)(del[0] ? del[1]/del[0] : del[1]);
	}

	return *sig;
}
