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

static OSStatus RenderCallback(	void *							inRefCon,
							   AudioUnitRenderActionFlags *	ioActionFlags,
							   const AudioTimeStamp *			inTimeStamp,
							   UInt32							inBusNumber,
							   UInt32							inNumberFrames,
							   AudioBufferList *				ioData)
{
	if (*ioActionFlags == kAudioUnitRenderAction_PostRender) {
		AudioFilePlayer *This = (AudioFilePlayer *)inRefCon;
		if (!(*ioActionFlags & kAudioUnitRenderAction_PostRenderError)) { // ... (1)
			
			Float32 *left = (Float32 *)ioData->mBuffers[0].mData;	// ... (2)
			Float32 *right = (Float32 *)ioData->mBuffers[1].mData;	// ... (3)
			
			for(int i = 0; i<inNumberFrames; i++)
			{	
				left[i] *= [This.mVolumeSlider floatValue] * (1.0 - [This.mPanSlider floatValue]);
				right[i] *= [This.mVolumeSlider floatValue] * [This.mPanSlider floatValue];
			}
			
			[This.waveformView drawWaveformsOfLeft:left right:right frames:inNumberFrames];
			[This.spectralView drawSpectrumFromLeft:left right:right frames:inNumberFrames];
		}
	}
	return noErr;
}

int componentCountForAUType(OSType inAUType)
{
	CAComponentDescription desc = CAComponentDescription(inAUType);
	return desc.Count();
}

void getComponentsForAUType(OSType inAUType, CAComponent *ioCompBuffer, int count)
{
	CAComponentDescription desc = CAComponentDescription(inAUType);
	CAComponent *last = NULL;
	
	for (int i = 0; i < count; ++i) {
		ioCompBuffer[i] = CAComponent(desc, last);
		last = &(ioCompBuffer[i]);
	}
}

@implementation AudioFilePlayer

@synthesize mVolumeSlider, mPanSlider;
@synthesize waveformView, spectralView;

- (id)init
{
	if(self = [super initWithWindow:nil]){
		[self initGraph];
	}
	return self;
}

- (void)awakeFromNib
{
	mAUNodeDictionary = [[NSMutableDictionary dictionaryWithCapacity:0] retain];
	mAUPanelDictionary = [[NSMutableDictionary dictionaryWithCapacity:0] retain];
	[self auList];
}

- (void)auList
{
	// get AU effect list
	int componentCount = componentCountForAUType(kAudioUnitType_Effect);	// ...... (1)
	UInt32 dataByteSize = componentCount * sizeof(CAComponent);
	mAUList = (CAComponent *)malloc(dataByteSize);							// ...... (2)
	memset (mAUList, 0, dataByteSize);
	getComponentsForAUType(kAudioUnitType_Effect, mAUList, componentCount); // ...... (3)
	
	[mAUPopUpButton1 removeAllItems];										// ...... (4)
	[mAUPopUpButton1 addItemWithTitle:@"None"];								// ...... (5)
	[mAUPopUpButton2 removeAllItems];
	[mAUPopUpButton2 addItemWithTitle:@"None"];
	[mAUPopUpButton3 removeAllItems];
	[mAUPopUpButton3 addItemWithTitle:@"None"];
	[mAUPopUpButton4 removeAllItems];
	[mAUPopUpButton4 addItemWithTitle:@"None"];
	
	for (int i = 0; i < componentCount; ++i) {
		[mAUPopUpButton1 addItemWithTitle:(NSString *)(mAUList[i].GetAUName())]; // .. (6)
		[mAUPopUpButton2 addItemWithTitle:(NSString *)(mAUList[i].GetAUName())];
		[mAUPopUpButton3 addItemWithTitle:(NSString *)(mAUList[i].GetAUName())];
		[mAUPopUpButton4 addItemWithTitle:(NSString *)(mAUList[i].GetAUName())];
	}    
}

