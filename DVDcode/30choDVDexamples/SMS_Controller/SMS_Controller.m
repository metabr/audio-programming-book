//
//  SMS_Controller.m
//  SMS_Controller
//
//  Created by Taemin Cho on 1/9/08.
//  Copyright 2008 __Taemin Cho__. All rights reserved.
//

#import "SMS_Controller.h"
#import "SMS_DebugMacros.h" // import SMS_DebugMacros.h

@implementation SMS_Controller

- (id)init
{
	return [self initWithSmsOpen];
}

- (id)initWithSmsOpen
{
	if([super init]){
		[NSApp setDelegate:self]; // set the delegate outlet of NSApp to self
		
		if(sms_Open()){
			ERR(sms_Open()); // Debug macro
			[self smsError];
			[NSApp terminate:NSApp];
		}
	}
	return self;
}

- (void)awakeFromNib
{
	flag = 0; // play/stop flag
	
	audioFilePlayer = [[AudioFilePlayer alloc] init]; // init AudioFilePlayer
	[audioFilePlayer showWindow:self]; // show AudioFilePlayer window
	
	midiController =[[MIDI_Controller alloc] init]; // init MIDI Controller 
	[midiController showWindow:self]; // show MIDI Controller window
	
	[offButton setEnabled:NO];
	[onButton setEnabled:YES];
}

- (void)smsError
{
	NSBeep();
	NSRunAlertPanel(@"Close", @"Couldn't find any Sudden Motion Sensor", 
					@"Close", nil, nil);	
}

- (IBAction)off:(id)sender
{
	[timer invalidate];

	[offButton setEnabled:NO];
	[onButton setEnabled:YES];
}

- (IBAction)on:(id)sender
{
	timer = [NSTimer scheduledTimerWithTimeInterval:0.05
											 target:self
										   selector:@selector(updateData:)
										   userInfo:nil
											repeats:YES];

	[offButton setEnabled:YES];
	[onButton setEnabled:NO];
}

- (void)updateData:(NSTimer *)theTimer
{
	sms_data_t smsData;
	
	ERR(sms_GetData(&smsData)); // Get data from Sudden Motion Sensor
	
	[x setIntValue:smsData.x];
	[y setIntValue:smsData.y];
	[z setIntValue:smsData.z];
	
	if(smsData.z < 0 && flag < 2)
	{
		switch(flag)
		{
			case 0:
				[audioFilePlayer play:self];
				flag = 3;
				break;
			case 1:
				[audioFilePlayer stop:self];
				flag = 2;
				break;
			default:
				break;
		}
		
		flagTimer = [NSTimer scheduledTimerWithTimeInterval:2
													 target:self
												   selector:@selector(flagReset:)
												   userInfo:nil
													repeats:NO];
	}
	
	Float32 cutOff = (1-(Float32)smsData.x+255) * 5;
	Float32 resonance = pow(abs((Float32)smsData.z),0.5);
	Float32 playbackRate = (((Float32)smsData.y+255) / 510) * 2;
	
	[audioFilePlayer setEffect1Parameter:cutOff Parameter2:resonance];
	[audioFilePlayer setEffect2Parameter:playbackRate];
	
	[midiController sendMIDIWithX:smsData.x Y:smsData.y Z:smsData.z ];
}

- (void)flagReset:(NSTimer *)theTimer
{	
	flag-=2;
}

- (BOOL)windowShouldClose:(id)sender
{

	NSBeep();
	NSInteger i = NSRunAlertPanel(@"Quit", @"Do you want quit Sudden Motion Sensor Controller ?", 
								@"Quit", @"Cancel", nil);

	if(i) [NSApp terminate:NSApp];
	
	return NO;			
}

- (void)applicationWillTerminate:(NSNotification*)notification
{  
	NSLog(@"Destroying @%d",self);
	ERR(sms_Close()); // Close the current Sudden Motion Sensor connection
	
	if([offButton isEnabled]){	// if timer is running
		[timer invalidate];		
	}

	[audioFilePlayer release]; // release audioFilePlayer when this application is terminated
	[midiController release]; // release midiController when this application is terminated
	
	[super dealloc];
}

@end
