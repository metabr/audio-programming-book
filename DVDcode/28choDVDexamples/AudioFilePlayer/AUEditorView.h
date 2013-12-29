//
//  AUEditorView.h
//  AU
//
//  Created by Taemin Cho on 1/2/09.
//  Copyright 2009 NYU. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioUnit/AUCocoaUIView.h>
#import <CoreAudioKit/CoreAudioKit.h>

@interface AUEditorView : NSWindowController {
	
	AudioUnit				mAudioUnit;
}

+ (AUEditorView*) editorForAudioUnit:(AudioUnit)unit;
+ (BOOL) pluginClassIsValid:(Class)pluginClass; 

- (id) initWithAudioUnit:(AudioUnit)unit;

- (NSView *) getCocoaView;
- (void) createView;
- (BOOL) hasCocoaView;

@end