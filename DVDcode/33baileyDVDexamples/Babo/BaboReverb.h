/*
 *  BaboReverb.h
 *  Babo
 *
 *  Created by Jonathan Bailey on 7/23/09.
 *  Copyright 2009 Drumwell Group. All rights reserved.
 *
 */

#ifndef __BaboReverb_h__
#define __BaboReverb_h__

#ifndef __Size_T__
#define __Size_T__
typedef unsigned long size_t;
#endif

#ifndef PI
#define PI      (3.141592653589793238462643383279502884197)
#endif
#define TWOPI   (6.283185307179586476925286766559005768394)
#define PI_F    ((float) PI)
#define TWOPI_F ((float) TWOPI)


#define BABO_NODES	(15) /* Number of nodes of feedback delay network    */
#define BABO_TAPS	(6)  /* Number of taps in the early reflections line */



typedef struct
	{
		float	*memptr;
		float	*endptr;
		size_t  samples;
	} BaboMemory;

typedef struct
	{
		BaboMemory   core;
		float       *input;
	} BaboDelay;

typedef BaboDelay BaboTapline;

typedef struct
	{
		float   a0,
		a1,
		z1,
		z2,
		input;
	} BaboLowPass;

typedef struct
	{
		BaboDelay   delay;
		BaboLowPass filter;
	} BaboNode;

typedef struct
	{
		float    complementary_early_diffusion;
		float    fdn[BABO_NODES][BABO_NODES];
		BaboNode node[BABO_NODES] ;
	} BaboMatrix;

/*
 * The opcode functionality should be:
 *
 * ar,al babo asig,kx,ky,kz,ix,iy,iz[,idiff[,ifno]]
 *
 * where - mandatory arguments:
 *
 * kx,ky,kz     - position of the source
 * ix,iy,iz     - dimensions of the resonator (room)
 *
 *         optional arguments:
 *
 * idiff        - diffusion coefficient (a number from 0 to 1: default 1)
 * ifno         - number of the GEN 02 function which holds all the other
 *                defaulted parameters
 *
 * the optional size-8 gen 02 function may hold the following parameters:
 *
 * fn 0 8 2 decay hidecay rx ry rz rdistance direct early_diff
 *
 * where:
 *
 * decay        - global decay coefficient (a number from 0 to 1: default 0.99)
 * hidecay      - hi-freq decay coefficient (a number from 0 to 1: default 0.1)
 * rx,ry,rz     - position of the receiver (default: 0, 0, 0)
 * rdistance    - x distance between stereo receivers (e.g.: ears :)
 *                (default: 0.3)
 * direct       - direct delayed signal coefficient (default: 0.5)
 * early_diff   - diffusion coefficient of the early reflections (default: 0.8)
 *
 */

typedef struct
	{
		/* output args          */
		float       *outleft,*outright,
		/* start input args     */
		*input,
		ksource_x,ksource_y,ksource_z,
		lx,ly,lz,
		/* optional args */
		*odiffusion_coeff,
		*oexpert_values;
		/* end input args       */
		/* backward-logic optional args copy */
		float       diffusion_coeff,
		expert_values;
		/* internal values */
		float       decay, hidecay,
		receiver_x, receiver_y, receiver_z,
		inter_receiver_distance,
		direct,
		early_diffuse;
		BaboTapline tapline;                        /* early reflections    */
		BaboDelay   matrix_delay;                   /* diffusion delay      */
		BaboMatrix  matrix;                         /* diffusion            */
	} BABO;

#define BABO_DEFAULT_DIFFUSION_COEFF        (1.0)
#define BABO_DEFAULT_DECAY                  (0.99)
#define BABO_DEFAULT_HIDECAY                (0.1)
#define BABO_DEFAULT_RECV_X                 (0.0)
#define BABO_DEFAULT_RECV_Y                 (0.0)
#define BABO_DEFAULT_RECV_Z                 (0.0)
#define BABO_DEFAULT_INTER_RECV_DISTANCE    (0.3)
#define BABO_DEFAULT_DIRECT                 (0.5)
#define BABO_DEFAULT_DIFFUSE                (0.8)

void set_expert_values(BABO *p);
int baboset(BABO *p, float sr, float onedsr);
int babo(BABO *p, float sr, float onedsr, int num_samples);
float babo_reverb_time(BABO *p);


#endif
