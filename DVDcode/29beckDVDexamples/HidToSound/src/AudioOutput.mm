//
//  AudioOutput.mm
//  TestHidOsc
//
//  Created by Andrew Beck on 1/18/08.
//

#import "AudioOutput.h"

OSStatus	MyRenderer(void 				*inRefCon, 
				AudioUnitRenderActionFlags 	*ioActionFlags, 
				const AudioTimeStamp 		*inTimeStamp, 
				unsigned long 				inBusNumber, 
				unsigned long 				inNumberFrames, 
				AudioBufferList 			*ioData)

{
	NSMutableArray* tempSines = [AudioOutput getSines];

	unsigned long nSampleFrames = inNumberFrames;
	float *sourceP = (float*) ioData->mBuffers[0].mData;

    while (nSampleFrames-- > 0) {
        *sourceP = (random() / (float)RAND_MAX)*0.00001;
		sourceP += 1;
	}

	int i, k=0;
	for (i=[tempSines count]-1; i>=0; i--)
	{
		[[tempSines objectAtIndex:i] render:inNumberFrames toBuf:ioData->mBuffers[0].mData
			atSR:[[AudioOutput getAudioOutput] getSr] inFormat:32];
		if ([[tempSines objectAtIndex:i] isDead])
			[tempSines removeObjectAtIndex:i];
		k=45;
	}

	//we're just going to copy the data into each channel
	int channel;
	for (channel = 1; channel < ioData->mNumberBuffers; channel++)
		memcpy (ioData->mBuffers[channel].mData, ioData->mBuffers[0].mData, ioData->mBuffers[0].mDataByteSize);

	return noErr;
}

@implementation AudioOutput

- (id) init
{
	[super init];
	
	sr = 44100;
	theFormatFlags =  kLinearPCMFormatFlagIsFloat 
						| kAudioFormatFlagsNativeEndian
						| kLinearPCMFormatFlagIsPacked
						| kAudioFormatFlagIsNonInterleaved;
	theBytesPerFrame = theBytesInAPacket = 4;
	theBitsPerChannel = 32;

	int bufSize = 1024;
	double i;
	for (i=0; i<bufSize; i=i+1)
		sinBuf[(int)i] = sin((i / bufSize) * (M_PI * 2.0));
	sinBuf[bufSize + 1] = sinBuf[0];

	for (i=0; i<bufSize; i=i+1)
		if (i > (bufSize/2))
			sqBuf[(int)i] = 1;
		else
			sqBuf[(int)i] = -1;
	sqBuf[bufSize + 1] = sqBuf[0];

	for (i=0; i<bufSize; i=i+1)
		sawBuf[(int)i] = (i/bufSize);
	sawBuf[bufSize + 1] = sawBuf[0];
	
	srand([[NSDate date] timeIntervalSince1970]);

	sines = [[[NSMutableArray alloc] init] retain];
	[self createAU];
	[self runAU];
	
	audioOutput = self;
	
	return self;
}

- (void) createAU
{
	OSStatus err = noErr;

	// Open the default output unit
	ComponentDescription desc;
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_DefaultOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	
	Component comp = FindNextComponent(NULL, &desc);
	if (comp == NULL) { NSLog (@"FindNextComponent\n"); return; }
	
	err = OpenAComponent(comp, &outputUnit);
	if (comp == NULL) { NSLog (@"OpenAComponent=%ld\n", err); return; }

	// Set up a callback function to generate output to the output unit
    AURenderCallbackStruct input;
	input.inputProc = &MyRenderer;
	input.inputProcRefCon = NULL;

	err = AudioUnitSetProperty (outputUnit, 
								kAudioUnitProperty_SetRenderCallback, 
								kAudioUnitScope_Input,
								0, 
								&input, 
								sizeof(input));
	if (err) { NSLog (@"AudioUnitSetProperty-CB=%ld\n", err); return; }
}

