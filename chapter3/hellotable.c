#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tinyAudioLib.h"

#define SAMPLING_RATE 44100
#define TABLE_LEN 512

#define SINE 0
#define SQUARE 1
#define SAW 2
#define TRIANGLE 3

float table[TABLE_LEN];

enum {ARG_PROGNAME, ARG_FREQ, ARG_DUR, ARG_WAVEFORM, ARG_NARGS};

void usage()
{
	printf("Usage: hellotable freq dur waveform\n"
			"\twhere waveform:\n"
			"\t0 = sine,\n"
			"\t1 = square,\n"
			"\t2 = saw,\n"
			"\t3 = triangle.\n");
}

void fill_sine()
{
	for (int i = 0; i < TABLE_LEN; i++)
		table[i] = (float) sin(2 * M_PI * i / TABLE_LEN);
}

void fill_square()
{
	for (int i = 0; i < TABLE_LEN / 2; i++)
		table[i] = 1;
	for (int i = TABLE_LEN / 2; i < TABLE_LEN; i++)
		table[i] = -1;
}

void fill_saw()
{
	for (int i = 0; i < TABLE_LEN; i++)
		table[i] = 1 - (2 * (float) i / (float) TABLE_LEN);
}

void fill_triangle()
{
	for (int i = 0; i < TABLE_LEN / 2; i++)
		table[i] = 2 * (float) i / (float) (TABLE_LEN / 2) - 1;
	for (int i = TABLE_LEN / 2; i < TABLE_LEN; i++)
		table[i] = 1 - (2 * (float) i / (float) (TABLE_LEN / 2));
}

int main(int argc, char *argv[])
{
	float frequency, duration;
	int waveform;

	if (argc != ARG_NARGS) {
		usage();
		return 1;
	}

	frequency = atof(argv[ARG_FREQ]);
	duration = atof(argv[ARG_DUR]);

	waveform = atoi(argv[ARG_WAVEFORM]);
	if (waveform < 0 || waveform > 3) {
		printf("Error, waveform should be between 0 and 3.\n\n");
		usage();
		return 1;
	}

	// filling the table
	switch (waveform) {
	case SINE:
		fill_sine();
		break;
	case SQUARE:
		fill_square();
		break;
	case SAW:
		fill_saw();
		break;
	case TRIANGLE:
		fill_triangle();
		break;
	default:
		printf("Error, waveform not defined.\n\n");
		usage();
		return 1;
	}

	double sample_increment = frequency * TABLE_LEN / SAMPLING_RATE;
	double phase = 0;
	float sample;

	for (int i = 0; i < duration * SAMPLING_RATE; i++) {
		sample = table[(long) phase];
		outSampleMono(sample * 0.9);
		phase += sample_increment;
		if (phase > TABLE_LEN)
			phase -= TABLE_LEN;
	}

	// synthesis
	tinyInit();

	// cleanup
	tinyExit();

	return 0;
}
