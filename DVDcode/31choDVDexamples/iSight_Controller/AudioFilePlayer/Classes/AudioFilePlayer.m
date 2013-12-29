/*
 *  AudioFilePlayer.m
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 1/10/08.
 *  Copyright 2008 __Taemin Cho__. All rights reserved.
 *
 */
 
#import "AudioFilePlayer.h"
#import "AFP_DebugMacros.h" // debug macros

@implementation AudioFilePlayer

- (id)init
{
	self = [super initWithWindowNibName:@"AudioFilePlayer"];
	if(self){
		mFilePath = NULL;
		if([self initGraph]){ // call initGraph to make AudioUnitGraph.
			[super dealloc];
			self = nil;
		}
	}
	else{
		[super dealloc];
		self = nil;
	}
	return self;
}

- (OSStatus)initGraph
{
	OSStatus err = noErr;
	
	err = NewAUGraph(&mGraph);
	RETURN_ERR(err);

#if AUDIO_UNIT_VERSION >= 1060 // if SDK 10.6 (Snow Leopard)	
	AudioComponentDescription desc;  
#else
	ComponentDescription desc;  
#endif
	
//Create Audio Units and those Nodes		
	// AudioFilePlayer
	desc.componentType = kAudioUnitType_Generator; 
	desc.componentSubType = kAudioUnitSubType_AudioFilePlayer;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	
	err = AUGraphAddNode (mGraph, &desc, &mPlayerNode);
	RETURN_ERR(err);
	
	// Output AU node
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_DefaultOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	err = AUGraphAddNode (mGraph, &desc, &mOutputNode);
	RETURN_ERR(err);
	
	// Effect1 AU nodes
	desc.componentType = kAudioUnitType_Effect;
	desc.componentSubType = kAudioUnitSubType_LowPassFilter;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	err = AUGraphAddNode (mGraph, &desc, &mEffect1Node);
	RETURN_ERR(err);
	
	// Effect2 AU nodes	
	desc.componentType = kAudioUnitType_FormatConverter;
	desc.componentSubType = kAudioUnitSubType_Varispeed;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;

	err = AUGraphAddNode (mGraph, &desc, &mEffect2Node);
	RETURN_ERR(err);

// Open Audio Unit Graph	
	err = AUGraphOpen (mGraph);
	RETURN_ERR(err);

// Get Audio Units
	err = AUGraphNodeInfo(mGraph, mOutputNode, NULL, &mOutputUnit);
	RETURN_ERR(err);	
	err = AUGraphNodeInfo(mGraph, mPlayerNode, NULL, &mPlayerUnit);
	RETURN_ERR(err);
	err = AUGraphNodeInfo(mGraph, mEffect1Node, NULL, &mEffect1Unit);
	RETURN_ERR(err);
	err = AUGraphNodeInfo(mGraph, mEffect2Node, NULL, &mEffect2Unit);
	RETURN_ERR(err);
		
// Connect Audio Units		
	err = AUGraphConnectNodeInput (mGraph, mPlayerNode, 0, mEffect1Node, 0);
	RETURN_ERR(err);
	err = AUGraphConnectNodeInput (mGraph, mEffect1Node, 0, mEffect2Node, 0);
	RETURN_ERR(err);
	err = AUGraphConnectNodeInput (mGraph, mEffect2Node, 0, mOutputNode, 0);
	RETURN_ERR(err);

// Initialize Audio Unit Graph	
	err = AUGraphInitialize (mGraph);
	RETURN_ERR(err);					  
	
RETURN:
		return err;
}

// Setter methods.

- (void)setEffect1Parameter:(Float32)cutOff 
				 Parameter2:(Float32)resonance
{						  
	AudioUnitSetParameter(mEffect1Unit,
						  kLowPassParam_CutoffFrequency,
						  kAudioUnitScope_Global,
						  0, cutOff, 0);

	AudioUnitSetParameter(mEffect1Unit,
						  kLowPassParam_Resonance,
						  kAudioUnitScope_Global,
						  0, resonance, 0);						  
}

- (void)setEffect2Parameter:(Float32)playbackRate
{	
	AudioUnitSetParameter(mEffect2Unit,
						  kVarispeedParam_PlaybackRate,
						  kAudioUnitScope_Global,
						  0, playbackRate, 0);	
}

