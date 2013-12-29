#import "playbackController.h"

@implementation playbackController


// when pressing the "open" button
- (IBAction)open:(id)sender
{
	 // initting a panel
	 oPanel = [NSOpenPanel openPanel];
	 // making sure we can choose only one file at a time
	 [oPanel setAllowsMultipleSelection:NO];
	 // sets the default directory and file type
     int result = [oPanel runModalForDirectory:NSHomeDirectory()
                    file:nil types:[NSArray arrayWithObject:@"wav"]];
     
	 // when user hits 'OK' after choosing the file
	 if (result == NSOKButton) {
		
		// setting up the file name and path
		myFileName=[oPanel filename];
		myFileName = [myFileName stringByResolvingSymlinksInPath]; // this line is not necessary, however recommended

		// when the initiation is an init function you must allocate first
		soundFile = [[NSSound alloc] initWithContentsOfFile:myFileName byReference:YES];  
		 
		// updates the display outlet to show the file and path
		[textField setStringValue:myFileName];  
		
		
	}

}


// when play button is pressed
- (IBAction)play:(id)sender
{
	// when it is not playing:
	if (![soundFile isPlaying]) {
		
		// start playing
		[soundFile play];
		
		//start animation
		[animation startAnimation:self];
		
		//change button title to pause
		[playButton setTitle:@"Pause"];
		
		//setup timer
		mainTimer = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(tick) userInfo:nil repeats:YES]; // interval is 0.5 a second, tick is the name of the function
		[[NSRunLoop currentRunLoop] addTimer:mainTimer forMode:NSModalPanelRunLoopMode]; // adds the timer to the main loop
	}
	
	// if it was playing
	else {
		// if it was'nt pause then pause
		if (!isPaused) {
			[soundFile pause];
			[animation stopAnimation:self];
			isPaused=YES;
			[playButton setTitle:@"Resume"];
		}
		
		// if it was paused then resume
		else {
			[soundFile resume];
			isPaused=NO;
			[animation startAnimation:self];
			[playButton setTitle:@"Pause"];
		}
	}	
}

// when the stop button is pressed

- (IBAction)stop:(id)sender
{
	isPaused=NO;  // sets pause indication
	[playButton setTitle:@"Play"]; // sets button title
	[animation stopAnimation:self]; // stop animation
	[soundFile stop]; // stop playing the file

}


// function that is called with each tick
-(void)tick 
{
	// if the file is done playing
	if (![soundFile isPlaying]) {
	
		// call the stop function 
		[self stop:self];
		
		// stop the timer
		[mainTimer invalidate];
	}
}

	
@end
