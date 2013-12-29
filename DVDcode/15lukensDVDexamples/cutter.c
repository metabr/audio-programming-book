// cutter.c
// Written by Tim Lukens
// artelis@gmail.com
//
// This program cuts an audio file into slices and plays them back in a random order.
// It's intended to be used with a drum loop, but can be used any way the user sees fit.
// Three libraries are used.
//
// portsf:
// Used to read an audio file and copy every sample to a 2D array (left and right channel).
//
// portaudio:
// Used to playback the samples read into the 2D array.
// Also used to pause the program for as long as a single slice.
// 
// portmidi:
// Used to select a MIDI device and read incoming MIDI messages.
//
// compile with:
// gcc cutter.c -o cutter -lportmidi -lportaudio lib/libportsf.a lib/portsf.a -framework coremidi -framework coreaudio


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "include/portaudio.h"
#include "include/portsf.h"
#include "include/portmidi.h"
#include "include/porttime.h"

#define PI			3.14159265

//Audio defines
#define BUFFER		256				//Audio buffer
#define SRATE		44100.0			//Sample rate
#define MAX_FRAMES	1048576			//Max number of samples our program can handle - 1048576 samples at an SR of 44100 is a little less than 24 seconds
#define ENV_SIZE	8192
#define ENV_DIVIDE	64

//MIDI defines
#define	MD_CC		 0xB0			//Hex for CC on channel 0

//cutterData struct that will be used for the entire program
typedef struct
{
	//Portsf stuff
	PSF_PROPS props;			//portsf file properties
	float framesread;			//flag for whether or not frames are being read
	float totalread;			//total number of frames read
	int ifd, ofd;				//in file, out file
	int error;					//number of errors
	PSF_CHPEAK* peaks;			//PEAK information
	
	//PortAudio stuff
	float* frame;				//pointer to current frame
	float** allFrames;			//pointer to a 2D array of all frames (left and right channel)
	int currentFrame;			//current frame - where we are in the file
	int numHold;				//number of times to hold on a slice
	int slices;					//total number of slices being made to the file
	int flagRand;				//if flagRand == 1 then slices are picked randomly
	
	//MIDI stuff
	const PmDeviceInfo *info;
	PmError retval;	PortMidiStream *mstream[32];		//PortMIDI stream (struct)
	PmEvent *event;						//PortMIDI event (struct)
	PmMessage pmmsg;					//PortMIDI message (long)
	int dev;							//PortMIDI device
	int cc[4];							//Continuous controller array
	int midiFlag;						//Are we using MIDI?
	
	//Envelope stuff
	float **envTable;
	float envInc;
	float envIndex;
	int window;
	int switchWindow;
	
} cutterData;


//function prototypes
void Initialize(cutterData *inData);
int SetupMidi(cutterData *inData);
int SetCC(cutterData *inData);
int OpenFile(cutterData *inData, char inFile[32]);
int ReadFile(cutterData*inData);
int StartAudio(cutterData *inData);
int MainLoop(cutterData *inData);
int MidiParser(cutterData *inData);


//main
int main(int argc, char* argv[])
{
	cutterData inData;		//struct with all the audio and midi data
	Initialize(&inData);	//Initialize values
	
	srand(time(NULL));		//seed random number generator to the clock
	
	//check for command line arguments
	if(argc < 2){
		printf("insufficient arguments.\n"
				"usage:\n\t"
				"./cutter infile [slices, stutters, random(0 or 1), MIDI(0 or 1)]\n");
		return 1;
	}
	
	if(argc > 2)
	{
		inData.slices = atoi(argv[2]);
		printf("Number of slices: %d\n", inData.slices);
	}
	if(argc > 3)
	{
		inData.numHold = atoi(argv[3]);
		printf("Number of stutters: %d\n", inData.numHold);
	}
	if(argc > 4)
	{
		if(atoi(argv[4]) == 0 || atoi(argv[4]) == 1) inData.flagRand = atoi(argv[4]);
		printf("Random = %d\n", inData.flagRand);
	}
	if(argc > 5)
	{
		if(atoi(argv[5]) == 0 || atoi(argv[5]) == 1) inData.midiFlag = atoi(argv[5]);
		if(atoi(argv[5]) == 0) printf("Running without MIDI\n");
	}
	
	//setup midi and continuous controllers
	if(inData.midiFlag)
	{
		if(SetupMidi(&inData)) return 1;
		if(SetCC(&inData)) return 1;
	}

	//start portsf
	if(psf_init()) {printf("unable to start portsf\n"); return 1;}
	printf("Portsf opened!\n");

	//Open, read, and start audio
	if(OpenFile(&inData, argv[1])) return 1;	
	if(ReadFile(&inData)) return 1;	
	if(StartAudio(&inData)) return 1;
	
	//Main loop
	if(MainLoop(&inData)) return 1;

	return 0;	
}