- (IBAction)open:(id)sender
{
	int result;
	OSStatus err = noErr;
	
    NSOpenPanel *oPanel = [NSOpenPanel openPanel]; // ....... (1)
    NSArray *filesToOpen;
	// All file types NSSound understands
    NSArray *fileTypes = [NSSound soundUnfilteredTypes]; // ....... (2)
		
    [oPanel setAllowsMultipleSelection:NO]; // ...... (3)
	
    result = [oPanel runModalForDirectory:NSHomeDirectory() 
									 file:nil 
									types:fileTypes];  // ........(4)
	
    if (result == NSOKButton) {
		if(mFilePath != NULL){
	
			Boolean outIsRunning;
			AUGraphIsRunning(mGraph, &outIsRunning);
			
			if(outIsRunning){		// .......... (5)
				AUGraphStop(mGraph);
				AudioFileClose (mAudioFile); 
			}else
				AudioFileClose(mAudioFile);
		}
		
        filesToOpen = [oPanel filenames];
        mFilePath = [filesToOpen objectAtIndex:0];
        [filePath setStringValue:mFilePath];
		
        err = [self loadSoundFromPath:mFilePath]; // ........ (6)
		
		
		if(err){									         // .. (7)
			[filePath setStringValue:@"Sound failed to load..."];
			mFilePath = NULL;
			[playButton setEnabled:NO];
		}
		else
			[playButton setEnabled:YES];
	}
}

- (OSStatus)loadSoundFromPath:(NSString *)theFilePath 
{
	OSStatus err = noErr;
	
	//const char* inputFile = [theFilePath cStringUsingEncoding:NSASCIIStringEncoding];
	
    AudioStreamBasicDescription fileASBD;  // ....... (1)
    
    err = [self getFileInfoOf:theFilePath 
					   FileID:&mAudioFile 
					 FileASBD:&fileASBD];  
    RETURN_ERR(err); 
	
	err = AudioUnitSetProperty(mPlayerUnit, kAudioUnitProperty_ScheduledFileIDs, 
							   kAudioUnitScope_Global, 0, &mAudioFile, sizeof(mAudioFile)); // .. (2)
	RETURN_ERR(err);
	
	// now we load the file contents up for playback before we start playing
	Float64 fileDuration = [self preparePlayUnit:&mPlayerUnit 
										FileASBD:&fileASBD 
										  FileID:mAudioFile];  // ...... (3)

	printf ("file duration: %f secs\n", fileDuration);

RETURN:	
	return err;
}

- (OSStatus)getFileInfoOf:(NSString *)theFilePath 
				   FileID:(AudioFileID *)fileID  
				 FileASBD:(AudioStreamBasicDescription *)fileASBD
{
    OSStatus err= noErr;
    UInt32 size;
	
#if AUDIO_TOOLBOX_VERSION >= 1060  // if SDK 10.6 (Snow Leopard)
	
	NSURL * fileURL = [NSURL fileURLWithPath:theFilePath]; // ... (1)
	
	err = AudioFileOpenURL((CFURLRef)fileURL, kAudioFileReadPermission,0,fileID);   //Obtain AudioFileID
	
#else
	
	const char* inputFile = [theFilePath cStringUsingEncoding:NSASCIIStringEncoding];
	
	FSRef fileRef;			// ... (1)
	
	FSPathMakeRef ((const UInt8 *)inputFile, &fileRef, 0); //Obtain filesystem reference to the file
	err = AudioFileOpen(&fileRef, fsRdPerm,0,fileID);   //Obtain AudioFileID
	
#endif
	
	RETURN_ERR(err);
	
	size = sizeof(AudioStreamBasicDescription);  // .... (2)
	memset(fileASBD, 0, size);

	//Fetch the AudioStreamBasicDescription of the audio file.
	err = AudioFileGetProperty(*fileID, kAudioFilePropertyDataFormat, &size, fileASBD); // .. (3)
	RETURN_ERR(err);

	[self printStreamDesc: fileASBD]; // ....... (4)

RETURN:
		return err;
}

- (Float64)preparePlayUnit:(AudioUnit *)playerUnit 
				  FileASBD:(AudioStreamBasicDescription *)fileASBD 
					FileID:(AudioFileID)audioFile
{	
	UInt64 nPackets;
	UInt32 propsize = sizeof(nPackets);
	AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataPacketCount, &propsize, &nPackets); // .. (1)
	
	// calculate the duration in second.
	Float64 fileDuration = (nPackets * fileASBD->mFramesPerPacket) / fileASBD->mSampleRate;
	
	memset (&mRegion.mTimeStamp, 0, sizeof(mRegion.mTimeStamp));
	mRegion.mTimeStamp.mFlags = kAudioTimeStampSampleTimeValid;
	mRegion.mTimeStamp.mSampleTime = 0;
	mRegion.mCompletionProc = NULL;
	mRegion.mCompletionProcUserData = NULL;
	mRegion.mAudioFile = audioFile;
	mRegion.mLoopCount = 0;
	mRegion.mStartFrame = 0;
	mRegion.mFramesToPlay = (UInt32)(nPackets * fileASBD->mFramesPerPacket); // ....(2)
	
	// must set this property on scheduled file player for all files to be played
	AudioUnitSetProperty (*playerUnit, kAudioUnitProperty_ScheduledFileRegion, 
						  kAudioUnitScope_Global, 0,&mRegion, sizeof(mRegion));
	
	// AudioTimeStamp, with either sample time or host time valid. Sample time
	// takes precedence, -1 means "now". Host time of 0 means "now."
	AudioTimeStamp startTime;
	memset (&startTime, 0, sizeof(startTime));
	startTime.mFlags = kAudioTimeStampSampleTimeValid;
	startTime.mSampleTime = -1;                                // ..............(3)
	
	AudioUnitSetProperty(*playerUnit, kAudioUnitProperty_ScheduleStartTimeStamp, 
						 kAudioUnitScope_Global, 0, &startTime, sizeof(startTime));
	
	return fileDuration;
}

