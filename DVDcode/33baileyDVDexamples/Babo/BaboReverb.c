/*
 *  BaboReverb.c
 *  Babo
 *
 *  Created by Jonathan Bailey on 7/23/09, based on the Csound code for Babo, (C) Davide Rocchesso and Nicola Bernardini. 
 *
 */

#include "BaboReverb.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


#if !defined(FLT_MAX)
#define FLT_MAX         (1.0e38)
#endif

static const int sound_speed = 330;

/*
 * private Babo tools
 */

#define square(x)   ((x)*(x))

#ifdef never
static inline int32
bround(float x)
{
    return (int32)(x >= 0 ? x + 0.5 : x - 0.5);
}
#endif

/*
 * Memory allocation object methods
 *
 * (this object is a bit funny because it is in fact a wrapper of the
 * memory allocation functions in csound)
 */

static BaboMemory *
BaboMemory_create(BaboMemory *this, size_t size_in_floats)
{
    size_t size_in_bytes = size_in_floats * sizeof(float);
	
    this->memptr = malloc(size_in_bytes);
	
    memset(this->memptr, 0, size_in_bytes);
	
    this->samples = size_in_floats;
	this->endptr = (this->memptr + size_in_floats);
	
    return this;
}

static inline size_t
BaboMemory_samples(const BaboMemory *this)
{
    return this->samples;
}

static inline float *
BaboMemory_start(const BaboMemory *this)
{
    return (float *) this->memptr;
}

static inline float *
BaboMemory_end(const BaboMemory *this)
{
    return (float *) (this->endptr);
}

/* static inline float * */
/* BaboMemory_size(const BaboMemory *this) */
/* { */
/*     return (float *) this->memptr.size; */
/* } */

/*
 * common delay/tapline methods
 */

static void
_Babo_common_delay_create(float sr, BaboDelay *this, float max_time)
{
    size_t num_floats =
	(size_t)lrintf((float)ceil((double)(max_time*sr)));
	
    BaboMemory_create(&this->core, num_floats);
}

/*
 * Babo Delay object methods
 */

static BaboDelay *
BaboDelay_create(float sr, BaboDelay *this, float max_time)
{
    _Babo_common_delay_create(sr, this, max_time);
	
    this->input = BaboMemory_start(&this->core);
	
    return this;
}

static float
BaboDelay_input(BaboDelay *this, float input)
{
	*this->input++ = input;
	
    if (this->input >= BaboMemory_end(&this->core))
        this->input -= BaboMemory_samples(&this->core);
	
    return input;
}

static float
BaboDelay_output(const BaboDelay *this)
{
    size_t num_samples = BaboMemory_samples(&this->core);
    float *output_ptr = this->input - (num_samples - 1);
	
    if (output_ptr < BaboMemory_start(&this->core))
        output_ptr += num_samples;
	
    return *output_ptr;
}

/*
 * Babo Tapline object methods
 */

/*
 * The tapline is created with a size that is dependent on the size
 * of the room (selected at i-time)
 */





static BaboTapline *
BaboTapline_create(float sr, BaboTapline *this, float x, float y, float z)
{
    float max_time = (2.0 * sqrt((x*x) + (y*y) + (z*z))) / sound_speed;
	
    _Babo_common_delay_create(sr, (BaboDelay *) this, max_time);
	
    this->input = BaboMemory_start(&this->core);
	
    return this;
}

static inline float
BaboTapline_maxtime(float onedsr, BaboDelay *this)
{
    return (((float) BaboMemory_samples(&this->core)) * onedsr);
}

static inline float
BaboTapline_input(BaboTapline *this, float input)
{
    return BaboDelay_input((BaboDelay *) this, input);
}

typedef struct
	{
		float   attenuation;
		float   delay_size;
	} BaboTapParameter;

typedef struct
	{
		BaboTapParameter    direct;
		BaboTapParameter    tap[BABO_TAPS];
	} BaboTaplineParameters;

/*
 * BaboTapline_single_output:
 * this function calculates the delay output by doing linear interpolation
 * between the two adjacent integer sample delays of an otherwise fractional
 * delay time. This is why BaboTapParameter.delay_size is kept as a float
 * in the first place.
 */
