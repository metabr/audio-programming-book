/* HidFinder */

#import <Cocoa/Cocoa.h>
#include "HidFinderData.h"
#include "HidDevice.h"

@interface HidFinder : NSObject
{
	NSMutableArray* deviceList;
	HidDevice* currentDevice;
	
	IBOutlet id delegate;
	
	NSTimer* pollTimer;
}

NSDictionary* gUsageStrings;
+ (NSDictionary*) getUsageStrings;

- (IBAction)go:(id)sender;

- (void) setCurrentDevice:(HidDevice*) setCurrentDevice;
- (HidDevice*) currentDevice;
- (void) buildDeviceList;

// Polling Methods
- (void) enableDevicePolling:(BOOL)toEnable withTimeInterval:(float)timeInterval;
- (void) setDelegate:(id) delegate;
- (void) poll:(NSTimer*)theTimer;

// Delegate Function (Implement this to recieve data in your prorgam)
- (void) elementUpdated:(HidElement*) element;

- (IBAction) save:(id) sender;
- (IBAction) load:(id) sender;
- (IBAction) copyElement:(id) sender;
- (IBAction) pasteElement:(id) sender;

@end
