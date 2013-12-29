#include <stdio.h>

void triad(float basefreq, char *waveform, int *vcount, float amp);

main(int argc, char **argv)
{
  int vname_count = 1;
  
  /* vname_count will keep track of Csound a-rate variables. Since we are
  going to change this variable's value inside the function, we need to pass
  it by address, which is what the ampersand does. */
  
  triad(440.0, "TRIANGLE", &vname_count, 2000.0);
}


void triad(float basefreq, char *waveform, int *vcount, float amp)
{
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq, waveform);
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq * 1.25, waveform);
  printf ("OSC ga%d %.4f %s NONE NONE -1 1\n",(*vcount)++,basefreq * 1.5, waveform);
  
  /* This is our first use of automatic Csound variables. *vcount contains the current
	count of global audio (a-rate) variables. In the above code we have incremented the
	value of *vcount by 3. Therefore in our print statement, we access the correct numbers
	of the Csound variables by the subtractions shown below.
  */
  
  printf ("MIXOUT ga%d+ga%d+ga%d %f\n",*vcount-3,*vcount-2,*vcount-1, amp);
}
