//
//  EAGLView.m
//  Spectral Template
//
//  Created by Johannes Bochmann on 07.04.09.
//  Copyright JoboMusic GmbH 2009. All rights reserved.
//



#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"

#define USE_DEPTH_BUFFER 0
#define kAccelerometerFrequency			25 //Hz

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation EAGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
    
    if ((self = [super initWithCoder:coder])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        animationInterval = 1.0 / 60.0;

    }
    return self;
}

- (void)setupAudio:(NSString *)file {
	
	//setup the audiocallback and read in a file
	audioCallback = [[RemoteIO alloc]initWithFile:file];
	//initialize aurio callback
	[audioCallback initializeAudio];
	 
	[audioCallback stop];
	
	bufferMatrix = [audioCallback bufferMatrix];
	
	//Configure and enable the accelerometer
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	
	
}

- (void)startAudioPlayback
{
	[audioCallback start];
}


- (void)setupViewForSpectrum
{	
	// Clears the view with black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
		
	bufferMatrix = [audioCallback bufferMatrix];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MATRIX_COLUMNS, FFT_FRAMESIZE/2, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferMatrix->magTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Enable use of the texture
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	
	glViewport(0, 0, backingWidth, backingHeight);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0f, 480.0f, 0.0f, 360.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
	
	// Set a blending function to use
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Enable blending
	glEnable(GL_BLEND);
}

- (void)updateTexture{
	
	glBindTexture(GL_TEXTURE_2D, texName);
	
	bufferMatrix = [audioCallback bufferMatrix];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MATRIX_COLUMNS, FFT_FRAMESIZE/2, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferMatrix->magTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
}

- (int)getLoadPercent
{
	return [audioCallback loadPercent];
}


- (void)drawView {
	const GLfloat squareVertices[] = {
        0.0f,	0.0f,
        480.0f, 0.0f,
        0.0f,	360.0f,
        480.0f, 360.0f,
    };
	
	const GLshort texCoords[] = {
		1, 0, 
		0, 0, 
		1, 1,
		0, 1, 
	};
	
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
   	
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexPointer(2, GL_FLOAT, 0, squareVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glTexCoordPointer(2, GL_SHORT, 0, texCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	[self updateTexture];
	
    glBindTexture(GL_TEXTURE_2D, texName);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (UInt16) scaleYCoord:(float) yCoord
{
	//input range 0 to 480 output range MATRIX_COLUMNS to 0
	yCoord = 480-yCoord;
	yCoord *= 0.53333333f;
	return (UInt16) yCoord;
}

- (UInt16) scaleXCoord:(float) xCoord
{
	//input range 0 to 320 output range FFT_FRAMESIZE to 0
	xCoord = 320-xCoord;
	xCoord *= 0.8f;
	return (UInt16) xCoord;
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [touches anyObject];
	
	CGPoint coord = [touch locationInView:self];
	
	bufferMatrix = [audioCallback bufferMatrix];
	bufferMatrix->setScaledAttenuation([self scaleYCoord: coord.y],[self scaleXCoord:coord.x], 0.5f);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [touches anyObject];
	
	CGPoint coord = [touch locationInView:self];
	
	bufferMatrix = [audioCallback bufferMatrix];
	bufferMatrix->setScaledAttenuation([self scaleYCoord: coord.y],[self scaleXCoord:coord.x], 0.5f);
}


- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	if ( (fabs(acceleration.x) > 1.5) || (fabs(acceleration.y) > 1.5) || (fabs(acceleration.z) > 1.5) )
	{
		printf("shake");
		bufferMatrix = [audioCallback bufferMatrix];
		bufferMatrix->resetAttenuation();
	}
	
}


- (void)layoutSubviews {
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}


- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)dealloc {
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
	[audioCallback cleanUp];
    [audioCallback release];
	
	delete[] bufferMatrix;
    [context release];  
    [super dealloc];
}

@end