/* a-rate function */
static float BaboTapline_single_output(const BaboTapline *this,
									   const BaboTapParameter *pp)
{
	/*
	 * the assignement right below should be really a floor(p->delay_size),
	 * but apparently floor() calls are really expensive on some
	 * architectures (notably Pentiums), so we do a simple cast to a
	 * size_t instead. This should always work, but I cannot test it on
	 * other architectures than mine (k6), so it potentially is a source
	 * of problems. [nicb@axnet.it]
	 */
    size_t delay_floor  = (size_t) pp->delay_size;
    size_t delay_ceil   = delay_floor + 1;
    float fractional    = pp->delay_size - (float) delay_floor;
    float *output_floor = this->input - delay_floor;
    float *output_ceil  = this->input - delay_ceil;
    float output        = 0.0;
	
    if (output_floor <  BaboMemory_start(&this->core))
        output_floor += BaboMemory_samples(&this->core);
	
    if (output_ceil <  BaboMemory_start(&this->core))
        output_ceil += BaboMemory_samples(&this->core);
	
    output = (*output_floor*(1-fractional)) + (*output_ceil*fractional);
	
    return output * pp->attenuation;
}

/* k-rate function */
static inline void BaboTapline_preload_parameter(float onedsr,
												 BaboTapParameter *this,	
                                                 float distance)
{
    /*
     * Direct sound parameters at the input of delay tap_lines.
     * Right and left direct_att is not really physical, but ensures that:
     *          direct_att=(1/2) when distance is 1 m
     *          direct_att=1     when distance is 0 m.
     */
    this->delay_size    = (distance / sound_speed) * onedsr;
    this->attenuation   = 1.0 / (1.0 + distance);
}

/* k-rate function */
static BaboTaplineParameters *
BaboTapline_precalculate_parameters(
									float onedsr, const BaboTapline *this, BaboTaplineParameters    *results,
									float r_x, float r_y, float r_z,    /* receiver position (i-rate) */
									float s_x, float s_y, float s_z,    /* source   position (k-rate) */
									float l_x, float l_y, float l_z)    /* room     coords   (i-rate) */
{
    float   sqr_xy, sqr_yz, sqr_xz, /* x^2+y^2  y^2+z^2 .......         */
	sqr_diff_x, sqr_diff_y, sqr_diff_z; /* optimization temps   */
	
    /* image method distance calculation */
	
    sqr_diff_x  = square(r_x - s_x);
    sqr_diff_y  = square(r_y - s_y);
    sqr_diff_z  = square(r_z - s_z);
	
    sqr_yz      = sqr_diff_y + sqr_diff_z;
    sqr_xz      = sqr_diff_x + sqr_diff_z;
    sqr_xy      = sqr_diff_x + sqr_diff_y;
	
    BaboTapline_preload_parameter(onedsr, &results->direct,
								  sqrt(sqr_diff_x + sqr_yz));
	
    BaboTapline_preload_parameter(onedsr, &results->tap[0],
                                  sqrt(square(l_x + r_x + s_x) + sqr_yz));
    BaboTapline_preload_parameter(onedsr, &results->tap[1],
                                  sqrt(square(l_x - r_x - s_x) + sqr_yz));
    BaboTapline_preload_parameter(onedsr, &results->tap[2],
								  sqrt(sqr_xz + square(l_y - r_y - s_y)));
    BaboTapline_preload_parameter(onedsr, &results->tap[3],
								  sqrt(sqr_xz + square(l_y + r_y + s_y)));
	
    BaboTapline_preload_parameter(onedsr,
                                  &results->tap[4],
                                  sqrt(sqr_xy + square(l_z - r_z - s_z)));
    BaboTapline_preload_parameter(onedsr,
                                  &results->tap[5],
                                  sqrt(sqr_xy + square(l_z + r_z + s_z)));
	
    return results;
}

/* a-rate function */
static float
BaboTapline_output(const BaboTapline *this,
				   const BaboTaplineParameters *pars)
{
    int     i;
    float   output = BaboTapline_single_output(this, &pars->direct);
	
    for (i = 0; i < BABO_TAPS; ++i)
		output  += BaboTapline_single_output(this, &pars->tap[i]);
	
    return output;
}

/*
 * Babo lowpass filter object methods
 */

static BaboLowPass *
BaboLowPass_create(BaboLowPass *this, float decay, float hidecay, float norm)
{
    float real_decay    = exp(norm * log(decay));
    float real_hidecay  = exp(norm * log(hidecay));
	
    this->a0 = (real_decay + real_hidecay) * 0.25;
    this->a1 = (real_decay - real_hidecay) * 0.5;
    this->z1 = this->z2 = 0.0;
	
    return this;
}

