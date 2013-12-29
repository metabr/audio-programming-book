//
//  AudioOutput.h
//  TestHidOsc
//
//  Created by Andrew Beck on 1/18/08.
//

#import <Cocoa/Cocoa.h>
#import <AudioUnit/AudioUnit.h>
#import <CoreAudio/CoreAudio.h>
#import <HidFinder/HidFinder.h>
#import <SineEvent.h>

@interface AudioOutput : NSObject {
	AudioUnit outputUnit;
	
	double sr;
	UInt32 theFormatFlags;
	UInt32 theBytesInAPacket;
	UInt32 theBitsPerChannel;
	UInt32 theBytesPerFrame;
	
	float sinBuf[1025];
	float sqBuf[1025];
	float sawBuf[1025];
}

NSMutableArray* sines;
AudioOutput*	audioOutput;

+ (AudioOutput*) getAudioOutput;
+ (NSMutableArray*) getSines;
- (double) getSr;
- (void) createAU;
- (void) runAU;
- (void) elementUpdated:(HidElement*) element;

@end

OSStatus	MyRenderer(void 				*inRefCon, 
				AudioUnitRenderActionFlags 	*ioActionFlags, 
				const AudioTimeStamp 		*inTimeStamp, 
				UInt32 						inBusNumber, 
				UInt32 						inNumberFrames, 
				AudioBufferList 			*ioData);