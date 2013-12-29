/* OSCSender */

#import <Cocoa/Cocoa.h>
#import <ObjCOSC/OSCPort.h>
#import <HidFinder/HidElement.h>

@interface OSCSender : NSObject
{
	NSString* ip;
	NSString* strPort;
	int portNum;
	OSCPort* port;
}

- (void) connect;
- (void) setIp:(NSString*) setIp;
- (NSString*) ip;
- (void) setStrPort:(NSString*) setStrPort;
- (NSString*) strPort;

- (void) elementUpdated:(HidElement*) element;

@end
