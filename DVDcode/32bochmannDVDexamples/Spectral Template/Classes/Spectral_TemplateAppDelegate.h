//
//  Spectral_TemplateAppDelegate.h
//  Spectral Template
//
//  Created by Johannes Bochmann on 07.04.09.
//  Copyright JoboMusic GmbH 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <AudioToolbox/AudioToolbox.h>


@class EAGLView;

@interface Spectral_TemplateAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
	
	IBOutlet UIButton *playbackButton;
	IBOutlet UIProgressView *analyzeProgress;
	
	NSTimer *pgbarTimer;
    NSTimeInterval pgbarInterval;

	Float64	hwSampleRate;
	Float32	hwBufferDuration;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

-(void)loadAudioFile;
-(IBAction) startAudioPlayback: (id) sender;

@end

