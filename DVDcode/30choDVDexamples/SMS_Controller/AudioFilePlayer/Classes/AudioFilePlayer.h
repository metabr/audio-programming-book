/*
 *  AudioFilePlayer.h
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 1/10/08.
 *  Copyright 2008 __Taemin Cho__. All rights reserved.
 *
 */

#import <Cocoa/Cocoa.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioUnit/AudioUnit.h>

@interface AudioFilePlayer : NSWindowController
{
    IBOutlet NSTextField *filePath;
	IBOutlet NSButton *playButton;
	
	AUGraph						mGraph;
	AudioFileID					mAudioFile;
	ScheduledAudioFileRegion	mRegion;
	
	NSString					*mFilePath;
	
	AudioUnit					mOutputUnit, mPlayerUnit, mEffect1Unit, mEffect2Unit;
	AUNode						mOutputNode, mPlayerNode, mEffect1Node, mEffect2Node;
}
- (IBAction)open:(id)sender;
- (IBAction)play:(id)sender;
- (IBAction)stop:(id)sender;

- (OSStatus)initGraph;
- (OSStatus)loadSoundFromPath:(NSString *)theFilePath;
- (void)printStreamDesc:(AudioStreamBasicDescription *)inDesc;

- (OSStatus)getFileInfoOf:(NSString *)theFilePath 
				   FileID:(AudioFileID *)fileID  
				 FileASBD:(AudioStreamBasicDescription *)fileASBD;

- (Float64)preparePlayUnit:(AudioUnit *)playerUnit 
				  FileASBD:(AudioStreamBasicDescription *)fileASBD 
					FileID:(AudioFileID)audioFile;

- (void)resetPlayerUnit:(AudioUnit *)playerUnit
				 Region:(ScheduledAudioFileRegion *)region;
				 
- (void)setEffect1Parameter:(Float32)cutOff 
				 Parameter2:(Float32)resonance;

- (void)setEffect2Parameter:(Float32)playbackRate;

@end
