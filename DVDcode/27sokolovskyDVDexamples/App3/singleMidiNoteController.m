#import "singleMidiNoteController.h"

// THIS IS VERY IMPORTANT - as well as importing the QuickTime.Frameworks to your project using 'Project>Add to Project' menu
#import <QuickTime/QuickTimeMusic.h>


@implementation singleMidiNoteController


// function to be called when button is pressed
- (IBAction)play:(id)sender
{
	// these are all structs and fields of the Note allocator (NA) that needs to be declared
	NoteAllocator   na = OpenDefaultComponent('nota', 0);
    NoteChannel     nc = 1;
    NoteRequest     nr;
  
	// opening a channel
	NAStuffToneDescription(na,2 , &nr.tone); // the number '2' is the chosen instrument from the GM list
	NANewNoteChannel(na, &nr, &nc);	
	
	// playing a note
	NAPlayNote(na, nc, 72 , 100);  // note on -> note 72, velocity 100
	Delay(20,0);   // delay 
	NAPlayNote(na, nc,72, 0); // note off -> same note with 0 velocity

}

@end