//Initialize all the data
void Initialize(cutterData *inData)
{
	system("clear"); //this command may not work on all systems. Comment it out if it doesn't compile.
	
	//set init values
	inData->ifd = -1;
	inData->ofd = -1;
	inData->error = 0;
	inData->peaks = NULL;
	inData->frame = NULL;
	inData->currentFrame = -1;
	inData->slices = 512;
	inData->numHold = 2;
	inData->flagRand = 0;
	inData->midiFlag = 1;
	inData->envIndex = 0.0;
	inData->window = 1;
	inData->switchWindow = 0;
	
	int i, j;
	
	//allocate space for 2D allFrames array
	inData->allFrames = (float **)malloc(sizeof(float*) * 2);
	for(i=0;i<2;i++)
	{
		inData->allFrames[i] = (float*)malloc(sizeof(float) * MAX_FRAMES);
	}
	
	//allocate space for MIDI variables
	inData->event = (PmEvent *)malloc(sizeof(PmEvent));   //allocate memory for the event
	inData->pmmsg = (PmMessage)malloc(sizeof(PmMessage));  //allocate memory for the message
	
	//allocate space for envelope table
	inData->envTable = (float **)malloc(sizeof(float *) * 3);
	for(i=0;i<3;i++)
	{
		inData->envTable[i] = (float*)malloc(sizeof(float) * ENV_SIZE);
	}
	
	//make envelope table
	double omega = 2.0 * PI / (ENV_SIZE-1);
	
	for(i=1;i<ENV_SIZE;i++)
	{
		for(j=0;j<3;j++)
		{
			inData->envTable[j][0] = 0;
			inData->envTable[j][ENV_SIZE - 1] = 0;
		
			//standard window
			if(j==0)
			{
				if(i < ENV_SIZE/ENV_DIVIDE) inData->envTable[j][i]	= (1 / ((float)ENV_SIZE / (float)ENV_DIVIDE)) * i;
				if(i >= ENV_SIZE/ENV_DIVIDE && i < ENV_SIZE - (ENV_SIZE/ENV_DIVIDE)) inData->envTable[j][i] = 1;
				if(i >= ENV_SIZE - (ENV_SIZE/ENV_DIVIDE)) inData->envTable[j][i] = (1 / ((float)ENV_SIZE / (float)ENV_DIVIDE)) * (ENV_SIZE - i);
			}
		
			if(j==1) inData->envTable[j][i] = (0.54 - 0.46 * cos(omega*(i-1)));		//Hamming Window
			if(j==2) inData->envTable[j][i] = (.5 * (1 - cos(omega*(i-1))));		//Hann Window
		}
	}
}



//Select MIDI device
int SetupMidi(cutterData *inData)
{
	int cnt, i, go = 1;
	
	Pm_Initialize();	//Init PortMIDI
	
	if (cnt = Pm_CountDevices())		//count the number of midi devices
	{
		for(i = 0; i < cnt; i++)		//loop through every midi device
		{
			inData->info = Pm_GetDeviceInfo(i);
			if(inData->info->input)				//is the device available for input?
				inData->retval = Pm_OpenInput(&inData->mstream[i], i, NULL, 512, NULL, NULL);	//open every midi input device
		}
	}
	
	printf("Play the MIDI device you wish to use\n");
	
	//Loop to auto-detect the MIDI device to be used throughout the program
	while(go)
	{
		for(i = 0; i < cnt; i++)	//loop through every midi device
		{
			if(Pm_Read(inData->mstream[i], inData->event, sizeof(long)) > 0)	//if we receive an event from any midi device at all then
				inData->dev = i, go = 0;								//select it as our main input device
		}
	}
	//end loop
	
	printf("You have selected MIDI device %d: %s\n", inData->dev, Pm_GetDeviceInfo(inData->dev)->name);

	Pt_Start(1, NULL, NULL); //start timer
	
	//close extra midi streams
	for(i = 0; i < cnt; i++)
	{
		if(i != inData->dev)
			Pm_Close(inData->mstream[i]);
	}
	
	return 0;
}


