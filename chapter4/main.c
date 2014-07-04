#include "waveform.h"
#include "portsf.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

double duration = 3.0;
double speed = 440.0;
char *output = NULL;
char *wavename = NULL;
int formula = -1;
enum ARG_RESE { WAVEFORM_GIVEN, FORMULA_GIVEN };
typedef enum ARG_RESE ARG_RES;

ARG_RES decode_arguments(int argc, char **argv)
{
	int i = 1;
	char *a = argv[0];
	while (--argc) {
		a = argv[i];
		if('-' == a[0]) {
			switch (a[1]) {
			case 'w': /* waveform */
				if (0 == argc) {
					printf("No waveform argument\n");
					exit(1);
				}
				wavename = argv[i+1];
				argc--;
				i += 2;
				break;
			case 'f': /* formula name */
				if (0 == argc) {
					printf("No waveform argument\n");
					exit(1);
				}
				if (strcmp(argv[i+1], "sine") == 0) {
					formula = SINE;
				} else if (strcmp(argv[i+1], "cosine") == 0) {
					formula = COSINE;
				} else {
					printf("No valid formula name\n");
					exit(1);
				}
				argc--;
				i += 2;
				break;
			case 's': /* speed */
				if (0 == argc) {
					printf("No speed given\n");
					exit(1);
				}
				speed = atof(argv[i+1]);
				argc--;
				i += 2;
				break;
			case 't':
				if (0 == argc) {
					printf("No duration given\n");
					exit(1);
				}
				duration = atof(argv[i+1]);
				argc--;
				i += 2;
				break;
			case 'o':
				if (0 == argc) {
					printf("No output argument\n");
					exit(1);
				}
				output = argv[i+1];
				argc--;
				i += 2;
				break;
			}
		}
		else {
			printf("No tag found\n");
			exit(1);
		}
	}

	if (NULL == output) {
		printf("No output given\n");
		exit(1);
	}

	if (NULL == wavename && -1 == formula) {
		printf("Neither waveform nor formula specified\n");
		exit(1);
	}

	if (NULL != wavename && -1 != formula) {
		printf("Both waveform and formula specified\n");
		exit(1);
	}

	if (NULL == wavename) {
		return FORMULA_GIVEN;
	} else {
		return WAVEFORM_GIVEN;
	}
}

int main(int argc, char **argv)
{
	int format = decode_arguments(argc, argv);
	OUTPUT ff;
	WAVEFORM xx;
	psf_init();
	if (WAVEFORM_GIVEN == format) {
		xx = readform(wavename);
	} else {
		xx = createformula(formula);
	}

	ff = initialize_sound(output, sr);
	copywaveform(xx, speed, ff, duration);
	close_sound(ff);
	return 0;
}
