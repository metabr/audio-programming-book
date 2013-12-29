//
//  RemoteIO.m
//  Spectral Template
//
//  Created by Johannes Bochmann on 15.04.09.
//  Copyright 2009 JoboMusic GmbH. All rights reserved.
//


#import <AudioUnit/AudioUnit.h>
#import "RemoteIO.h"
#import "pv.h"
//#include "spec.h"
//#include "fourier.h"


#define kOutputBus 0
#define kInputBus 1


@implementation RemoteIO

@synthesize inMemoryAudiofile;
@synthesize bufferMatrix;
@synthesize loadPercent;

//the remoteIO audioUnit
AudioComponentInstance audioUnit;
AudioStreamBasicDescription audioFormat;



-(id)initWithFile:(NSString *)file {
	
	if ((self = [super init])) {
        
		//standart fft size of 1024 samples per fft frame
		channel1InputSampleBuffer = (float *)malloc(sizeof(float) * FFT_FRAMESIZE);
		channel1OutputSampleBuffer = (float *)malloc(sizeof(float) * FFT_FRAMESIZE);

		for(int j=0;j<FFT_FRAMESIZE;j++)
			channel1OutputSampleBuffer[j] = 0.0;
		
		
		//the ouput is magnitude and phases interleaved
		channel1MagnitudePhasesBuffer = (float *)malloc(sizeof(float) * FFT_FRAMESIZE * (FFT_FRAMESIZE/HOPSIZE)); 
		
		channel1MagnitudePhasesFrame = (float *)malloc(sizeof(float) * FFT_FRAMESIZE * (FFT_FRAMESIZE/HOPSIZE));
		
		currentMatrixFrame = 0;

		//setup the window for the fft
		fftWindow = (float *)malloc(sizeof(float) * FFT_FRAMESIZE);
		for(int i=0; i< FFT_FRAMESIZE; i++) fftWindow[i] = 0.5f - (float)(0.5*cos((double)i*twopi/(double)FFT_FRAMESIZE));
		for(int i=0; i < FFT_FRAMESIZE; i++) fftWindow[i] *= 0.8;
		
		initializeStft();
		
		inMemoryAudiofile = [[InMemoryAudioFile alloc] init];
		
		[self performSelectorInBackground:@selector(analyzeAudioFile:) withObject: file];
		
    }
    return self;
}

-(void)analyzeAudioFile:(NSString *)file 
{
	/*You need this for all threads you create or you will leak! */
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	//open our audio file - the data is stored in an internal array
	[inMemoryAudiofile open:file];
	printf("Packet Count: %d\n", [inMemoryAudiofile packetCount]);
	
	//in case we had a previous audio file open delete the buffer matrix
	delete[] bufferMatrix;
	//this bufferMatrix has room to hold a mono channel of the incoming audio file
	bufferMatrix = new FFTBufferMatrix([inMemoryAudiofile packetCount], FFT_FRAMESIZE/HOPSIZE);
	
	//free any previously assigned buffers
	delete[] channel1InputSampleBuffer;
	delete[] channel1MagnitudePhasesBuffer;
	
	channel1InputSampleBuffer = (float *) malloc(sizeof(float) * [inMemoryAudiofile packetCount]);
	channel1MagnitudePhasesBuffer = (float *) malloc(sizeof(float) * [inMemoryAudiofile packetCount] * (FFT_FRAMESIZE/HOPSIZE));
	
	//convert from SInt16 to float
	for(int i=0; i<[inMemoryAudiofile packetCount]; i++)
	{
		//two samples make up one frame, we collapse to mono
		channel1InputSampleBuffer[i] = ([self convertInt16ToNormalizedFloat:[inMemoryAudiofile getNextSample]] + [self convertInt16ToNormalizedFloat:[inMemoryAudiofile getNextSample]])/2.f;
	}
	
	//analyze the mono channel
	stft(channel1InputSampleBuffer, fftWindow, channel1MagnitudePhasesBuffer, [inMemoryAudiofile packetCount], FFT_FRAMESIZE, HOPSIZE);
	
	int i, k, column, index, percentStep, percentStepCounter;
	loadPercent = 0;
	percentStepCounter = 0;
	percentStep = [inMemoryAudiofile packetCount]*(FFT_FRAMESIZE/HOPSIZE)/100;
	//copy real and imaginary values into the bufferMatrix and calculate the mag and phase
	for(i=0, k=0; i<[inMemoryAudiofile packetCount]*(FFT_FRAMESIZE/HOPSIZE); i+=2, k++)
	{
		column = i/FFT_FRAMESIZE;
		index = k%(FFT_FRAMESIZE/2);
		
		percentStepCounter+=2;
		
		if(percentStepCounter >= percentStep) 
		{
			percentStepCounter = 0;
			loadPercent++;
			printf("Analyzing: %d%%\n", loadPercent);
		}
		
		bufferMatrix->setMagPhasesMatrixElement(column, index, channel1MagnitudePhasesBuffer[i], channel1MagnitudePhasesBuffer[i+1]);
	}
	
	//reset the file positions to zero
	bufferMatrix->resetStreams();
	printf("number of columns: %d\n", column);
	//remove our pool and free the memory collected by it
    [pool release];
}



-(float)convertInt16ToNormalizedFloat:(SInt16) input
{
	float output = (float)input;
	
	output /= 32767.0;
	
	return output;
}

-(SInt16)convertNormalizedFloatToInt16:(float) input
{
	SInt16 output;
	
	if((input > 1.0f) || (input < -1.0f)) printf("overbound %f\n", input);
	
	input *= 32767.0f;
	output = (SInt16) input;
	
	return output;
}



