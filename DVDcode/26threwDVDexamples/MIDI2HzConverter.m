#import "MIDI2HzConverter.h"

@implementation MIDI2HzConverter
- (float)convertMidi:(int)note
{
	
	double freq;
	double semitone_ratio;
	double c0; /* for frequency of MIDI Note 0 */
	double c5; /* for frequency of Middle C */

	/* calculate required numbers */

	semitone_ratio = pow(2, 1/12.0);

	c5 = 220.0 * pow(semitone_ratio, 3);
	c0 = c5 * pow(0.5, 5);

	freq = c0 * pow(semitone_ratio, note);
	
	return freq;
	
}
@end
