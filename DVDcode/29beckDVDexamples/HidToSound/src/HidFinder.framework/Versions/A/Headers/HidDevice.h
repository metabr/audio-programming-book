//
//  HidDevice.h
//  HID Finder
//
//  Created by Andrew Beck on 12/27/07.
//

#import <Cocoa/Cocoa.h>
#include <IOKit/IOCFPlugIn.h>
#include "HidElement.h"

@interface HidDevice : NSObject {
	NSMutableDictionary* properties;
	io_object_t ioHidDevice;
	IOHIDDeviceInterface** interface;
	
	NSString* name;
	NSString* rawDump;
	
	BOOL isOpen;

	NSMutableArray* elementList;
	HidElement* currentElement;	
}

// Initalization
- (HidDevice*)initWithDevice:(io_object_t) setDevice properties:(NSDictionary*) props;

// Accessors
- (void) setProperties:(NSDictionary*) props;
- (NSMutableDictionary*) properties;
- (void) setIoHidDevice:(io_object_t) setIoHidDevice;
- (io_object_t) ioHidDevice;
- (void) setName:(NSString*) setName;
- (NSString*) name;
- (void) setRawDump:(NSString*) setRawDump;
- (NSString*) rawDump;

// Interface related methods
- (void) createInterface;
- (IOHIDDeviceInterface**) interface;
- (IOReturn) open;
- (void) close;

- (void) buildElementList:(NSMutableDictionary*) ref;
- (void) addElement:(NSMutableDictionary*) refElement;

- (void) pollCurrentElement;
- (void) pollAllElements;
- (void) setDelegate:(id) delegate;

- (void) setCurrentElement:(HidElement*) setCurrentElement;
- (HidElement*) currentElement;

@end