static inline float
BaboLowPass_input(BaboLowPass *this, float input)
{
    this->z2 = this->z1;
    this->z1 = this->input;
    this->input = input;
    return input;
}
static inline float
BaboLowPass_output(const BaboLowPass *this)
{
    return  (this->a0 * this->input)    +
	(this->a1 * this->z1)       +
	(this->a0 * this->z2);
}
/*
 * Babo node object methods
 */

static BaboNode *
BaboNode_create(float sr, BaboNode *this, float time,
				float min_time, float decay,
				float hidecay)
{
    BaboDelay_create(sr, &this->delay, time);
    BaboLowPass_create(&this->filter, decay, hidecay, time/min_time);
	
    return this;
}

static inline float
BaboNode_input(BaboNode *this, float input)
{
    return BaboDelay_input(&this->delay, input);
}

static inline void
BaboNode_feed_filter(BaboNode *this)
{
    BaboLowPass_input(&this->filter, BaboDelay_output(&this->delay));
}

static inline float
BaboNode_output(const BaboNode *this)
{
    return BaboLowPass_output(&this->filter);
}

/*
 * Babo Matrix object methods
 */

static void
BaboMatrix_create_FDN(BaboMatrix *this, float diffusion)
{
    int  i,j;
    float _2PI_NODES = TWOPI_F / BABO_NODES;
    /*
     * The following sequence of eigenvalues provides, by IDFT,
     * the maximally diffusive sequence, i.e. a row of the circulant
     * feedback matrix.
     * eigenvalues are expressed in radians, because only the argument
     * is expressed, since the magnitude is one
     */
	
    const float  max_diffusion_eigenvalues[BABO_NODES]=
    {
        3.142592,
		-1.7370,
		-2.1559,
		-1.2566,
		-2.9936,
        1.0472,
		-2.5133,
		-1.6140,
        1.6140,
        2.5133,
		-1.0472,
        2.9936,
        1.2566,
        2.1559,
        1.7370
    };
	
    /*
     * Here we multiply the arguments of the sequence of eigenvalues by
     * p->idiffusion_coeff. In this way we scale the amount of diffusion
     * Range of diffusion:   0 = no diffusion
     *                       1 = maximum diffusion
     */
	
    float  real_X[BABO_NODES]       = { 0.0 },
	imaginary_X[BABO_NODES]  = { 0.0 },
	arg_X[BABO_NODES]        = { 0.0 },
	real_x[BABO_NODES]       = { 0.0 };
	
    for (i = 0; i < BABO_NODES; ++i)
    {
        real_X[i] = imaginary_X[i] = 0.0;
        arg_X[i]  = max_diffusion_eigenvalues[i] * diffusion;
        real_X[i] = cos(arg_X[i]);
        imaginary_X[i] = sin(arg_X[i]);
    }
	
    /*
     * The Real part of the InverseDFT of the eigenvalues supplies the
     * circulant matrix coefficients.
     */
	
    for (i = 0; i < BABO_NODES; ++i)
        for (j = 0; j < BABO_NODES; ++j)
            real_x[j] += (real_X[j] * cos(_2PI_NODES*i*j)-
						  imaginary_X[j] * sin(_2PI_NODES*i*j))/BABO_NODES;
	
    for (i = 0; i < BABO_NODES; ++i)
        for (j = 0; j < BABO_NODES; ++j)
            this->fdn[i][j] = real_x[(j-i+15) % BABO_NODES];
}

static float
BaboMatrix_calculate_delays(float delay_time[], float x, float y, float z)
{
    int i = 0;
    float min = 0.0;
	
    const static struct babo_diffusion_constants
    {
        int x, y, z;
		
    } BABO_DIRECTIONS[] =
    {
        /*
         * Each triplet is a mode identifier.
         * E.g. {1,0,0} is the first axial mode
         */
        { 1, 0, 0 },
        { 2, 1, 0 },
        { 1, 1, 0 },
        { 1, 2, 0 },
        { 0, 1, 0 },
        { 0, 2, 1 },
        { 0, 1, 1 },
        { 0, 1, 2 },
        { 0, 0, 1 },
        { 1, 0, 2 },
        { 1, 0, 1 },
        { 1, 1, 1 },
        { 1, 2, 1 },
        { 2, 1, 1 },
        { 2, 0, 1 }
    };
    /*
     * we calculate the delays related to each node, in the following
     * way:
     *
     *                                   2
     * delay[i] = -----------------------------------------------
     *                      +-----------------------------------
     *    sound_speed * \  / |       |2  |       |2  |       |2
     *                   \/  |  x[i] |   |  y[i] |   |  z[i] |
     *                       | ----- | + | ----- | + | ----- |
     *                       |   X   |   |   Y   |   |   Z   |
     *
     * and we keep a notion of the minimum delay path which is
     * needed later on to do the rescaling of the decay and hidecay
     * parameters.
     */
    min = (float)FLT_MAX; /* let's initialize this with something really big */
	
    for (i = 0; i < BABO_NODES; ++i)
    {
        const struct babo_diffusion_constants *dbdp = &BABO_DIRECTIONS[i];
        delay_time[i] = 2.0 / (sound_speed *
								   sqrt(((dbdp->x/x) * (dbdp->x/x)) +
										((dbdp->y/y) * (dbdp->y/y)) +
										((dbdp->z/z) * (dbdp->z/z))));
		
        min = min > delay_time[i] ? delay_time[i] : min;
    }
    return min;
}

