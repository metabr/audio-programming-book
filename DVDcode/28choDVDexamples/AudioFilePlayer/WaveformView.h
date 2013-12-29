//
//  WaveformView.h
//  AudioPlayer
//
//  Created by Taemin Cho on 8/6/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface WaveformView : NSView {

	int numFrames;
	
	Float32 *mLeftBuffer, *mRightBuffer;
}

- (void)drawWaveformsOfLeft:(Float32 *)left right:(Float32 *)right frames:(int)frames;

@end
