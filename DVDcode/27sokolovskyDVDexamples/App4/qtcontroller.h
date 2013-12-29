/* qtcontroller */


// header for the class

#import <Cocoa/Cocoa.h>

@interface qtcontroller : NSObject
{
	// outlets
    IBOutlet id errorWindow;
    IBOutlet id instrumentTextbox;
    IBOutlet id keyRadio;
    IBOutlet id tempoSlider;
    IBOutlet id velocitySlider;
	
	// variables
	int tempo, velocity, scale, instrument, key;
	bool error;
	float interval;
	int majorScale[9]; // array of the intervals for the major scale
	int n;
	
}

// one action
- (IBAction)play:(id)sender;


@end




/// programmed by chen sokolovsky © Boston MA, Feb 2008