static BaboMatrix *
BaboMatrix_create(float sr,
				  BaboMatrix *this, float diffusion, float x, float y,
                  float z, float decay, float hidecay, float early_diffusion)
{
    int i = 0;
    float delays[BABO_NODES];
    float min_delay = BaboMatrix_calculate_delays(delays, x, y, z);
	
    this->complementary_early_diffusion = 1.0 - early_diffusion;
	
    BaboMatrix_create_FDN(this, diffusion);
	
    for (i = 0; i < BABO_NODES; ++i)
        BaboNode_create(sr, &this->node[i], delays[i],
						min_delay, decay, hidecay);
	
    return this;
}

static inline float
BaboMatrix_coefficient(const BaboMatrix *this, int x, int y)
{
    return this->fdn[x][y];
}

/* a-rate function */
static void
BaboMatrix_output(BaboMatrix *this, float outputs[], float input,
				  float diffusion_coeff)
{
    float            filter_tmpout[BABO_NODES]  = { 0.0 },
	tmp2[BABO_NODES]           = { 0.0 };
    register int     i = 0, j = 0;
	
    for (i = 0; i < BABO_NODES; ++i)
    {
        filter_tmpout[i] = BaboNode_output(&this->node[i]);
        BaboNode_feed_filter(&this->node[i]);
    }
	
    /*
     * Here the matrix-by-vector multiply takes place, xout is the
     * column vector.
     * The mod(BABO_NODES) operation allows to write the circulant matrix-
     * by-vector multiply in a compact way.
     */
	
    for (i = 0; i < BABO_NODES; ++i)
    {
        for (j = 0; j < BABO_NODES; ++j)
            tmp2[i] += BaboMatrix_coefficient(this, i, j) * filter_tmpout[j];
		
        BaboNode_input(&this->node[i], tmp2[i] + input);
        /* We add delayed signal at the input of ^^^^^  the delay lines. */
    }
	
    outputs[0] = outputs[1] =   BaboNode_output(&this->node[0]) +
	BaboNode_output(&this->node[4]) +
	BaboNode_output(&this->node[8]);
	
    outputs[0] +=   (diffusion_coeff *
					 ((BaboNode_output(&this->node[7]) +
					   BaboNode_output(&this->node[12]))));
	
    outputs[1] +=   (diffusion_coeff *
					 ((BaboNode_output(&this->node[9]) +
					   BaboNode_output(&this->node[13]))));
	
    outputs[0] *=   this->complementary_early_diffusion;
    outputs[1] *=   this->complementary_early_diffusion;
}

/*
 * private utility functions
 */

//static void resolve_defaults(BABO *p);
//static void set_expert_values(CSOUND *csound, BABO *p);
//
//static void
//set_defaults(CSOUND *csound, BABO *p)
//{
//    resolve_defaults(p);
//	
//    p->diffusion_coeff = p->diffusion_coeff < 0 ?
//	BABO_DEFAULT_DIFFUSION_COEFF : p->diffusion_coeff;
//	
//    set_expert_values(csound,p);
//	
//    /*
//     * the user supplies, optionally, the complete distance
//     * or else it is set by default
//     * we divide by two because it is more handy to deal
//     * with half the distance in the program (i.e. the distance
//     * from a center point)
//     */
//    p->inter_receiver_distance *= 0.5;
//	
//}

//
//static void
//resolve_defaults(BABO *p)
//{
//    /*
//     * in typical csound backward logic :), the defaults may or may not,
//     * depending on how they're used in the orchestra definition,
//     * turn out to run on the same pointer - so, basically all optional
//     * argument values have to be copied (from last to first) in separate
//     * "real" values inside the entry structure in order to be used
//     */
//	
//    p->expert_values    = *(p->oexpert_values);
//    p->diffusion_coeff  = *(p->odiffusion_coeff);
//}

