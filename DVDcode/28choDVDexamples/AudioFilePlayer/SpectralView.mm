//
//  SpectralView.m
//  AudioPlayer2
//
//  Created by Taemin Cho on 8/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "SpectralView.h"


@implementation SpectralView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self fftSetup];
    }
    return self;
}

- (void)drawRect:(NSRect)rect {
	
	// Draw a rectangle background
	[[NSColor blackColor] setFill];
	NSRectFill(rect);
	
	// Draw spectrum
	NSBezierPath* spectralPath = [NSBezierPath bezierPath];
	[spectralPath setLineWidth:1.0]; // Has no effect.
	
	[spectralPath moveToPoint:NSMakePoint(0, 0)];
	
	for(int i = 0; i < nOver2 / 3.0; i++)
	{	
		CGFloat x = i * rect.size.width / (nOver2 / 3.0) ;
		[spectralPath lineToPoint:NSMakePoint(x , spectrum[i] *  5.0)];
	}
	
	[spectralPath lineToPoint:NSMakePoint(rect.size.width, 0)];
	
	NSGradient* aGradient = [[[NSGradient alloc]
							  initWithColorsAndLocations:[NSColor redColor], (CGFloat)0.0,
							  [NSColor orangeColor], (CGFloat)0.166,
							  [NSColor yellowColor], (CGFloat)0.33,
							  [NSColor greenColor], (CGFloat)0.5,
							  [NSColor blueColor], (CGFloat)0.75,
							  [NSColor purpleColor], (CGFloat)1.0,
							  nil] autorelease];
	
    [aGradient drawInBezierPath:spectralPath angle:0.0];
}

- (void)drawSpectrumFromLeft:(Float32 *)left right:(Float32 *)right frames:(int)frames
{	
	float *signal = (float*) malloc(frames * sizeof(float));	// ... (1)
	vDSP_vadd(left, 1, right, 1, signal, 1, frames);			// ... (2)
	
	memset(window, 0, n * sizeof(float));						// ... (3)
	vDSP_hamm_window(window, frames, 0);						// ... (4)
	
	vDSP_vmul(signal, 1, window, 1, window, 1, frames);			// ... (5)
	vDSP_ctoz ( ( COMPLEX * ) window, 2, &fftResult, 1, nOver2 );	// ... (6)
	vDSP_fft_zrip ( setupReal, &fftResult, 1, log2n, FFT_FORWARD ); // ... (7)
	vDSP_zvabs(&fftResult, 1, spectrum, 1, nOver2);					// ... (8)
	free(signal);			// ... (9)
	
	[self setNeedsDisplay:YES]; // ... (10)
}

- (void)fftSetup
{
	// Set the size of FFT.
	log2n = 10;
	n = 1 << log2n;                // n is 2^10.
	nOver2 = n / 2;                // half of n as real part and imag part.
	
	setupReal = vDSP_create_fftsetup (log2n, FFT_RADIX2); 
	
	fftResult.realp = ( float* ) malloc ( nOver2 * sizeof ( float ) );
	fftResult.imagp = ( float* ) malloc ( nOver2 * sizeof ( float ) );
	
	window = (float*) malloc(n * sizeof(float));
	spectrum = (float*) malloc(nOver2 * sizeof (float));
}

- (void)dealloc
{
	if(setupReal)
		vDSP_destroy_fftsetup(setupReal);
		
	if(fftResult.realp)
		free(fftResult.realp);
	
	if(fftResult.imagp)
		free(fftResult.imagp);
	
	if(window)
		free(window);
	
	if(spectrum)
		free(spectrum);
	
	[super dealloc];
}

@end
