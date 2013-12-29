#include <stdio.h>

void triad(float basefreq, char *waveform, int *vcount, float amp);

main(int argc, char **argv)
{
  int vname_count = 1;
  triad(440.0, "TRIANGLE", &vname_count, 2000.0);
  triad(220.0, "TRIANGLE", &vname_count, 2000.0);
  triad(110.0, "TRIANGLE", &vname_count, 2000.0);
}


void triad(float basefreq, char *waveform, int *vcount, float amp)
{
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq, waveform);
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq * 1.25, waveform);
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq * 1.5, waveform);
  printf ("MIXOUT ga%d+ga%d+ga%d %f\n",*vcount-3,*vcount-2,*vcount-1, amp);
}
