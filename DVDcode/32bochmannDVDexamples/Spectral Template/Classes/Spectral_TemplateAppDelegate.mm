//
//  Spectral_TemplateAppDelegate.m
//  Spectral Template
//
//  Created by Johannes Bochmann on 07.04.09.
//  Copyright JoboMusic GmbH 2009. All rights reserved.
//

#import "Spectral_TemplateAppDelegate.h"
#import "EAGLView.h"

@implementation Spectral_TemplateAppDelegate

@synthesize window;
@synthesize glView;

#pragma mark -Audio Session Interruption Listener

void rioInterruptionListener(void *inClientData, UInt32 inInterruption)
{
	//Implement your interruption handling here
}

#pragma mark -Audio Session Property Listener

void propListener(	void *                  inClientData,
				  AudioSessionPropertyID	inID,
				  UInt32                  inDataSize,
				  const void *            inData)
{
	//Implement your property listener here
}



- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	// hide the status bar
	[application setStatusBarHidden:YES animated:YES];
	
	[playbackButton setEnabled:NO];
	
	// Initialize and configure the audio session
    // the rioInterruptionListener is the callback that gets invoked 
    // when an interruption takes place such as an incoming phone call
	AudioSessionInitialize(NULL, NULL, rioInterruptionListener, self);
	AudioSessionSetActive(true);
	
	UInt32 audioCategory = kAudioSessionCategory_LiveAudio;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);
	AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, propListener, self);
	
	UInt32 size = sizeof(hwSampleRate);
	hwSampleRate = 44100.0;
	AudioSessionSetProperty(kAudioSessionProperty_CurrentHardwareSampleRate, size, &hwSampleRate);
	AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareSampleRate, &size, &hwSampleRate);
	printf("Sample Rate %f \n", hwSampleRate);
	
	//Float32 preferredBufferSize = .02321995; //1024 samples
	Float32 preferredBufferSize = 0.01160998;  //512 Samples
	AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(preferredBufferSize), &preferredBufferSize);
	size = sizeof(UInt32);
	AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, &size, &hwBufferDuration);
	printf("Buffer Duration %f \n", hwBufferDuration);
	
	//[glView setHidden:TRUE];
	
	pgbarInterval = 5.0 / 60.0;
	pgbarTimer = [NSTimer scheduledTimerWithTimeInterval:pgbarInterval target:self selector:@selector(updatePGbar) userInfo:nil repeats:YES];
	
	//we need to do the analyzing in a new thread so we don't get timeout messages
	[NSThread detachNewThreadSelector:@selector(loadAudioFile) toTarget:self withObject:nil];
	
}

-(void) updatePGbar
{
	float progress = (float)[glView getLoadPercent]/100.0f;
	[analyzeProgress setProgress:progress];
	if(progress == 1.0f)
	{
		//now loading is complete and we can activate the playback button
		[playbackButton setEnabled:YES];
		[pgbarTimer invalidate];
	}

}

-(void)loadAudioFile
{
	/*You need this for all threads you create or you will leak! */
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	[glView setupAudio:[[NSBundle mainBundle] pathForResource:@"Vibe1" ofType:@"wav"]];	
			
	//remove our pool and free the memory collected by it
    [pool release];
}

-(IBAction) startAudioPlayback: (id) sender
{
	
	[glView setupViewForSpectrum];
	
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
	
	[window bringSubviewToFront:glView];
	
	[glView startAudioPlayback];
	
	[glView setHidden:FALSE];
}



- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	AudioSessionSetActive(false);
	[window release];
	[glView release];
	[pgbarTimer release];
	[super dealloc];
}

@end