- (void) runAU
{
	OSStatus err = noErr;
    
	// We tell the Output Unit what format we're going to supply data to it
	// this is necessary if you're providing data through an input callback
	// AND you want the DefaultOutputUnit to do any format conversions
	// necessary from your format to the device's format.
	AudioStreamBasicDescription streamFormat;
		streamFormat.mSampleRate = sr;		//	the sample rate of the audio stream
		streamFormat.mFormatID = kAudioFormatLinearPCM;			//	the specific encoding type of audio stream
		streamFormat.mFormatFlags = theFormatFlags;		//	flags specific to each format
		streamFormat.mBytesPerPacket = theBytesInAPacket;	
		streamFormat.mFramesPerPacket = 1;	
		streamFormat.mBytesPerFrame = theBytesPerFrame;		
		streamFormat.mChannelsPerFrame = 1;	
		streamFormat.mBitsPerChannel = theBitsPerChannel;	
	
	err = AudioUnitSetProperty (outputUnit,
							kAudioUnitProperty_StreamFormat,
							kAudioUnitScope_Input,
							0,
							&streamFormat,
							sizeof(AudioStreamBasicDescription));
	if (err) {  NSLog (@"AudioUnitSetProperty-SF=%4.4s, %ld\n", (char*)&err, err); return; }
	
    // Initialize unit
	err = AudioUnitInitialize(outputUnit);
	if (err) {  NSLog (@"AudioUnitInitialize=%ld\n", err); return; }
    
	Float64 outSampleRate;
	UInt32 size = sizeof(Float64);
	err = AudioUnitGetProperty (outputUnit,
							kAudioUnitProperty_SampleRate,
							kAudioUnitScope_Output,
							0,
							&outSampleRate,
							&size);
	if (err) {  NSLog (@"AudioUnitSetProperty-GF=%4.4s, %ld\n", (char*)&err, err); return; }

	// Start the rendering
	// The DefaultOutputUnit will do any format conversions to the format of the default device
	err = AudioOutputUnitStart (outputUnit);
	if (err) { NSLog (@"AudioOutputUnitStart=%ld\n", err); return; }
			
			// RUNS FOREVER!!!!
			// we call the CFRunLoopRunInMode to service any notifications that the audio
			// system has to deal with
//	usleep (5 * 1000 * 1000); // sleep for 5 seconds

// REALLY after you're finished playing STOP THE AUDIO OUTPUT UNIT!!!!!!	
// but we never get here because we're running until the process is nuked...	
//	verify_noerr (AudioOutputUnitStop (outputUnit));
//	
//    err = AudioUnitUninitialize (outputUnit);
//	if (err) { printf ("AudioUnitUninitialize=%ld\n", err); return; }
}

+ (AudioOutput*) getAudioOutput
{
	return audioOutput;
}

+ (NSMutableArray*) getSines
{
	return sines;
}

- (double) getSr
{
	return sr;
}

- (void) elementUpdated:(HidElement*) element
{
	if ([element value] != 0)
	{
		NSArray* cItems = [[element string] componentsSeparatedByString:@", "];
		SineEvent* temp = NULL;
		float bot=0;
		float mult=0;
		int i;
		for (i=0; i<[cItems count]; i++)
		{
			switch ([[cItems objectAtIndex:i] characterAtIndex:0])
			{
				case 's':
				case 'S':
					if (temp != NULL)
					{
						[[AudioOutput getSines] addObject:temp];
						temp = NULL;
					}
					temp = [[SineEvent alloc] initWithBuf:sinBuf];
					break;
				case 'q':
				case 'Q':
					if (temp != NULL)
					{
						[[AudioOutput getSines] addObject:temp];
						temp = NULL;
					}
					temp = [[SineEvent alloc] initWithBuf:sqBuf];
					break;
				case 'w':
				case 'W':
					if (temp != NULL)
					{
						[[AudioOutput getSines] addObject:temp];
						temp = NULL;
					}
					temp = [[SineEvent alloc] initWithBuf:sawBuf];
					break;
				case 'f':
				case 'F':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y > 0)
							[temp setFreq:y];
					}
					break;
				case 'g':
				case 'G':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y > 0)
							[temp setFinalFreqRatio:y];
					}
					break;
				case 'e':
				case 'E':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y > 0)
							[temp setDelay:(int) (y*[self getSr])];
					}
					break;
				case 'h':
				case 'H':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y > 0)
							[temp setFinalFreq:y];
					}
					break;
				case 'a':
				case 'A':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y != 0)
							[temp setAmp:y];
					}
					break;
				case 'b':
				case 'B':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y != 0)
							[temp setAtk:(y*[self getSr])];
					}
					break;
				case 'd':
				case 'D':
					if (temp != NULL)
					{
						float y = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
						if (y > 0)
							[temp setDuration:(y*[self getSr])];
					}
					break;
				case 'v':
				case 'V':
					if (temp != NULL)
					{
						bot = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
					}
					break;
				case 'x':
				case 'X':
					if (temp != NULL)
					{
						if (bot != 0)
						{
							float top = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
							if (top > 0)
							{
								float scale = top / bot;
								float fre = exp(log(scale)*(random() / (float)RAND_MAX))*bot;
								[temp setFreq:fre];
								bot = 0;
							}
						}
					}
					break;
				case 'y':
				case 'Y':
					if (temp != NULL)
					{
						if (bot != 0)
						{
							float top = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
							if (top > 0)
							{
								float scale = top / bot;
								float amp = exp(log(scale)*(random() / (float)RAND_MAX))*bot;
								[temp setAmp:amp];
								bot = 0;
							}
						}
					}
					break;
				case 'z':
				case 'Z':
					if (temp != NULL)
					{
						if (bot != 0)
						{
							float top = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
							if (top > 0)
							{
								float dur = ((top-bot)*(random() / (float)RAND_MAX))+bot;
								[temp setDuration:dur*[self getSr]];
								bot = 0;
							}
						}
					}
					break;
				case 'm':
				case 'M':
					if (temp != NULL)
					{
						mult = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
					}
					break;
				case 'n':
				case 'N':
					if (temp != NULL)
					{
						if (mult != 0)
						{
							float add = [[[cItems objectAtIndex:i] substringFromIndex:1] floatValue];
							float fre = ([element value] * mult) + add;
							[temp setFreq:fre];
						}
					}
					break;

				default:
					break;
			}
		}
		if (temp != NULL)
		{
			[[AudioOutput getSines] addObject:temp];
			temp = NULL;
		}
	}
}

@end

