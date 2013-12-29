/* we need stdio.h to get access to the C function printf().\ */

#include <stdio.h>

/* Here is our function prototype */

void triad(float basefreq, char *waveform);

main(int argc, char **argv)
{
	triad(440.0, "TRIANGLE");
}

void triad(float basefreq, char *waveform)
{
	printf ("OSC ga1 %.4f %s NONE NONE -1 1\n",basefreq, waveform);
	printf ("OSC ga2 %.4f %s NONE NONE -1 1\n",basefreq*1.25, waveform);
	printf ("OSC ga3 %.4f %s NONE NONE -1 1\n",basefreq*1.5, waveform);
	printf ("MIXOUT ga1+ga2+ga3 3000\n");
}