- (void)printStreamDesc:(AudioStreamBasicDescription *)fileASBD
{
	if (!fileASBD) {
		printf ("Can't print a NULL desc!\n");
		return;
	}
	
	UInt32 * formatID = &fileASBD->mFormatID;
	*formatID = EndianU32_NtoB(*formatID); 
	
	printf ("File format is:\n");
	printf ("- - - - - - - - - - - - - - - - - - - -\n");
	printf ("  Sample Rate:%f\n", fileASBD->mSampleRate);
	printf ("  Format ID:%s\n", (char*)formatID);
	printf ("  Format Flags:%lX\n", (long unsigned int)fileASBD->mFormatFlags);
	printf ("  Bytes per Packet:%ld\n", (long int)fileASBD->mBytesPerPacket);
	printf ("  Frames per Packet:%ld\n", (long int)fileASBD->mFramesPerPacket);
	printf ("  Bytes per Frame:%ld\n", (long int)fileASBD->mBytesPerFrame);
	printf ("  Channels per Frame:%ld\n", (long int)fileASBD->mChannelsPerFrame);
	printf ("  Bits per Channel:%ld\n", (long int)fileASBD->mBitsPerChannel);
	printf ("- - - - - - - - - - - - - - - - - - - -\n");
}

- (IBAction)play:(id)sender
{
	Boolean outIsRunning;
	AUGraphIsRunning(mGraph, &outIsRunning);
	
	if(!outIsRunning){
		ERR(AUGraphStart(mGraph)); // ....... (1)
		[playButton setTitle:@"Pause"];
	}
	else
	{
		ERR(AUGraphStop(mGraph)); // ........ (2)
		[playButton setTitle:@"Play"];
		
		AudioTimeStamp pauseTime;
		UInt32 size = sizeof(pauseTime);
		AudioUnitGetProperty(mPlayerUnit, kAudioUnitProperty_CurrentPlayTime, 
							 kAudioUnitScope_Global, 0, &pauseTime, &size); // ... (3)
		
		mRegion.mStartFrame += pauseTime.mSampleTime; // ...... (4)
		
		if(mRegion.mFramesToPlay > mRegion.mStartFrame)
			[self resetPlayerUnit:&mPlayerUnit Region:&mRegion]; // .... (5)
		else
		{
			[self stop:nil]; // ..... (6)
			ERR(AUGraphStart(mGraph));
		}
	}
}

- (IBAction)stop:(id)sender 
{	
	if(mFilePath) {
		
		ERR(AUGraphStop (mGraph));
		[playButton setTitle:@"Play"];
		mRegion.mStartFrame = 0;        // ........ (7)
		[self resetPlayerUnit:&mPlayerUnit Region:&mRegion];
	}
}

- (void)resetPlayerUnit:(AudioUnit *)playerUnit
				 Region:(ScheduledAudioFileRegion *)region
{
	AudioUnitReset(*playerUnit, kAudioUnitScope_Global, 0); // .... (1)
	
	AudioUnitSetProperty (*playerUnit, kAudioUnitProperty_ScheduledFileRegion, 
						  kAudioUnitScope_Global, 0, region, sizeof(*region));
	
	AudioTimeStamp startTime;
	memset (&startTime, 0, sizeof(startTime));
	startTime.mFlags = kAudioTimeStampSampleTimeValid;
	startTime.mSampleTime = -1;
	AudioUnitSetProperty(*playerUnit, kAudioUnitProperty_ScheduleStartTimeStamp, 
						 kAudioUnitScope_Global, 0, &startTime, sizeof(startTime));
}

- (void)dealloc
{
	printf("AudioFilePlayer Deallocating ... \n");
	
	Boolean outIsRunning;
	AUGraphIsRunning(mGraph, &outIsRunning);
	
	if(outIsRunning)
		ERR(AUGraphStop (mGraph));
		
	if(mFilePath)
		ERR(AudioFileClose (mAudioFile));
	
	ERR(AUGraphUninitialize (mGraph));
	ERR(AUGraphClose (mGraph));
	ERR(DisposeAUGraph(mGraph));
	
	[super dealloc];
}
@end
