#ifndef WAVEFORM_H
#define WAVEFORM_H
enum FORMULAE { SINE, COSINE };
typedef enum FORMULAE FORMULA;

typedef struct WaveForm {
	int size;
	double samples[1];
} WAVE;

typedef WAVE *WAVEFORM;

typedef char *FILE_HANDLE;

typedef double SPEED;

typedef int OUTPUT; /* For portsf library */

#define sr (44100)

WAVEFORM readform(FILE_HANDLE);
WAVEFORM createformula(FORMULA);
void copywaveform(WAVEFORM, SPEED, OUTPUT, double);

OUTPUT initialize_sound(char *name, int srate);
int close_sound(OUTPUT out);

#endif
