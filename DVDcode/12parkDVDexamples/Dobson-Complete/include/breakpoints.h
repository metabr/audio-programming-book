/* breakpoint.h */
#ifndef __BREAKPOINTS_H_INCLUDED
#define __BREAKPOINTS_H_INCLUDED

#include <stdio.h>

typedef struct breakpoint {
		double time;
		double value;
} BREAKPOINT;

BREAKPOINT	maxpoint(const BREAKPOINT* points,unsigned long npoints);
BREAKPOINT	minpoint(const BREAKPOINT* points,unsigned long npoints);
int			inrange(const BREAKPOINT* points,double minval,double maxval,unsigned long npoints);
double		val_at_brktime(const BREAKPOINT* points, unsigned long npoints, double time);
BREAKPOINT* get_breakpoints(FILE* fp, unsigned long* psize);

/* breakpoint stream handling */

typedef struct breakpoint_stream {
	BREAKPOINT*		points;
	BREAKPOINT		leftpoint,rightpoint;
	unsigned long	npoints;
	double			curpos;
	double			incr;
	double			width;
	double			height;
	unsigned long   ileft,iright;
	int				more_points;
} BRKSTREAM;

/* srate cannot be 0; size pointer is optional - can be NULL */
BRKSTREAM*	bps_newstream(FILE* fp,unsigned long srate,unsigned long* size);
void		bps_freepoints(BRKSTREAM* stream);
double		bps_tick(BRKSTREAM* stream);		 /* NB: no error-checking, caller must ensure stream is valid */
void		bps_rewind(BRKSTREAM* stream); 
int			bps_inrange(BRKSTREAM* stream,double minval,double maxval);
int			bps_getminmax(BRKSTREAM* stream,double *outmin,double *outmax);

/* entirely arbitrary...*/
#define NPOINTS (64)
#define LINELENGTH (80)


#endif
