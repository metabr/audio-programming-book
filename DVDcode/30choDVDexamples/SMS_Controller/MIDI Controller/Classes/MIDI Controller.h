#import <Cocoa/Cocoa.h>
#import <CoreMIDI/CoreMIDI.h>

// Define of MIDI Status Byte

#define NOTE_ON			0x90
#define	NOTE_OFF		0x80
#define CONTROL_MSG		0xB0
#define PITCHBEND_MSG	0xE0

@interface MIDI_Controller : NSWindowController 
{	
    IBOutlet NSTableView *mTableView;
	
	NSMutableArray		*records, *outPorts;
	NSMutableDictionary *ref;
	MIDIPortRef			outPort;
}
- (IBAction)addRemove:(id)sender;

- (void)sendMIDIData:(Byte *)data To:(int)indexOfOutPort;
- (void)sendMIDIWithX:(int)x Y:(int)y Z:(int)z;

@end