- (void)connectAUGraph
{
	ERR(AUGraphClearConnections(mGraph)); // .... (1)
	
	AUNode source, dest;
	source = mPlayerNode;				// .... (2)
	for(int tag = 0; tag < 4; tag++){			// .... (3)
		dest = (AUNode)[[mAUNodeDictionary objectForKey:[NSNumber numberWithInt:tag]] longValue]; // ... (4)
		if(dest){						// ... (5)
			ERR(AUGraphConnectNodeInput (mGraph, source, 0, dest, 0)); // ... (6)
			source = dest;				// ... (7)
		}
	}
	
	ERR(AUGraphConnectNodeInput (mGraph, source, 0, mMixerNode, 0)); // ... (8)
	ERR(AUGraphConnectNodeInput (mGraph, mMixerNode, 0, mOutputNode, 0)); // ... (9)
	ERR(AUGraphUpdate(mGraph, NULL));	// ... (10)
}

- (IBAction)iaAUPopUpButtonPressed:(id)sender
{
	int index = [sender indexOfSelectedItem];				// ... (1)
	NSNumber *tag = [NSNumber numberWithInt:[sender tag]];	// ... (2)
	
	// Remove an old Node
	if([mAUNodeDictionary objectForKey:tag]) {	// ... (3)
		
		// remove the old view first before closing the AU
		[[mAUPanelDictionary objectForKey:tag] close];
		[mAUPanelDictionary removeObjectForKey:tag];
		
		AUNode oldNode;
		oldNode = (AUNode) [[mAUNodeDictionary objectForKey:tag] longValue]; // ... (4)
		ERR(AUGraphRemoveNode(mGraph, oldNode));			// ... (5)
		[mAUNodeDictionary removeObjectForKey:tag];			// ... (7)
	}
	
	// Add a new Node
	if(index){												// ... (8)
		AUNode	newNode;
		AudioUnit unit;
		CAComponentDescription desc = mAUList[index - 1].Desc();	// ... (9)
	
		ERR(AUGraphAddNode(mGraph, &desc, &newNode));				// ... (10)
		ERR(AUGraphNodeInfo(mGraph, newNode, NULL, &unit));
	
		[mAUNodeDictionary setObject:[NSNumber numberWithLong:(long)newNode] forKey:tag]; // ... (11)
	
		[self connectAUGraph];												// ... (3)
		AUEditorView *aueditor = [AUEditorView editorForAudioUnit:unit];	// ... (4)
		[mAUPanelDictionary setObject:aueditor forKey:tag];					// ... (5)
		[aueditor showWindow:nil];											// ... (6)
		return;																// ... (7)
	}
	
	[self connectAUGraph];			// ... (12)
}

- (IBAction)openAUEditor:(id)sender
{
	NSNumber *tag = [NSNumber numberWithInt:[sender tag]];
	[[mAUPanelDictionary objectForKey:tag] showWindow:nil];
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
	
	// Mixer AU node
	desc.componentType = kAudioUnitType_Mixer;
	desc.componentSubType = kAudioUnitSubType_StereoMixer;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	err = AUGraphAddNode(mGraph, &desc, &mMixerNode);					// .... (1)
	RETURN_ERR(err);
	
	// Open Audio Unit Graph	
	err = AUGraphOpen (mGraph);
	RETURN_ERR(err);
	
	// Get Audio Units
	err = AUGraphNodeInfo(mGraph, mOutputNode, NULL, &mOutputUnit);
	RETURN_ERR(err);	
	err = AUGraphNodeInfo(mGraph, mPlayerNode, NULL, &mPlayerUnit);
	RETURN_ERR(err);
	err = AUGraphNodeInfo(mGraph, mMixerNode, NULL, &mMixerUnit);		// .... (2)
	RETURN_ERR(err);
	
	err  = AudioUnitAddRenderNotify(mMixerUnit, RenderCallback, self);
	RETURN_ERR(err);
	
	// Connect Audio Units		
	err = AUGraphConnectNodeInput (mGraph, mPlayerNode, 0, mMixerNode, 0); // .... (3)
	RETURN_ERR(err);
	err = AUGraphConnectNodeInput (mGraph, mMixerNode, 0, mOutputNode, 0); // .... (4)
	RETURN_ERR(err);
	
	// Initialize Audio Unit Graph	
	err = AUGraphInitialize (mGraph);
	RETURN_ERR(err);					  
	
RETURN:
	return err;
}