//Set CCs
int SetCC(cutterData *inData)
{
	int i, go = 1, timeCount;
	
	timeCount = Pt_Time();
	while(Pt_Time() - timeCount < 1000); //give us some time after we move the controller
	
	//clear MIDI buffer
	while(Pm_Poll(inData->mstream[inData->dev]))
	{
		Pm_Read(inData->mstream[inData->dev], inData->event, 1);
	}
	
	for(i = 0; i < 4; i++) //loop to assign CC's
	{
		switch(i)
		{
			case 0:
				printf("Move slices controller\n");
				break;
			case 1:
				printf("Move holds controller\n");
				break;
			case 2:
				printf("Move direction controller\n");
				break;
			case 3:
				printf("Move window controller\n\n\n");
				break;
		}
		
		while(go)
		{
			if(Pm_Read(inData->mstream[inData->dev], inData->event, sizeof(long)) > 0) //did something happen?
			{
				inData->pmmsg = inData->event->message; //assign the event to pmmsg
				inData->cc[i] = Pm_MessageData1(inData->pmmsg); //put that data byte into the CC array as and int
				go = 0;
			}
		}
		
		timeCount = Pt_Time();
		while(Pt_Time() - timeCount < 1000); //give us some time after we move the controller
		
		//clear MIDI buffer
		while(Pm_Poll(inData->mstream[inData->dev]))
		{
			Pm_Read(inData->mstream[inData->dev], inData->event, 1);
		}
		
		go = 1;
	}
		
	return 0;
}



//Open the file for reading
int OpenFile(cutterData *inData, char inFile[32])
{
	//open file
	inData->ifd = psf_sndOpen(inFile,&inData->props,0);												  
	if(inData->ifd < 0){
		printf("Error: unable to open infile %s\n",inFile);
		return 1;
	}
	
	//allocate space for one sample frame
	inData->frame = (float*) malloc(inData->props.chans * sizeof(float));
	if(inData->frame==NULL){
		printf("No memory!\n");
		return 1;
	}
		
	//allocate space for PEAK info
	inData->peaks  =  (PSF_CHPEAK*) malloc(inData->props.chans * sizeof(PSF_CHPEAK));
	if(inData->peaks == NULL){
		printf("No memory!\n");
		return 1;
	}
	
	
	printf("File %s opened successfully!\n", inFile);
	
	return 0;
}



int ReadFile(cutterData *inData)
{
	int i;
	inData->totalread = 0; // count sample frames as they are copied
	
	//read the entire audio file and store it in allFrames
	while ((inData->framesread = psf_sndReadFloatFrames(inData->ifd, (float*)inData->frame, 1)) == 1)		
	{
		//loop for mono or stereo
		for(i=0;i<inData->props.chans;i++)
			inData->allFrames[i][(int)inData->totalread] = inData->frame[0];
		
		inData->totalread++;
	}
	
	printf("Total frames: %d\n", psf_sndSize(inData->ifd));
	inData->envInc = ((1 / ((float)inData->totalread / (float)SRATE / inData->slices)) * ((float)ENV_SIZE / (float)SRATE)) / 2;
	
	printf("Envelope Increment: %f\n", inData->envInc);
	
	return 0;
}
		




static int paCutterCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
	//cast last userdata to our data pointer
	cutterData *data = (cutterData*)userData; 

	float *out = outputBuffer;
	int i;
	
	for(i=0; i<framesPerBuffer; i++)
	{
		if(data->flagRand != 2) data->currentFrame++;
		else data->currentFrame--;
		
		*out++ = data->allFrames[0][data->currentFrame] * data->envTable[data->window - 1][(int)data->envIndex];
		if(data->props.chans == 2)
		{
			data->envIndex += data->envInc;
			if(data->envIndex > ENV_SIZE) data->envIndex -= ENV_SIZE;
			*out++ = data->allFrames[1][data->currentFrame] * data->envTable[data->window - 1][(int)data->envIndex];
		}

		data->envIndex += data->envInc;
		if(data->envIndex > ENV_SIZE) data->envIndex -= ENV_SIZE;
	}
	
	return 0;
}




