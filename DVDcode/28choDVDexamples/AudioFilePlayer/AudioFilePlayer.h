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
#include "CAComponent.h"
#include "CAComponentDescription.h"
#include "AUEditorView.h"
#include "WaveformView.h"
#include "SpectralView.h"

@interface AudioFilePlayer : NSWindowController
{
    IBOutlet NSTextField	*filePath;
	IBOutlet NSButton		*playButton;
	IBOutlet NSPopUpButton	*mAUPopUpButton1, *mAUPopUpButton2, *mAUPopUpButton3, *mAUPopUpButton4;
	
	IBOutlet NSSlider		*mVolumeSlider, *mPanSlider;
	
	IBOutlet WaveformView		*waveformView;
	IBOutlet SpectralView		*spectralView;
	
	CAComponent					*mAUList;
	NSMutableDictionary			*mAUNodeDictionary;
	NSMutableDictionary			*mAUPanelDictionary;
	
	AUGraph						mGraph;
	AudioFileID					mAudioFile;
	ScheduledAudioFileRegion	mRegion;
	
	NSString					*mFilePath;
	
	AudioUnit					mOutputUnit, mPlayerUnit, mMixerUnit;
	AUNode						mOutputNode, mPlayerNode, mMixerNode;
}

@property(assign, readonly) NSSlider		*mVolumeSlider, *mPanSlider;
@property(assign, readonly) WaveformView	*waveformView;
@property(assign, readonly) SpectralView	*spectralView;

- (IBAction)open:(id)sender;
- (IBAction)play:(id)sender;
- (IBAction)stop:(id)sender;
- (IBAction)iaAUPopUpButtonPressed:(id)sender;
- (IBAction)openAUEditor:(id)sender;

- (OSStatus)initGraph;
- (OSStatus)loadSoundFromPath:(NSString *)theFileName;
- (void)printStreamDesc:(AudioStreamBasicDescription *)inDesc;

- (OSStatus)getFileInfoOf:(NSString *)theFilePath 
				   FileID:(AudioFileID *)fileID  
				 FileASBD:(AudioStreamBasicDescription *)fileASBD;

- (Float64)preparePlayUnit:(AudioUnit *)playerUnit 
				  FileASBD:(AudioStreamBasicDescription *)fileASBD 
					FileID:(AudioFileID)audioFile;

- (void)resetPlayerUnit:(AudioUnit *)playerUnit
				 Region:(ScheduledAudioFileRegion *)region;

- (void)auList;
- (void)connectAUGraph;

@end
