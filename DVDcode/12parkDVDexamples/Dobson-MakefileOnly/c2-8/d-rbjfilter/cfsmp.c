#include <math.h>
#include "cfsamp.h"

/****************************************************************************

Amplitude and phase calculation of IIR equation
run at sample rate "sampleRate" at frequency "F". 

AMPLITUDE                                PHASE
-----------                              -----------
cf_mag(F,sampleRate,                     cf_phi(F,sampleRate,      
       a0,a1,a2,a3,a4,a5,                       a0,a1,a2,a3,a4,a5, 
       b0,b1,b2,b3,b4,b5)                       b0,b1,b2,b3,b4,b5) 
-----------                              -----------


If you need a frequency diagram, draw a plot for F=0...sampleRate/2

If you need amplitude in dB, use cf_lin2db(cf_mag(.......))


Set b0=-1 if you have such function:

y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] + a3*x[n-3] + a4*x[n-4] + a5*x[n-5] + 
               + b1*y[n-1] + b2*y[n-2] + b3*y[n-3] + b4*y[n-4] + b5*y[n-5];

Set b0=1  if you have such function:

y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] + a3*x[n-3] + a4*x[n-4] + a5*x[n-5] + 
               - b1*y[n-1] - b2*y[n-2] - b3*y[n-3] - b4*y[n-4] - b5*y[n-5];


Do not try to reverse engineer these formula - they don't give any sense
other than they are derived from transfer function, and they work.


Copyright (C)2002 by George Yohng

****************************************************************************/

#define C_PI 3.14159265358979323846264

double cf_mag(double f,double rate,
              double a0,double a1,double a2 /*,double a3,double a4,double a5*/,
              double b0,double b1,double b2 /*,double b3,double b4,double b5*/)
{

#ifdef NOTDEF

	double a3,a4,a5,b3,b4,b5;
	a3 = a4 =a5 =b3 = b4 = b5 = 0.0;
    return
        sqrt((a0*a0 + a1*a1 + a2*a2 + a3*a3 + a4*a4 + a5*a5 + 
              2*(a0*a1 + a1*a2 + a2*a3 + a3*a4 + a4*a5)*cos((2*f*C_PI)/rate) + 
              2*(a0*a2 + a1*a3 + a2*a4 + a3*a5)*cos((4*f*C_PI)/rate) +
              2*a0*a3*cos((6*f*C_PI)/rate) + 2*a1*a4*cos((6*f*C_PI)/rate) +
              2*a2*a5*cos((6*f*C_PI)/rate) + 2*a0*a4*cos((8*f*C_PI)/rate) + 
              2*a1*a5*cos((8*f*C_PI)/rate) + 2*a0*a5*cos((10*f*C_PI)/rate))/
              (b0*b0 + b1*b1 + b2*b2 + b3*b3 + b4*b4 + b5*b5 + 
              2*(b0*b1 + b1*b2 + b2*b3 + b3*b4 + b4*b5)*cos((2*f*C_PI)/rate) + 
              2*(b0*b2 + b1*b3 + b2*b4 + b3*b5)*cos((4*f*C_PI)/rate) +
              2*b0*b3*cos((6*f*C_PI)/rate) + 2*b1*b4*cos((6*f*C_PI)/rate) +
              2*b2*b5*cos((6*f*C_PI)/rate) + 2*b0*b4*cos((8*f*C_PI)/rate) + 
              2*b1*b5*cos((8*f*C_PI)/rate) + 2*b0*b5*cos((10*f*C_PI)/rate)));
#else
	return
        sqrt((a0*a0 + a1*a1 + a2*a2 +  
              2*(a0*a1 + a1*a2 )*cos((2*f*C_PI)/rate) + 
              2*(a0*a2) * cos((4*f*C_PI)/rate)              
			   )/
              (b0*b0 + b1*b1 + b2*b2 + 
              2*(b0*b1 + b1*b2 )*cos((2*f*C_PI)/rate) + 
              2*(b0*b2) * cos((4*f*C_PI)/rate)));
#endif
}


