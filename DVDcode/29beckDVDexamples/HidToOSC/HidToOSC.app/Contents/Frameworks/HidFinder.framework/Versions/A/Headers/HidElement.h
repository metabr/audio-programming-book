//
//  HidDevice.m
//  HID Finder
//
//  Created by Andrew Beck on 12/27/07.
//

#import <Cocoa/Cocoa.h>
#import <IOKit/hid/IOHIDLib.h>

@interface HidElement : NSObject {
		long min;
		long max;
		long size;
		long usage;
		long usagePage;
		long type;
		id cookie;

		NSString* name;

		BOOL isScaled;
		BOOL isEnabled;
		NSString* string;
		
		long value;

		id delegate;
		id parent;
}

- (id) initWithParent:(id) setParent;

// initalize values from an NSDictionary* describing the element
- (void) setInfo:(NSDictionary*) refElement;

// Accessor methods
- (void) setValue:(long) setValue;
- (float) value;
- (void) setMin:(long) setMin;
- (long) min;
- (void) setMax:(long) setMax;
- (long) max;
- (void) setSize:(long) setSize;
- (long) size;
- (void) setUsage:(long) setUsage;
- (long) usage;
- (void) setUsagePage:(long) setUsagePage;
- (long) usagePage;
- (void) setType:(long) setType;
- (long) type;
- (void) setCookie:(id) setCookie;
- (id) cookie;
- (void) setIsScaled:(BOOL) setIsScaled;
- (BOOL) isScaled;
- (void) setIsEnabled:(BOOL) setIsEnabled;
- (BOOL) isEnabled;
- (void) setName:(NSString*) setName;
- (NSString*) name;
- (void) setString:(NSString*) setName;
- (NSString*) string;

- (void) refresh;

- (void) updateValue:(IOHIDDeviceInterface**) interface;
- (void) setDelegate:(id) setDelegate;
- (float) scaledValue;


- (id) parent;
@end
