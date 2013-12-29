#import <Cocoa/Cocoa.h>

@interface Controller : NSObject {
    IBOutlet id freqField;
    IBOutlet id midi2hzconverter;
    IBOutlet id noteField;
}
- (IBAction)convert:(id)sender;
@end
