#include <stdio.h>

#define NPOINTS (64)
#define LINELENGTH (80)

typedef struct breakpoint {
                double time;
                double value;
} BREAKPOINT;

BREAKPOINT maxpoint(const BREAKPOINT* points,long npoints);
BREAKPOINT* get_breakpoints(FILE* fp, long* psize);
