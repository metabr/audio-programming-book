#include "snd_def.h"

float butterwoth(float *sig, float freq, float bw, float *del, int mode, int vsize, float srate)
{
	double a, a1, a2, b1, b2, tanthe, costhe, sqrtan, tansq, w;

	switch(mode) {
		case LP:
			tanthe = 1.0 / tan(M_PI * freq / srate);
			sqrtan = sqrt(2.0) * tanthe;
			tansq = tanthe * tanthe;
			a = 1.0 / (1.0 + sqrtan + tansq);
			a1 = 2.0 * a;
			a2 = a;
			b1 = 2.0 * (1.0 - tansq) * a;
			b2 = (1.0 - sqrtan + tansq) * a;
			break;
		case HP:
			tanthe = 1.0 / tan(M_PI * freq / srate);
			sqrtan = sqrt(2.0) * tanthe;
			tansq = tanthe * tanthe;
			a = 1.0 / (1.0 + sqrtan + tansq);
			a1 = -2.0 * a;
			a2 = a;
			b1 = 2.0 * (tansq - 1.0) * a;
			b2 = (1.0 - sqrtan + tansq) * a;
			break;
		case BR:
			tanthe = tan(M_PI * bw / srate);
			costhe = 2.0 * cos(M_2_PI * freq / srate);
			a = 1.0 / (1.0 + tanthe);
			a1 = -costhe * a;
			a2 = a;
			b1 = -costhe * a;
			b2 = (1.0 - tanthe) * a;
			break;
		case BP:
		default:
			tanthe = 1.0 / tan(M_PI * bw / srate);
			costhe = 2.0 / cos(M_2_PI * freq / srate);
			a = 1.0 / (1.0 + tanthe);
			a1 = 0;
			a2 = -a;
			b1 = -tanthe * costhe * a;
			b2 = (tanthe - 1.0) * a;
	}

	for (int i = 0; i < vsize; i++) {
		w = sig[i] - b1 * del[0] - b2 * del[1];
		sig[i] = (float)(a * w + a1 * del[0] + a2 * del[1]);
		del[1] = del[0];
		del[0] = (float)w;
	}

	return *sig;
}
