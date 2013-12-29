/// programmed by chen sokolovsky © Boston MA, Feb 2008


#import "qtcontroller.h"
#import <QuickTime/QuickTimeMusic.h>

@implementation qtcontroller

- (IBAction)play:(id)sender
{
    NoteAllocator   na = OpenDefaultComponent('nota', 0);     // initializing note allocator;
    NoteChannel     nc=1;
    NoteRequest     nr;
 

	majorScale[1]=0; majorScale[2]=2; majorScale[3]=4; majorScale[4]=5;     // intervals of a major scale
	majorScale[5]=7; majorScale[6]=9; majorScale[7]=11; majorScale[8]=12;
	
	// reading the values from the UI
	tempo = [tempoSlider intValue];  
	key = 60 + majorScale[[keyRadio selectedColumn]+1]; //nsmatrix - for the radio buttons
	velocity = [velocitySlider intValue];
	instrument = [instrumentTextbox intValue];
	interval = 1800/tempo;
	
	
	// checking for out of range instrument number
	error = NO;
	if (instrument<=0) {[errorWindow makeKeyAndOrderFront:self]; error=YES; } //nswindow - showing the error window
	if (instrument>127) {[errorWindow makeKeyAndOrderFront:self]; error=YES; } //nswindow
	
	// opening a channel
	NAStuffToneDescription(na,instrument, &nr.tone);
	NANewNoteChannel(na, &nr, &nc);	
	
	if (!error)
	{
		// plays the scale (8 notes)
		for (n=1 ; n<9; n++)
			{
				// note on
				NAPlayNote(na, nc, key+majorScale[n], velocity);    
				Delay(interval/2,0); 
				
				//note off (note on with velocity 0 )
				NAPlayNote(na, nc, key+majorScale[n], 0);   
				Delay(interval, 0);
			}
	}						
}

@end



/// programmed by chen sokolovsky © Boston MA, Feb 2008
