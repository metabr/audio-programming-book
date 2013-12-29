/* sfplay.c
  * simple soundfile playback for The Book
 *  using the Portable Audio v19 api.
 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */	

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <portaudio.h>
#include <portsf.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <signal.h>
#ifdef _WIN32
#include <conio.h>
#endif

#define FRAMESIZE (4096)
#ifndef min
#define min(x,y)  ((x) < (y) ? (x) : (y))
#endif
typedef struct {
	float *buffer;
	long framesize;
	int used;
}
psf_buffer;


typedef struct {
	PSF_PROPS props;
	int sfd;
	psf_buffer *outbuf;
	unsigned long frames_played;
	int last_buffer; /* init to 1; at detected eof send one extra buffer to flush, and set this to 0 */
} psfdata;
static int file_playing;


/* traps ctrl-C, stops  playback  */
void playhandler(int sig)
{
	if(sig == SIGINT)
		file_playing = 0;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/

static int patestCallback(const	void *inputBuffer, void *outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
				        void *userData )
{
	psfdata *data = (psfdata *)userData;
	float *out = (float*)outputBuffer;
	
	if(file_playing) {
        /* our signal that we have used up all file data */
		if(data->outbuf->framesize==0){
			file_playing = 0;
			return 1;     /* portaudio plays out data remaining in internal buffers */
		}
		if(out==NULL)
			return 0;
		/* copy current block, and read next one when available*/
	   /* TODO: get some sort of timeout in here! */

		memcpy(out,data->outbuf->buffer,data->outbuf->framesize * sizeof(float) * data->props.chans);
#ifdef _WIN32
		InterlockedExchange(&(data->outbuf->used),1);
#else
		data->outbuf->used = 1;
#endif
		data->frames_played += data->outbuf->framesize;
	
	}

	return 0;
}


int show_devices(void);






/*******************************************************************/
int main(int argc,char **argv)
{
	PaStreamParameters outputParameters;
    PaStream *stream = NULL;
	PaError err = paNoError;
	PaDeviceIndex ndevices;
	psfdata sfdata;
	PSF_CHPEAK *fpeaks = NULL;
	int res;
	time_t in_peaktime = 0;
    MYLONG lpeaktime;
	int waitkey = 1;
	int from_frame = 0;
	int i,j= 0;
	PaDeviceIndex device;
    PaTime starttime;
	long framesread,size;
	float *buf = NULL;	
	long inchans;

	signal(SIGINT,playhandler);

	sfdata.sfd = -1;
	sfdata.outbuf = NULL;
	sfdata.last_buffer = 1;   /* HACK! to flush all audio before stopping! */
	//TODO: use the FinishedCallback system!
	printf("SFPLAY: play multi-channel soundfile. V1.0 RWD 2009\n");
	printf("Uses Portaudio Library v19.\n\n");
	file_playing = 0;
	err = Pa_Initialize();
	if( err != paNoError ) 
		goto error;
	ndevices =  Pa_GetDeviceCount();
	if(ndevices){
		device =  Pa_GetDefaultOutputDevice();
	}
	else{
		printf("no output device found!\n");
		return 1;
	}
	if(argc < 2){
		printf("usage: sfplay [-dN] soundfile [from]\n"
			   "       -dN  : use output Device N\n"
#ifdef WIN32			  
			   "channel config for soundcard (WIN32 DirectSound):\n"
			   "       3ch --> L-C-R\n"
			   "       4ch --> rectangular Quad\n"
			   "       5ch --> 5.0 (Quad + Centre)\n"
			   "       6ch --> standard 5.1 format.\n"
			   "  Otherwise, sets speaker position flags to 0 for generic output assignment\n"
#endif
			   "sfplay reads PEAK chunk if present to rescale over-range floatsam files\n"
			   
			   );

		show_devices();
		Pa_Terminate();
			return 1;
	}

	while(argv[1][0]=='-'){
		switch(argv[1][1]){
		
		case('d'):
			if(argv[1][2]=='\0'){
				printf("-d flag requires parameter\n");
				return 1;
			}
			device = atoi(&(argv[1][2]));
			break;
		case('i'):
			waitkey = 0;
			break;
		
		default:
			printf("unrecognised flag option\n");
			Pa_Terminate();
			return 1;
		}
		argv++;	 argc--;
	}

	if(argc < 2){
		printf("usage: sfplay [-dN] soundfile\n");
		printf("       -dN  : use output Device N\n");
		show_devices();
		Pa_Terminate();
		return 1;
	}

	if(psf_init()){
		printf("Error initializing psfsys\n");
		goto error;
	}
	/* allow auto rescaling of overrange floats via PEAK chunk */
	sfdata.sfd = psf_sndOpen(argv[1],&(sfdata.props),1);
	if(sfdata.sfd < 0){
		printf("unable to open soundfile %s\n",argv[1]);
		goto error;
	}
	size = psf_sndSize(sfdata.sfd);
	if(size ==0){
		printf("Soundfile is empty!\n");
		goto error;
	}
	inchans = sfdata.props.chans;
	

	printf("opened %s: %ld frames, %ld channels\n",argv[1],size,inchans);
	fpeaks = (PSF_CHPEAK *) calloc(inchans,sizeof(PSF_CHPEAK));
	if(fpeaks==NULL){
		puts("no memory for PEAK data\n");
		goto error;
	}
    //lpeaktime = (int) in_peaktime;
	res = psf_sndReadPeaks(sfdata.sfd,fpeaks,(MYLONG *) &lpeaktime);
	if(res==0) {
		printf("no PEAK data in this soundfile\n");
	}
	else if(res < 0){
		printf("Error reading PEAK data\n");
		goto error;
	}
	else{
        in_peaktime = (time_t) lpeaktime;
		printf("PEAK data:\ncreation time: %s",ctime(&in_peaktime));
        
		for(i=0;i < inchans;i++){
			printf("CH %d: %.4f at frame %lu: \t%.4f secs\n",
				i,fpeaks[i].val,fpeaks[i].pos,(double)(fpeaks[i].pos / (double)sfdata.props.srate));
		}
	}
	
	
	sfdata.outbuf = (psf_buffer *) malloc(sizeof(psf_buffer));
	if(sfdata.outbuf==NULL){
		puts("No memory for audio data\n");
		goto error;
	}
		

	sfdata.outbuf->buffer = (float *) malloc(FRAMESIZE * sizeof(float) * sfdata.props.chans);	
	if(sfdata.outbuf->buffer==NULL){
		puts("No memory for audio buffer\n");
		goto error;
	}
	sfdata.outbuf->used = 0;
	sfdata.outbuf->framesize = FRAMESIZE;
	sfdata.frames_played  = 0;
	
	buf = sfdata.outbuf->buffer;
    if(from_frame > 0){
		if(psf_sndSeek(sfdata.sfd,from_frame,PSF_SEEK_SET))	{
			printf("Error setting start position\n");
			goto error;
		}
	}
	if((framesread = psf_sndReadFloatFrames(sfdata.sfd,buf,FRAMESIZE)) < 0){
	    printf("Error reading soundfile data\n");
	    goto error;
	}
	
	if(framesread < FRAMESIZE)
		for(j= framesread;j < FRAMESIZE * sfdata.props.chans;j++)
			buf[j] = 0.0f;
	

	outputParameters.device = device;   /*Pa_GetDefaultOutputDevice(); */ /* default output device */
    outputParameters.channelCount = sfdata.props.chans;                     
    outputParameters.sampleFormat = paFloat32;             /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;


	err = Pa_OpenStream(
				&stream,
				NULL,  /* No input */
				&outputParameters, /* As above. */
                sfdata.props.srate,
				FRAMESIZE,			/* Frames per buffer. */
				paClipOff,      /* we won't output out of range samples so don't bother clipping them */
				patestCallback,
				&sfdata );
	if( err != paNoError ) 
		goto error;
	
	file_playing = 1;
	err = Pa_StartStream( stream );
	if( err != paNoError ) 
		goto error;
    starttime = Pa_GetStreamTime(stream);
	printf("Hit CTRL-C to stop.\n");
	while(file_playing){
		/* fill buffer with data, when available */
		if(sfdata.outbuf->used){
			if((framesread = psf_sndReadFloatFrames(sfdata.sfd,sfdata.outbuf->buffer,FRAMESIZE)) < 0){
				printf("Error reading soundfile data\n");
				goto error;
			}
			if(framesread==0){
				if(sfdata.last_buffer<=0)
					break;
				if(sfdata.last_buffer>0){
					framesread = FRAMESIZE;
					memset(sfdata.outbuf->buffer,0,framesread * sfdata.props.chans * sizeof(float));
					sfdata.last_buffer--;
				}
				
			}
			else {
                buf = sfdata.outbuf->buffer + (framesread * sfdata.props.chans);
                //pad short buffer to full framesize
                unsigned int remainder = (FRAMESIZE - framesread) * sizeof(float) * sfdata.props.chans;
                memset(buf,0,remainder);
            }
			if(sfdata.frames_played % (FRAMESIZE * 10)==0){
				//printf("\t%.4f secs\r",(float)(sfdata.frames_played / (double) sfdata.props.srate));
				printf("\t%.4f secs\r",(double)Pa_GetStreamTime(stream ) - starttime );
				fflush(stdout);
			}
			
			sfdata.outbuf->used = 0;
		}
		Pa_Sleep(1);
	}
    err = Pa_StopStream(stream); /* this only returns when final buffer fully played */
    if( err != paNoError ) 
		goto error;
	printf("\t%.4f secs\n",(float)(sfdata.frames_played / (double) sfdata.props.srate));
	fflush(stdout);
	printf("Playback finished.\n");
	err = Pa_CloseStream( stream );
	if( err != paNoError ) 
		goto error;
error:
	Pa_Terminate();
	if(sfdata.sfd >=0)
		psf_sndClose(sfdata.sfd);
	if(fpeaks)
		free(fpeaks);
	if(sfdata.outbuf){
		if(sfdata.outbuf->buffer)
			free(sfdata.outbuf->buffer);
		free(sfdata.outbuf);
	}

	psf_finish();
	if(err != paNoError){
		fprintf( stderr, "An error occured while using the portaudio stream\n" ); 
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		return err;
	}
	return 0;
}

int show_devices(void)
{
		int i,j;
		PaDeviceIndex numDevices,p;

		const    PaDeviceInfo *pdi;
		PaError  err;
		int nOutputDevices = 0;

#ifdef USE_ASIO
		printf("For ASIO multi-channel, you may need to select the highest device no.\n");		
#endif
		/*Pa_Initialize();*/
		numDevices =  Pa_GetDeviceCount();
		if( numDevices < 0 )
		{
			printf("ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
			err = numDevices;
			return err;
		}
		printf("Number of devices = %d\n", numDevices );
		for( p=0; p<numDevices; p++ )
		{
			pdi = Pa_GetDeviceInfo( p );
			
#ifdef _WIN32
			/*RWD: skip, info on inputs */
			if(pdi->maxOutputChannels == 0)
				continue;
#endif
			nOutputDevices++;

			/*RWD not so interested in inputs here */
			/*if( i == Pa_GetDefaultInputDeviceID() ) printf(" DefaultInput");*/
			if( p == Pa_GetDefaultOutputDevice() ) 
                printf("*");
            else
                printf(" ");
			printf("%d\t%d\t%d\t%s\n",p,
                   pdi->maxInputChannels,
                   pdi->maxOutputChannels,
                   pdi->name);
		}
		
		return 0;
}