//our audio render callback
static OSStatus playbackCallback(void *inRefCon, 
								 AudioUnitRenderActionFlags *ioActionFlags, 
								 const AudioTimeStamp *inTimeStamp, 
								 UInt32 inBusNumber, 
								 UInt32 inNumberFrames, 
								 AudioBufferList *ioData) 
{  
	//get a copy of the objectiveC class "self" we need this to get the next sample to fill the buffer
	RemoteIO *remoteIO = (RemoteIO *)inRefCon;
		
	//get the number of samples that need to be filled and perform fft
	for (int i = 0 ; i < ioData->mNumberBuffers; i++){
		
		AudioBuffer buffer = ioData->mBuffers[i];
		
		//casting the callback-buffer according to the ASBD of the remoteIO
		SInt16 *frameBuffer = (SInt16 *) buffer.mData;
		
		//get the current frame from our bufferMatrix
		remoteIO->bufferMatrix->getMagPhasesFrameFromFile(remoteIO->currentMatrixFrame, remoteIO->channel1MagnitudePhasesFrame);
		remoteIO->currentMatrixFrame++;
				
		//inverse stream fft
		streamIstft(remoteIO->channel1MagnitudePhasesFrame, remoteIO->fftWindow, remoteIO->channel1OutputSampleBuffer, FFT_FRAMESIZE, HOPSIZE);
		
		//output it
		int j, k;
		for(j = 0, k=0; j < (inNumberFrames * 2); j+=2, k++)
		{
			
			frameBuffer[j] = frameBuffer[j+1] = [remoteIO convertNormalizedFloatToInt16:remoteIO->channel1OutputSampleBuffer[k]];
		}
				
	}
	
	return noErr;
	
}

-(void) checkStatus:(OSStatus) status
{
	//do error checking here
}


// Below code is a cut down version (for output only) of the code written by
// Micheal "Code Fighter" Tyson (punch on Mike)
// See http://michael.tyson.id.au/2008/11/04/using-remoteio-audio-unit/ for details
-(void) initializeAudio{
	
	OSStatus status;
	
	// This description specifies the Audio Unit to be a RemoteIO type
	AudioComponentDescription desc;
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	// Get the component
	AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
	
	// Create the RemoteIO object
	status = AudioComponentInstanceNew(inputComponent, &audioUnit);
	[self checkStatus: status];
	
	// Enable IO for playback
	UInt32 flag = 1;
	status = AudioUnitSetProperty(audioUnit, 
								  kAudioOutputUnitProperty_EnableIO, 
								  kAudioUnitScope_Output, 
								  0,
								  &flag, 
								  sizeof(flag));
	[self checkStatus: status];
		
	// Describe the stream format for element 0 of the input scope
	memset (&audioFormat, 0, sizeof (audioFormat));
	audioFormat.mSampleRate			= 44100.00;
	audioFormat.mFormatID			= kAudioFormatLinearPCM;
	audioFormat.mFormatFlags		= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked; //we are using all the available bits per channel (kAudioFormatFlagIsPacked)
	audioFormat.mFramesPerPacket	= 1; // a frame is... 
	audioFormat.mChannelsPerFrame	= 2; // two interleaved samples
	audioFormat.mBitsPerChannel		= 8 * sizeof(SInt16);
	audioFormat.mBytesPerPacket		= 2 * sizeof(SInt16);
	audioFormat.mBytesPerFrame		= 2 * sizeof(SInt16);
	
	//Apply format to element 0 of the input scope
	bool fromApp = 0;
	status = AudioUnitSetProperty(audioUnit, 
								  kAudioUnitProperty_StreamFormat, 
								  kAudioUnitScope_Input, 
								  fromApp, 
								  &audioFormat, 
								  sizeof(audioFormat));
	[self checkStatus: status];
	
	// Setup our render callback
	AURenderCallbackStruct callbackStruct;
	// This is our callback that we write our audio data into
	callbackStruct.inputProc = playbackCallback;
	//set the reference to "self" - this becomes *inRefCon in the playback callback
	callbackStruct.inputProcRefCon = self;
	//setup the callback
	status = AudioUnitSetProperty(audioUnit, 
								  kAudioUnitProperty_SetRenderCallback, 
								  kAudioUnitScope_Global, 
								  0,
								  &callbackStruct, 
								  sizeof(callbackStruct));
	
	[self checkStatus: status];
	// Finally initialize our readily setup RemoteIO
	status = AudioUnitInitialize(audioUnit);
	[self checkStatus: status];
	
	
}


-(OSStatus)start{
	
	OSStatus status = AudioOutputUnitStart(audioUnit);
	NSLog([NSString stringWithFormat:@"Audio Unit Start: %d"], status);
	return status;
}

-(OSStatus)stop{
	OSStatus status = AudioOutputUnitStop(audioUnit);
	return status;
}

-(void)cleanUp{
	
	AudioOutputUnitStop(audioUnit);
	AudioUnitUninitialize(audioUnit);
	AudioComponentInstanceDispose(audioUnit	);
	
	[inMemoryAudiofile dealloc];
	[inMemoryAudiofile release];
	
	delete[] channel1InputSampleBuffer;
	delete[] channel1OutputSampleBuffer;
	delete[] channel1MagnitudePhasesBuffer;
	delete[] channel1MagnitudePhasesFrame;
	delete[] fftWindow;
	
	delete[] bufferMatrix;
	
	
	[super dealloc];
	
}


@end
