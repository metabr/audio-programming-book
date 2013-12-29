#import "PlaySampleController.h"

// this is the implementtion of our class

@implementation PlaySampleController


// this defines what happens when you press the button
- (IBAction)play:(id)sender
{
	// a new instance of NSSound is created and initialized with the file "sample.wav"
	// the file must be imported to the project using 'Project>add to Project' menu
	NSSound* mySample = [NSSound soundNamed:@"sample"];

	// the method to start playing 
	[mySample play];

}

@end