double cf_phi(double f,double rate,
              double a0,double a1,double a2 /*,double a3,double a4,double a5*/,
              double b0,double b1,double b2 /*,double b3,double b4,double b5*/)
{
#ifdef NOTDEF
	double a3,a4,a5,b3,b4,b5;
	a3 = a4 =a5 =b3 = b4 = b5 = 0.0;

	return 
        atan2((a0*b0 + a1*b1 + a2*b2 + a3*b3 + a4*b4 + a5*b5 + 
              (a0*b1 + a1*(b0 + b2) + a2*(b1 + b3) + a5*b4 + a3*(b2 + b4) +
              a4*(b3 + b5))*cos((2*f*C_PI)/rate) +
              ((a0 + a4)*b2 + (a1 + a5)*b3 + a2*(b0 + b4) +
              a3*(b1 + b5))*cos((4*f*C_PI)/rate) + a3*b0*cos((6*f*C_PI)/rate) +
              a4*b1*cos((6*f*C_PI)/rate) + a5*b2*cos((6*f*C_PI)/rate) + 
              a0*b3*cos((6*f*C_PI)/rate) + a1*b4*cos((6*f*C_PI)/rate) +
              a2*b5*cos((6*f*C_PI)/rate) + a4*b0*cos((8*f*C_PI)/rate) +
              a5*b1*cos((8*f*C_PI)/rate) + a0*b4*cos((8*f*C_PI)/rate) + 
              a1*b5*cos((8*f*C_PI)/rate) +
              (a5*b0 + a0*b5)*cos((10*f*C_PI)/rate))  
			  /
              (b0*b0 + b1*b1 + b2*b2 + b3*b3 + b4*b4 + b5*b5 + 
              2*((b0*b1 + b1*b2 + b3*(b2 + b4) + b4*b5)*cos((2*f*C_PI)/rate) + 
              (b2*(b0 + b4) + b3*(b1 + b5))*cos((4*f*C_PI)/rate) +
              (b0*b3 + b1*b4 + b2*b5)*cos((6*f*C_PI)/rate) + 
              (b0*b4 + b1*b5)*cos((8*f*C_PI)/rate) +
              b0*b5*cos((10*f*C_PI)/rate)))
			  ,
             ((a1*b0 + a3*b0 + a5*b0 - a0*b1 + a2*b1 + a4*b1 - a1*b2 +
              a3*b2 + a5*b2 - a0*b3 - a2*b3 + a4*b3 - 
              a1*b4 - a3*b4 + a5*b4 - a0*b5 - a2*b5 - a4*b5 + 
              2*(a3*b1 + a5*b1 - a0*b2 + a4*(b0 + b2) - a1*b3 + a5*b3 +
              a2*(b0 - b4) - a0*b4 - a1*b5 - a3*b5)*cos((2*f*C_PI)/rate) +
              2*(a3*b0 + a4*b1 + a5*(b0 + b2) - a0*b3 - a1*b4 - a0*b5 - a2*b5)*
              cos((4*f*C_PI)/rate) + 2*a4*b0*cos((6*f*C_PI)/rate) +
              2*a5*b1*cos((6*f*C_PI)/rate) - 2*a0*b4*cos((6*f*C_PI)/rate) -
              2*a1*b5*cos((6*f*C_PI)/rate) + 2*a5*b0*cos((8*f*C_PI)/rate) - 
              2*a0*b5*cos((8*f*C_PI)/rate))*sin((2*f*C_PI)/rate))
			  /
              (b0*b0 + b1*b1 + b2*b2 + b3*b3 + b4*b4 + b5*b5 + 
              2*(b0*b1 + b1*b2 + b2*b3 + b3*b4 + b4*b5)*cos((2*f*C_PI)/rate) + 
              2*(b0*b2 + b1*b3 + b2*b4 + b3*b5)*cos((4*f*C_PI)/rate) +
              2*b0*b3*cos((6*f*C_PI)/rate) + 2*b1*b4*cos((6*f*C_PI)/rate) +
              2*b2*b5*cos((6*f*C_PI)/rate) + 2*b0*b4*cos((8*f*C_PI)/rate) + 
              2*b1*b5*cos((8*f*C_PI)/rate) + 2*b0*b5*cos((10*f*C_PI)/rate)));
#else
	return 
        atan2((a0*b0 + a1*b1 + a2*b2  + 
              (a0*b1 + a1*(b0 + b2) + a2*(b1)) *cos((2*f*C_PI)/rate) +
              ((a0) * b2  + a2*b0) * cos((4*f*C_PI)/rate))  
			  /
              (b0*b0 + b1*b1 + b2*b2  + 
              2 * ((b0*b1 + b1*b2 )*cos((2*f*C_PI)/rate) + 
              (b2*b0 ) * cos((4*f*C_PI)/rate))),
              ((a1*b0  - a0*b1 + a2*b1 - a1*b2	+ 
              2*(-a0*b2 + a2*b0 )*cos((2*f*C_PI)/rate)) * sin((2*f*C_PI)/rate))
			  /	(b0*b0 + b1*b1 + b2*b2  + 
              2*(b0*b1 + b1*b2 )*cos((2*f*C_PI)/rate) + 
              2*(b0*b2) *cos((4*f*C_PI)/rate)));
#endif
}

double cf_lin2db(double lin)
{
    if (lin<9e-51) return -1000; /* prevent invalid operation */
    return 20*log10(lin);
}