- (IBAction)open:(id)sender
{
	int result;
	OSStatus err = noErr;
	
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    NSArray *filesToOpen;
	// All file types NSSound understands
    NSArray *fileTypes = [NSSound soundUnfilteredTypes];
		
    [oPanel setAllowsMultipleSelection:NO];
	
    result = [oPanel runModalForDirectory:NSHomeDirectory() 
									 file:nil 
									types:fileTypes];
	
    if (result == NSOKButton) {
		if(mFilePath != NULL){
	
			Boolean outIsRunning;
			AUGraphIsRunning(mGraph, &outIsRunning);
			
			if(outIsRunning){
				AUGraphStop(mGraph);
				AudioFileClose (mAudioFile); 
			}else
				AudioFileClose(mAudioFile);
		}
		
        filesToOpen = [oPanel filenames];
        mFilePath = [filesToOpen objectAtIndex:0];
        [filePath setStringValue:mFilePath];
		
        err = [self loadSoundFromPath:mFilePath];
		
		
		if(err){
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
	Float64 fileDuration;
		
    AudioStreamBasicDescription fileASBD;
    
    err = [self getFileInfoOf:theFilePath 
					   FileID:&mAudioFile 
					 FileASBD:&fileASBD];  
    RETURN_ERR(err); 
	
	err = AudioUnitSetProperty(mPlayerUnit, kAudioUnitProperty_ScheduledFileIDs, 
							   kAudioUnitScope_Global, 0, &mAudioFile, sizeof(mAudioFile));
	RETURN_ERR(err);
	
	// now we load the file contents up for playback before we start playing
	fileDuration = [self preparePlayUnit:&mPlayerUnit 
								FileASBD:&fileASBD 
								  FileID:mAudioFile];

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
	
	size = sizeof(AudioStreamBasicDescription);
	memset(fileASBD, 0, size);

	//Fetch the AudioStreamBasicDescription of the audio file.
	err = AudioFileGetProperty(*fileID, kAudioFilePropertyDataFormat, &size, fileASBD); 
	RETURN_ERR(err);

	[self printStreamDesc: fileASBD];  

RETURN:
	return err;
}

- (Float64)preparePlayUnit:(AudioUnit *)playerUnit 
				  FileASBD:(AudioStreamBasicDescription *)fileASBD 
					FileID:(AudioFileID)audioFile
{	
	UInt64 nPackets;
	UInt32 propsize = sizeof(nPackets);
	AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataPacketCount, &propsize, &nPackets); 
	
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
	mRegion.mFramesToPlay = (UInt32)(nPackets * fileASBD->mFramesPerPacket); 
	
	// must set this property on scheduled file player for all files to be played
	AudioUnitSetProperty (*playerUnit, kAudioUnitProperty_ScheduledFileRegion, 
						  kAudioUnitScope_Global, 0,&mRegion, sizeof(mRegion));
	
	// AudioTimeStamp, with either sample time or host time valid. Sample time
	// takes precedence, -1 means "now". Host time of 0 means "now."
	AudioTimeStamp startTime;
	memset (&startTime, 0, sizeof(startTime));
	startTime.mFlags = kAudioTimeStampSampleTimeValid;
	startTime.mSampleTime = -1; 
	
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
		ERR(AUGraphStart(mGraph)); 
		[playButton setTitle:@"Pause"];
	}
	else
	{
		ERR(AUGraphStop(mGraph)); 
		[playButton setTitle:@"Play"];
		
		AudioTimeStamp pauseTime;
		UInt32 size = sizeof(pauseTime);
		AudioUnitGetProperty(mPlayerUnit, kAudioUnitProperty_CurrentPlayTime, 
							 kAudioUnitScope_Global, 0, &pauseTime, &size); 
		
		mRegion.mStartFrame += pauseTime.mSampleTime; 
		
		if(mRegion.mFramesToPlay > mRegion.mStartFrame)
			[self resetPlayerUnit:&mPlayerUnit Region:&mRegion]; 
		else
		{
			[self stop:nil]; 
			ERR(AUGraphStart(mGraph));
		}
	}
}

- (IBAction)stop:(id)sender 
{	
	if(mFilePath) {
		
		ERR(AUGraphStop (mGraph));
		[playButton setTitle:@"Play"];
		mRegion.mStartFrame = 0; 
		[self resetPlayerUnit:&mPlayerUnit Region:&mRegion];
	}
}

- (void)resetPlayerUnit:(AudioUnit *)playerUnit
				 Region:(ScheduledAudioFileRegion *)region
{
	AudioUnitReset(*playerUnit, kAudioUnitScope_Global, 0); 
	
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
	
	[mAUNodeDictionary release];
	
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
