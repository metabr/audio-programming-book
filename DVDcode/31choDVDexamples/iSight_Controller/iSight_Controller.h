#import <Cocoa/Cocoa.h>
#import <QTKit/QTkit.h>
#import "TrackingView.h"
#import "AudioFilePlayer.h"
#import "MIDI Controller.h"

@interface iSight_Controller : NSObject {
    IBOutlet NSButton *reset;
    IBOutlet NSButton *set;
    IBOutlet NSButton *tracking;
    IBOutlet TrackingView *trackingView;
    IBOutlet NSButton *video;
	
	QTCaptureSession                       *mCaptureSession;
	QTCaptureDeviceInput                   *mCaptureDeviceInput;
	QTCaptureDecompressedVideoOutput       *mCaptureDecompressedVideoOutput;
	
	CVImageBufferRef	mCurrentImageBuffer;
	
	BOOL	videoFlag, trackingFlag;
	
	AudioFilePlayer	*audioFilePlayer;
	MIDI_Controller *midiController;
}
- (IBAction)reset:(id)sender;
- (IBAction)set:(id)sender;
- (IBAction)tracking:(id)sender;
- (IBAction)video:(id)sender;
- (IBAction)mirror:(id)sender;
- (void)sendXY:(NSNotification *)notification;
@end
