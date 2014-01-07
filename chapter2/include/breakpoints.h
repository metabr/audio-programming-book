#include <stdio.h>

#define NPOINTS (64)
#define LINELENGTH (80)

typedef struct breakpoint {
	double time;
	double value;
} BREAKPOINT;

BREAKPOINT maxpoint(const BREAKPOINT * points, long npoints);
BREAKPOINT *get_breakpoints(FILE * fp, unsigned long *psize);
int inrange(const BREAKPOINT * points, double minval, double maxval,
	    unsigned long npoints);
double val_at_brktime(const BREAKPOINT * points, unsigned long npoints,
		      double time);
