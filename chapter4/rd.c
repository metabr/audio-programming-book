#include "waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

WAVEFORM getnewform(int);
int readfile(WAVEFORM, FILE_HANDLE);
void put_val(WAVEFORM, int, double);

enum ERRORS { ERR_NO_FILE=100,
	ERR_NO_COUNT,
	ERR_NO_DATA,
	ERR_NO_FORMULA,
	ERR_NO_MEMORY
};

void errormsg(char *format, enum ERRORS err)
{
	fprintf(stderr, format, (int)err);
	switch (err) {
	case ERR_NO_FILE:
		fprintf(stderr, "\nfile not found\n");
		return;
	case ERR_NO_COUNT:
		fprintf(stderr, "\nno count in file\n");
		return;
	case ERR_NO_DATA:
		fprintf(stderr, "\ndata malformed\n");
		return;
	case ERR_NO_FORMULA:
		fprintf(stderr, "\nnot a valid formula\n");
		return;
	case ERR_NO_MEMORY:
		return;
	}
}

int lengthoffile(FILE_HANDLE file)
{
	FILE *ff = fopen(file, "r");
	int count;
	if (NULL == ff) {
		return 0;
	}
	if (1 != fscanf(ff, "%d", &count)) {
		fclose(ff);
		return 0;
	}
	fclose(ff);
	return count;
}

WAVEFORM readform(FILE_HANDLE file)
{
	WAVEFORM xx;
	int err;
	xx = getnewform(lengthoffile(file));
	if ( 0 == (err = readfile(xx, file))) {
		return xx;
	}
	errormsg("Failed to read wave form for reason %d", err);
	return NULL;
}

int readfile(WAVEFORM xx, FILE_HANDLE file)
{
	int count, i;
	double val;
	FILE *ff = fopen(file, "r");

	if (NULL == ff) {
		return ERR_NO_FILE;
	}
	if (1 != fscanf(ff, "%d", &count)) {
		return ERR_NO_COUNT;
	}
	for (i = 0; i < count; i++) {
		if (1 != fscanf(ff, "%lf", &val)) {
			return ERR_NO_DATA;
		}
		put_val(xx, i, val);
	}
	fclose(ff);
	return 0;
}

#define TRIG_SIZE (4096)
//#define TWOPIBYSIZE (M_2_PI/(double)TRIG_SIZE)
#define TWOPIBYSIZE (6.28318530718/(double)TRIG_SIZE)
WAVEFORM createformula(FORMULA trig)
{
	WAVEFORM xx = getnewform(TRIG_SIZE);
	int i;
	double val;

	for (i = 0; i < TRIG_SIZE; i++) {
		if (SINE == trig) {
			val = sin((double)i * TWOPIBYSIZE);
		} else if (COSINE == trig) {
			val = cos((double)i * TWOPIBYSIZE);
		} else {
			return NULL;
		}
		put_val(xx, i, val);
	}
	return xx;
}

WAVEFORM getnewform(int size)
{
	WAVEFORM xx = malloc((size - 1) * sizeof(double) + sizeof(WAVE));
	if (NULL == xx) {
		errormsg("Failed to allocate memory\n", ERR_NO_MEMORY);
		exit(1);
	}
	xx->size = size;
	return xx;
}

void put_val(WAVEFORM xx, int i, double val)
{
	xx->samples[i] = val;
}
