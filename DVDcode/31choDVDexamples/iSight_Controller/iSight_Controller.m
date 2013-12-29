#import "iSight_Controller.h"

@implementation iSight_Controller

- (id)init
{
	if([super init]){
		[NSApp setDelegate:self]; // set the delegate outlet of NSApp to self
		videoFlag = NO;
		trackingFlag = NO;
	}
	else{
		[super dealloc];
		self = nil;
	}
	return self;
}

- (void)awakeFromNib
{
	NSError *error = nil;
	
	audioFilePlayer = [[AudioFilePlayer alloc] init]; // init AudioFilePlayer
	[audioFilePlayer showWindow:self]; // show AudioFilePlayer window
	
	midiController =[[MIDI_Controller alloc] init]; // init MIDI Controller 
	[midiController showWindow:self]; // show MIDI Controller window
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(sendXY:) 
												 name:@"TrackingNotification" 
											   object:trackingView];
	
	[tracking setEnabled:NO];
	[set setEnabled:NO];
	
	if (!mCaptureSession) {
        // Set up a capture session that outputs raw frames
        BOOL success;
        mCaptureSession = [[QTCaptureSession alloc] init];	//... (1)
		
        // Find a video device
        QTCaptureDevice *device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo]; //... (2)
        success = [device open:&error];						//... (3)
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
		
        // Add a device input for that device to the capture session
        mCaptureDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:device]; //... (4)
        success = [mCaptureSession addInput:mCaptureDeviceInput error:&error];		//... (5)
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
		
        // Add a decompressed video output that returns raw frames to the session
        mCaptureDecompressedVideoOutput = [[QTCaptureDecompressedVideoOutput alloc] init];	//... (6)
        success = [mCaptureSession addOutput:mCaptureDecompressedVideoOutput error:&error];	//... (7)
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
		[mCaptureDecompressedVideoOutput setDelegate:self];									//... (8)
    }
}

- (void)sendXY:(NSNotification *)notification{
	
	NSPoint centerXY = [trackingView getCenterXY];
	
	int x,y;
	
	x = centerXY.x/639 * 513 - 257; // ....... (1)
	y = centerXY.y/479 * 513 - 257; // ....... (2)

	Float32 cutOff = (1-(Float32)x+255) * 5;
	Float32 resonance = pow(abs((Float32)y),0.5);
	Float32 playbackRate = (((Float32)y+255) / 510) * 2;
	
	[audioFilePlayer setEffect1Parameter:cutOff Parameter2:resonance];
	[audioFilePlayer setEffect2Parameter:playbackRate];	
	
	[midiController sendMIDIWithX:x Y:y Z: 0 ];
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput 
  didOutputVideoFrame:(CVImageBufferRef)videoFrame 
	 withSampleBuffer:(QTSampleBuffer *)sampleBuffer 
	   fromConnection:(QTCaptureConnection *)connection
{
		
	[trackingView setAnImage:[CIImage imageWithCVImageBuffer:videoFrame]];
		
}

- (IBAction)reset:(id)sender {
	[trackingView resetLUT];
	trackingFlag = NO;
	[tracking setEnabled:NO];
	[trackingView setTrackingFlag:NO];
}

- (IBAction)set:(id)sender {
	[trackingView setLUTFlag:YES];
	[tracking setEnabled:YES];
}

- (IBAction)tracking:(id)sender {
	trackingFlag = !trackingFlag;	
	[trackingView setTrackingFlag:trackingFlag];
}

- (IBAction)mirror:(id)sender {
	[trackingView setMirrorFlag:[sender state]];
}
- (IBAction)video:(id)sender {
	videoFlag = !videoFlag;
	
	if (videoFlag){
		[sender setTitle:@"Video Off"];
		[mCaptureSession startRunning];
	}else{
		[sender setTitle:@"Video On"];
		[mCaptureSession stopRunning];
	}
	
	[set setEnabled:videoFlag];
}

- (void)applicationWillTerminate:(NSNotification*)notification
{  
	NSLog(@"Destroying @%d",self);
	
	if([mCaptureSession isRunning]) 
		[mCaptureSession stopRunning];
	
    QTCaptureDevice *device = [mCaptureDeviceInput device];
    if ([device isOpen]) 
		[device close];
	
	[audioFilePlayer release]; // release audioFilePlayer when this application is terminated
	[midiController release]; // release midiController when this application is terminated

	[mCaptureSession release];
    [mCaptureDeviceInput release];
    [mCaptureDecompressedVideoOutput release];
    [super dealloc];
}
@end
