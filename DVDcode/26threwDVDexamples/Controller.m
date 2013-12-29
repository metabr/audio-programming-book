/*
 ###########################################################################
 # MIDI2Hz - A Cocoa Example
 # by barry threw - me@barrythrew.com
 # San Francisco, CA
 # (c) 2009 - Under a CC GPL http://creativecommons.org/licenses/GPL/2.0/
 ###########################################################################
 // MIDI2HZ is a Cocoa front end for converting MIDI note values to frequency
 ###########################################################################
 # Controller.m
*/

#import "Controller.h"
#import "MIDI2HzConverter.h"

@implementation Controller
- (IBAction)convert:(id)sender {

	int note;
	float freq;
	
	note = [noteField intValue];
	
	if (note > 127) {
		NSRunAlertPanel( @"Invalid Midi Value", @"Midi Values Range from 0-127.", @"OK", nil, nil);
		return;
	}
	
	if (note < 0) {
		NSRunAlertPanel( @"Invalid Midi Value", @"Midi Values Range from 0-127.", @"OK", nil, nil);
		return;
	}
	
	freq = [midi2hzconverter convertMidi:note];
	
	[freqField setFloatValue:freq];
    [freqField selectText:self];	

}
@end
