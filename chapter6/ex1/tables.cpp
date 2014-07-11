#include "snd_def.h"

float *line_table(int brk_points, float *points, int lenght)
{
	float start, end, incr;
	float *table = new float[lenght + 2];

	for (int n = 2; n < brk_points * 2; n += 2) {
		start = points[n - 1];
		end = points[n + 1];
		incr = (end - start) * 1.0 / (points[n] - points[n - 2]);
		for (int i = (int)points[n - 2]; i < points[n] && i < lenght + 2; i++) {
			table[i] = start;
			start += incr;
		}
	}
	normalise_table(table, lenght);
	return table;
}

float *exp_table(int brk_points, float *points, int lenght)
{
	float mult;
	float *table = new float[lenght + 2];
	double start, end;

	for (int n = 2; n < brk_points * 2; n += 2) {
		start = points[n - 1] + 0.00000001;
		mult = (float)pow(end / start, 1.0 / (points[n] - points[n - 2]));
		for (int i = 0; i < points[n] && i < lenght + 2; i++) {
			table[i] = (float)start;
			start *= mult;
		}
	}
	normalise_table(table, lenght);
	return table;
}

float *sinus_table(int lenght, float phase)
{
	float *table = new float[lenght + 2];
	phase *= M_2_PI;
	for (int n = 0; n < lenght + 2; n++) {
		table[n] = (float)cos(phase + n * 2 * M_PI / lenght);
	}

	return table;
}

float *fourier_table(int harms, float *amps, int lenght, float phase)
{
	float a;
	float *table = new float[lenght + 2];
	double w;
	phase *= M_2_PI;

	memset(table, 0, (lenght + 2) * sizeof(float));

	for (int i = 0; i < harms; i++)
		for (int n = 0; n < lenght + 2; n++) {
			a = amps ? amps[i] : 1.0;
			w = (i + 1) * (n * M_2_PI / lenght);
			table[n] += (float) (a * cos(w + phase));
		}

	normalise_table(table, lenght);
	return table;
}

float *saw_table(int harms, int lenght)
{
	float *amps = new float[harms];	
	for (int i = 0; i < harms; i++)
		amps[i] = 1.0 / (i + 1);
	float *table = fourier_table(harms, amps, lenght, -0.25);
	delete[] amps;
	return table;
}

float *sqr_table(int harms, int lenght)
{
	float *amps = new float[harms];	
	memset(amps, 0, sizeof(float) * harms);
	for (int i = 0; i < harms; i += 2)
		amps[i] = 1.0 / (i + 1);
	float *table = fourier_table(harms, amps, lenght, -0.25);
	delete[] amps;
	return table;
}

float *triangle_table(int harms, int lenght)
{
	float *amps = new float[harms];	
	memset(amps, 0, sizeof(float) * harms);
	for (int i = 0; i < harms; i += 2)
		amps[i] = 1.0 / ((i + 1) * (i + 1));
	float *table = fourier_table(harms, amps, lenght);
	delete[] amps;
	return table;
}

void normalise_table(float *table, int lenght, float scale)
{
	int n;
	float max = 0.0;
	for (n = 0; n < lenght; n++)
		max = table[n] > max ? table[n] : max;
	if (max)
		for (n = 0; n < lenght; n++)
			table[n] /= max;
}
