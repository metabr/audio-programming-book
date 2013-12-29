//
//  SineEvent.h
//  TestHidOsc
//
//  Created by Andrew Beck on 1/18/08.
//

#import <Cocoa/Cocoa.h>
#include <CoreAudio/CoreAudio.h>


@interface SineEvent : NSObject {
	long			sNumChannels;
	long			sWhichFormat;

	double j;

	double			amp;
	double			freq;
	double			finalFreqExp;
	int				delay;
	double			atk;
	int				ampCount;
	double			curAmp;
	BOOL			resetAmp;
	double			fullAtk;
	double			duration;
	double			fullDur;
	float			age;
	unsigned long	bufSize;
	unsigned long	theFormatID;

	float buf[1025];
// these are the same regardless of format
	unsigned long theFramesPerPacket; // this shouldn't change
}

- (id) initWithBuf:(float[]) setBuf;
- (void) setFreq:(double) setFreq;
- (double) freq;
- (void) setAmp:(double) setAmp;
- (double) amp;
- (void) setAtk:(double) setAtk;
- (void) setDuration:(double) setDur;
- (double) duration;
- (void) decDurBy:(float)sub;
- (BOOL) isDead;
- (double) age;
- (void) setDelay:(int) setDelay;
- (void) render:(UInt32) inFrames toBuf:(void*)inBuffer atSR:(double)inSampleRate inFormat:(int)inOutputFormat;
- (void) setFinalFreq:(double) finalFreq;
- (void) setFinalFreqRatio:(double) finalFreqRatio;

@end
