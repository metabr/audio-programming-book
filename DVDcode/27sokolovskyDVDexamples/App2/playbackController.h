/* playbackController */

#import <Cocoa/Cocoa.h>

// file header, interface:

@interface playbackController : NSObject
{

	// these are the outlets:
    IBOutlet id animation; // controlls the small animation dial. through this outlet we can tell it to start or stop
    IBOutlet id playButton; //  we use this outlet to change the text of the button prom "play" to "pause" to "resume"  according to the current function 
    IBOutlet id textField; //we use this to print the name and path of the opened file
	
	// these are the global variables
	NSSound *soundFile;  // the sample
    NSOpenPanel *oPanel;  // this is for choosing a file to open with our application
	NSString *myFileName; // where we save the file and path
	bool isPaused; // indication for pausing
	NSTimer *mainTimer;  // a timer that check if the sample is done playing
	
}

// these are the actions - one for each button

- (IBAction)open:(id)sender;
- (IBAction)play:(id)sender;
- (IBAction)stop:(id)sender;


// this is a function that is called every cycle of the timer and checks if the sample is done or still playing
- (void)tick;

@end