//static inline float
//load_value_or_default(const FUNC *table, int idx, float dEfault)
//{
//    float result = (table != (FUNC *) NULL && idx < table->flen) ?
//	table->ftable[idx] : dEfault;
//	
//    return result;
//}


// modified to set expert values to defaults
void set_expert_values(BABO *p)
{
    p->decay					= BABO_DEFAULT_DECAY;
    p->hidecay					= BABO_DEFAULT_HIDECAY;
    p->receiver_x				= BABO_DEFAULT_RECV_X;
    p->receiver_y				= BABO_DEFAULT_RECV_Y;
    p->receiver_z				= BABO_DEFAULT_RECV_Z;
    p->inter_receiver_distance	= BABO_DEFAULT_INTER_RECV_DISTANCE;
    p->direct					= BABO_DEFAULT_DIRECT;
    p->early_diffuse			= BABO_DEFAULT_DIFFUSE;
}

//static void
//verify_coherence(CSOUND *csound, BABO *p)
//{
//    if (*(p->lx) <= 0.0) || *(p->ly) <= FL(0.0) || *(p->lz) <= FL(0.0) {
//		csound->Die(csound, Str("Babo: resonator dimensions are incorrect "
//								"(%.1f, %.1f, %.1f)"),
//					*(p->lx), *(p->ly), *(p->lz));
//    }
//}

/*
 * PUBLIC FUNCTIONS - baboset(), babo()
 *
 * these get called from the csound engine
 *
 */

int baboset(BABO *p, float sr, float onedsr)	// JB: changed params to Babo *p
{
	
//    set_defaults(csound,p);
//    verify_coherence(csound,p);        /* exits if call is wrong */
	
    BaboTapline_create(sr,&p->tapline, p->lx, p->ly, p->lz);
    BaboDelay_create(sr, &p->matrix_delay,
                     BaboTapline_maxtime(onedsr, &p->tapline));
    BaboMatrix_create(sr, &p->matrix, p->diffusion_coeff, p->lx,
                      p->ly, p->lz, p->decay, p->hidecay, p->early_diffuse);
    return 1;	// JB: changed to 1 from OK
}

int babo(BABO *p, float sr, float onedsr, int num_samples)
{
    int     n, nsmps    = num_samples;
    float   *outleft    = p->outleft,
	*outright   = p->outright,
	*input      = p->input;
	
    BaboTaplineParameters left = { {0.0}, {{0.0}} },
	right = { {0.0}, {{0.0}} };
	
    BaboTapline_precalculate_parameters(onedsr, &p->tapline, &left,
										p->receiver_x - p->inter_receiver_distance,
										p->receiver_y, p->receiver_z,
										p->ksource_x, p->ksource_y, p->ksource_z,
										p->lx, p->ly, p->lz);
	
    BaboTapline_precalculate_parameters(onedsr, &p->tapline, &right,
										p->receiver_x + p->inter_receiver_distance,
										p->receiver_y, p->receiver_z,
										p->ksource_x, p->ksource_y, p->ksource_z,
										p->lx, p->ly, p->lz);
	
    for (n=0; n<nsmps; n++) {         /* k-time cycle                */
		float  left_tapline_out        = 0.0,
		right_tapline_out       = 0.0,
		delayed_matrix_input    = 0.0;
		float  matrix_outputs[2]       = { 0.0 };
		
		BaboTapline_input(&p->tapline, input[n]);
		BaboDelay_input(&p->matrix_delay, input[n]);
		
		left_tapline_out  = BaboTapline_output(&p->tapline, &left) *
        p->early_diffuse;
		
		right_tapline_out  = BaboTapline_output(&p->tapline, &right) *
        p->early_diffuse;
		
		delayed_matrix_input = BaboDelay_output(&p->matrix_delay);
		
		BaboMatrix_output(&p->matrix, matrix_outputs, delayed_matrix_input,
						  p->diffusion_coeff);
		
		outleft[n]  = left_tapline_out  + matrix_outputs[0];
		outright[n] = right_tapline_out + matrix_outputs[1];
    }
    return 1;
}

float babo_reverb_time(BABO *p) {
    float reverb_time = (2.0 * sqrt((p->lx*p->lx) + (p->ly*p->ly) + (p->lz*p->lz))) / sound_speed;
	return reverb_time;
}

