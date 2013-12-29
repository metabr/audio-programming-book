//
//  HidFinderData.h
//  HID Finder
//
//  Created by Andrew Beck on 12/29/07.
//

#import <Cocoa/Cocoa.h>
#import "HidElement.h"

@interface HidFinderData : NSObject {

	NSMutableDictionary* data;
	
	NSMutableDictionary* copyBuf;

}

HidFinderData* gData;

+ (HidFinderData*) getData;

- (void) setValue:(id) value ofKey:(NSString*) key from:(id) sender;
- (id) getValueOfKey:(NSString*) key from:(id) sender;
- (NSMutableDictionary*) getRoot:(id) sender;

- (void) save:(NSString*) filename;
- (void) load:(NSString*) filename;
- (void) openPanelDidEnd:(NSOpenPanel*)panel returnCode:(int)rc contextInfo:(void *) ctx;

- (void) copyElement:(HidElement*) element;
- (void) pasteElement:(HidElement*) element;

@end
