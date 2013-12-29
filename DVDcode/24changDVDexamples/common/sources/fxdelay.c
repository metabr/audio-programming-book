/* dsptools.c: implementaion file for delay objects and functions */

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <fxdelay.h>
/* don't want these to be public constants: */
#define NTAPS (128)
#define LINELENGTH (256)

FXDELAY* new_fxdelay(void)
{
	FXDELAY* delay;
	
	delay = (FXDELAY*) malloc(sizeof(FXDELAY));
	if(delay == NULL)
		return NULL;
	delay->dl_buf = NULL;
	delay->dl_length = 0;
	delay->dl_input_index = 0;
	delay->dl_srate = 0;
	delay->taps = NULL;
	return delay;
}

int fxdelay_init(FXDELAY* delay,long srate,double length_secs)
{	
	
	unsigned long len_frames = (unsigned long)(srate * length_secs );	
	if(len_frames == 0)
		return -1;
	/*  round upwards to allow for interpolation */
	len_frames++;
	/* reject init  if already created*/
	if(delay->dl_buf) 
		return -1;
	delay->dl_buf = (float *) calloc(len_frames,sizeof(float));
	if(delay->dl_buf == NULL)
		return -1;
	delay->dl_length = len_frames;
	delay->dl_input_index = 0;
	delay->dl_srate = srate;
	return 0;
}


int fxdelay_mtinit(FXDELAY* delay,long srate, DELAYTAP taps[],int ntaps)
{
	int i;
	/* create delay taking length from last tap */
	if(fxdelay_init(delay,srate,taps[ntaps-1].taptime))
		return -1;

	delay->taps = (MULTITAP*) malloc(ntaps * sizeof(MULTITAP));
	if(delay->taps == NULL){
		fxdelay_free(delay);
		return -1;
	}
	for(i=0;i < ntaps; i++)	{
		/* set delay taps away from dl_length, not from 0 ! */
		delay->taps[i].index = delay->dl_length - (unsigned long)(taps[i].taptime * srate + 0.5);
		/*  scale tapamps by ntaps  so sum is 1.0 */
		delay->taps[i].amp = taps[i].amp  / ntaps;
	}
	delay->ntaps = ntaps;
	return 0;
}


float  fxdelay_tick(FXDELAY* delay, double vdelay_secs,double feedback,float input)
{
	unsigned long base_readpos; 
	unsigned long next_index; 
	double vlength,dlength,frac,output;
	double readpos;
	float* buf = delay->dl_buf;
	
	dlength = (double) delay->dl_length;					/* get maxlen, save a cast later on */
	/* read pointer is vlength ~behind~ write pointer */
	vlength = dlength -  (vdelay_secs  * delay->dl_srate);
	vlength = vlength < dlength ? vlength : dlength;	  /* clip vdelay to max del length */

	/* this ifdef is simply to enable the truncating delay to be tried out, 
	 to find out why we don't want to use it! */

#ifndef INTERP_DELAY
	readpos = delay->dl_input_index + vlength;
	base_readpos = (unsigned long) ( readpos);				  
	if(base_readpos >= delay->dl_length)					  /* wrap dl indices */
		base_readpos -= delay->dl_length;

	next_index = base_readpos + 1;
	if(next_index >= delay->dl_length)
		next_index -= delay->dl_length;
	/* basic interp of variable delay pos */
	frac = readpos - (int) readpos;						  
	output = buf[base_readpos]+((buf[next_index] - buf[base_readpos]) * frac);
#else
	/* can use this to demo truncating delay */
   base_readpos = (unsigned long) ( (delay->dl_input_index + vlength) + 0.5);
   if(base_readpos >= delay->dl_length)					  /* wrap dl indices */
		base_readpos -= delay->dl_length;
   output = buf[base_readpos];
#endif	
	/* add in new sample + fraction of ouput, unscaled, for minimum decay at max feedback */
	buf[delay->dl_input_index++] = (float)(float) (input + (feedback * output));
	if(delay->dl_input_index == delay->dl_length)
		delay->dl_input_index = 0;
	return (float) output;
}

float  fxdelay_mttick(FXDELAY* delay, double feedback,float input)
{
	unsigned long i,index; 
	double output;
	float* buf = delay->dl_buf;
	
	output = 0.0;
	for(i = 0; i < delay->ntaps; i++){
		index = delay->taps[i].index;
		output += buf[index] * delay->taps[i].amp;	
		if(++index == delay->dl_length)
			index = 0;
		delay->taps[i].index = index;
	}
	buf[delay->dl_input_index++] = (float) (input + feedback * output );
	if(delay->dl_input_index == delay->dl_length)
		delay->dl_input_index = 0;

	return (float) output;
}


void fxdelay_free(FXDELAY* delay)
{
	if(delay){
		if (delay->dl_buf){
			free(delay->dl_buf);
			delay->dl_buf = NULL;
		}
		if(delay->taps)	{
			free(delay->taps);
			delay->taps = NULL;
		}
	}
}

/* internal (private) storage for error messages */
static char gettaps_error[80] = {'\0'};

/* based on the code in breakpoints.c */
/* uses pointer to a pointer to return new array */
int get_taps(FILE* fp, DELAYTAP** ptaps)
{	
	int got;
	int err = 0;
	long ntaps = 0, size = NTAPS;
	double lasttime    = 0.0;
	DELAYTAP* taps = NULL;
	char line[LINELENGTH];

	/* must set/reset this each time the function is called */
	strcpy(gettaps_error,"No error");

	if(fp == NULL) {
		strcpy(gettaps_error, "Bad file pointer");
		return -1;
	}
	taps = (DELAYTAP *) malloc(sizeof(DELAYTAP) * size);
	if(taps == NULL) {
		strcpy(gettaps_error,"No memory");
		return -1;
	}
	/* OK so far: read each line of file */
	while(fgets(line,LINELENGTH,fp)){
		double thistime, thisamp;
		if((got = sscanf(line, "%lf%lf", &thistime,&thisamp) ) < 0)
			continue;			  /* empty line */
		if(got == 0){
			sprintf(gettaps_error, "Line %ld has non-numeric data",ntaps+1);
			err++;
			break;
		}
		if(got == 1){
			sprintf(gettaps_error,"Incomplete tap data found at point %ld",ntaps+1);
			err++;
			break;
		}
		if(thistime == 0.0){
			sprintf(gettaps_error,"First tap time must be > 0.0.");
			err++;
			break;
		}
		if(thistime <= lasttime){
			sprintf(gettaps_error,"error in breakpoint data at point %ld: time not increasing", ntaps+1);
			err++;
			break;
		}
		lasttime = thistime;
		taps[ntaps].taptime = thistime;
		taps[ntaps].amp = thisamp;
		if(++ntaps == size){
			DELAYTAP* tmp;
			size += NTAPS;
			tmp = (DELAYTAP*) realloc(taps, sizeof(DELAYTAP) * size);
			if(tmp == NULL)	{	/* too bad! */
				/* have to release the memory, and return NULL to caller */
				strcpy(gettaps_error,"No memory");
				ntaps = 0;
				free(taps);
				taps = NULL;
				break;		 
			}
			taps = tmp;
		}			
	}
	if(err){
		free(taps);
		taps = NULL;
		ntaps = -1;
	}
	if(ntaps > 0)							
		*ptaps = taps;
	
	return ntaps;
}
/* get the error message: hides the string itself from hostile intent! */
const char* taps_getLastError(void)
{
	return (const char*) gettaps_error;
}

