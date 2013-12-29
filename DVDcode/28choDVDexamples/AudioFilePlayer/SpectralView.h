//
//  SpectralView.h
//  AudioPlayer2
//
//  Created by Taemin Cho on 8/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Accelerate/Accelerate.h>

@interface SpectralView : NSView {
	
	FFTSetup		setupReal;
	DSPSplitComplex	fftResult;
	
	Float32			*window;
	Float32			*spectrum;
	UInt32			log2n;
	UInt32			n, nOver2;
}

- (void)drawSpectrumFromLeft:(Float32 *)left right:(Float32 *)right frames:(int)frames;
- (void)fftSetup;

@end