int StartAudio(cutterData *inData)
{
	PaStream *stream;
	PaError err;
    
	// Initialize library before making any other calls
	err = Pa_Initialize();
	if( err != paNoError ) goto error;
    
	// Open an audio I/O stream
	err = Pa_OpenDefaultStream( &stream,
			      0,					// no input channels
			      inData->props.chans,	// same number of output channels as infile
			      paFloat32,			// 32 bit floating point output
			      SRATE,				// sample rate
			      BUFFER,				// frames per buffer
			      paCutterCallback,		// callback function
			      inData);				// data structure
	if( err != paNoError ) goto error;

	err = Pa_StartStream( stream );
	if( err != paNoError ) goto error;
	
	return 0;
	
error:
	printf("Error starting PortAudio stream!\n");
	return 1;
}



int MainLoop(cutterData *inData)
{
	float timer;
	int randnum = 0, i = 0;
	
	//MAIN LOOP
	timer = (float)inData->totalread / (float)SRATE / inData->slices;
	printf("Slice size: %f seconds\n", timer);
	
	printf("Press ctrl-c to quit.\n\n");

	while(1)
	{
		timer = (float)inData->totalread / (float)SRATE / inData->slices;
		Pa_Sleep(timer*1000);
		
		if(inData->midiFlag)
		{
			inData->envInc = (1 / ((float)inData->totalread / (float)SRATE / inData->slices)) * ((float)ENV_SIZE / (float)SRATE);
			MidiParser(inData);
		}
		
		if(i == 0)
		{
			if (inData->flagRand == 0) randnum++;
			if (inData->flagRand == 1) randnum = rand() % inData->slices;
		}
		if(randnum >= inData->slices) randnum = 0;
		if(randnum < 0) randnum = inData->slices;
		
		if(inData->flagRand != 2) inData->currentFrame = (randnum * (psf_sndSize(inData->ifd) / inData->slices));
		else inData->currentFrame = (randnum * (psf_sndSize(inData->ifd) / inData->slices)) + timer;
		
		i++;
		if(i >= inData->numHold) i = 0;
	}

	return 0;
}



int MidiParser(cutterData *inData)
{
	long status, data1, data2;
	float sliceVal, holdVal, randVal, windowVal;
	int i;
	
	if(Pm_Read(inData->mstream[inData->dev], inData->event, sizeof(long)) > 0) //did something happen?
	{
		inData->pmmsg = inData->event->message; //Decode the MIDI message into a long word
		status = Pm_MessageStatus(inData->pmmsg); //Decode the status
		data1 = Pm_MessageData1(inData->pmmsg); //Decode the first data byte
		data2 = Pm_MessageData2(inData->pmmsg); //Decode the second data byte
				
		if(status == MD_CC)
		{
			//number of slices
			if(data1 == inData->cc[0])
			{
				sliceVal = (pow(2, (int)(data2 / 8)));
				if (sliceVal < 1) sliceVal = 1;
				if(inData->slices != sliceVal)
				{
					inData->slices = sliceVal;
					printf("Slices = %d\n", inData->slices);
					if(inData->flagRand != 2) inData->currentFrame = -1;
					else inData->currentFrame = inData->totalread;
				}
			}
			
			//hold value
			if(data1 == inData->cc[1])
			{
				holdVal = (int)(data2 / 8);
				if (holdVal < 1) holdVal = 1;
				if(inData->numHold != holdVal)
				{
					inData->numHold = holdVal;
					printf("Number of holds = %d\n", inData->numHold);
					if(inData->flagRand != 2) inData->currentFrame = -1;
					else inData->currentFrame = inData->totalread;
				}
			}
			
			//direction
			if(data1 == inData->cc[2])
			{
				randVal = (int)(data2 / 42);
				randVal--;
				if(randVal > 1) randVal = 1;
				if(randVal < 0) randVal = 0;
				
				if(inData->flagRand != randVal)
				{
					inData->flagRand = randVal;
					if(randVal)
					{
						printf("Direction: Random\n");
						inData->currentFrame = -1;
					}
	
					if(!randVal)
					{
						printf("Direction: Forwards\n");
						inData->currentFrame = -1;
					}
				}
			}

			//window change			
			if(data1 == inData->cc[3])
			{
				windowVal = (int)(data2 / 32);
				if(windowVal < 1) windowVal = 1;
				
				if(inData->window != windowVal)
				{
					inData->window = windowVal;
					printf("Window: %d (1 = Rectangle, 2 = Hamming, 3 = Hann)\n", inData->window);
				}
			}
		}
	}
	
	//Flush MIDI buffer
	while(Pm_Poll(inData->mstream[inData->dev]))
	{
		Pm_Read(inData->mstream[inData->dev], inData->event, 1);
	}

	
	return 0;
}