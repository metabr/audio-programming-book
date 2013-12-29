//
//  EAGLView.h
//  Spectral Template
//
//  Created by Johannes Bochmann on 07.04.09.
//  Copyright JoboMusic GmbH 2009. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "RemoteIO.h"
#import "globals.h"
#include "FFTBufferMatrix.h"


/*
This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
The view content is basically an EAGL surface you render your OpenGL scene into.
Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
*/
@interface EAGLView : UIView <UIAccelerometerDelegate> {
    
@private
    /* The pixel dimensions of the backbuffer */
    GLint backingWidth; 
    GLint backingHeight;
    
    EAGLContext *context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;
    
    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;
    
    NSTimer *animationTimer;
    NSTimeInterval animationInterval;
	
	IBOutlet RemoteIO *audioCallback;
		
	FFTBufferMatrix *bufferMatrix;
			
	GLuint texName;

}

@property NSTimeInterval animationInterval;

- (void)setupAudio:(NSString *)file;
- (void)setupViewForSpectrum;
- (void)startAnimation;
- (void)stopAnimation;
- (void)startAudioPlayback;
- (void)drawView;
- (int)getLoadPercent;
- (void)updateTexture;
- (UInt16) scaleYCoord:(float) yCoord;
- (UInt16) scaleXCoord:(float) xCoord;


@end
