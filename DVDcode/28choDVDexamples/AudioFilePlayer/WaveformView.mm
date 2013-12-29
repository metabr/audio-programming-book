//
//  WaveformView.m
//  AudioPlayer
//
//  Created by Taemin Cho on 8/6/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "WaveformView.h"


@implementation WaveformView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		numFrames = 0;
		mLeftBuffer = NULL;
		mRightBuffer = NULL;
    }
    return self;
}

- (void)drawRect:(NSRect)rect {
	
	// Draw a rectangle background
	[[NSColor blackColor] setFill];		// ... (1)
	NSRectFill(rect);					// ... (2)
	
	CGFloat v_center = rect.size.height / 2.0;
	NSBezierPath* deviderPath = [NSBezierPath bezierPath];
	[deviderPath moveToPoint:NSMakePoint(0, v_center)];					// ... (3)
	[deviderPath lineToPoint:NSMakePoint(rect.size.width, v_center)];	// ... (4)
	[deviderPath setLineWidth:1.0];
	[[NSColor yellowColor] setStroke];
	[deviderPath stroke];
	
	// Draw waveform
	NSBezierPath* leftWavePath = [NSBezierPath bezierPath];
	NSBezierPath* rightWavePath = [NSBezierPath bezierPath];
	
	CGFloat leftCenter = v_center + v_center * 0.5;
	CGFloat rightCenter = v_center - v_center * 0.5;
	CGFloat height = rect.size.height * 0.5;
	[leftWavePath moveToPoint:NSMakePoint(0, leftCenter)];	// ... (5)
	[rightWavePath moveToPoint:NSMakePoint(0, rightCenter)];

	for(int i = 0; i < numFrames; i++)						// ... (6)
	{	
		CGFloat x = i * rect.size.width / numFrames;		// ... (7)
		[leftWavePath lineToPoint:NSMakePoint(x , mLeftBuffer[i] * height + leftCenter)];
		[rightWavePath lineToPoint:NSMakePoint(x , mRightBuffer[i] * height + rightCenter)];
	}
	
	[[NSColor whiteColor] setStroke];						// ... (8)
	[leftWavePath setLineWidth:1.0];
	[leftWavePath stroke];									// ... (9)
	[[NSColor blueColor] setStroke];
	[rightWavePath setLineWidth:1.0];
	[rightWavePath stroke];
}

- (void)drawWaveformsOfLeft:(Float32 *)left right:(Float32 *)right frames:(int)frames
{
	numFrames = frames;
	mLeftBuffer = left;
	mRightBuffer = right;
	[self setNeedsDisplay:YES]; // ... (1)
}

@end
