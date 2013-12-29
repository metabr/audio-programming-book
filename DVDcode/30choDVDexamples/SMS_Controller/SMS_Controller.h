//
//  SMS_Controller.h
//  SMS_Controller
//
//  Created by Taemin Cho on 1/9/08.
//  Copyright 2008 __Taemin Cho__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SuddenMotionSensor.h"
#import "AudioFilePlayer.h"
#import "MIDI Controller.h"

@interface SMS_Controller : NSObject
{
    IBOutlet NSTextField *x;
    IBOutlet NSTextField *y;
    IBOutlet NSTextField *z;
	IBOutlet NSButton *offButton;
	IBOutlet NSButton *onButton;
	
	NSTimer			*timer, *flagTimer;
	int				flag;
	
	AudioFilePlayer	*audioFilePlayer;
	MIDI_Controller *midiController;
}
- (IBAction)off:(id)sender;
- (IBAction)on:(id)sender;

- (id)initWithSmsOpen;  // find and initialize a Sudden Motion Sensor
- (void)smsError; 		// show No Sudden Motion Sensor Error alert

- (void)updateData:(NSTimer *)theTimer; // update the Sudden Motion Sensor data

@end
