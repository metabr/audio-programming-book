#import "OSCSender.h"

@implementation OSCSender

- (id) init
{
	[super init];
	
	[self setIp:[NSString stringWithString:@"127.0.0.1"]];
	[self setStrPort:[NSString stringWithString:@"57120"]];
	
	return self;
}

- (void) connect
{
	if (ip != NULL && (portNum > 0))
		port = [[OSCPort oscPortToAddress:[ip cStringUsingEncoding:NSMacOSRomanStringEncoding] portNumber:portNum] retain];
}

- (void) setIp:(NSString*) setIp
{
	if (ip!=NULL)
	{
		[ip release];
		ip = NULL;
	}
	[port release];
	port = NULL;
	ip = [setIp retain];
	[self connect];
}

- (NSString*) ip
{
	return ip;
}

- (void) setStrPort:(NSString*) setStrPort
{
	portNum = [setStrPort intValue];
	strPort = setStrPort;
	[port release];
	port = NULL;
	[self connect];
}

- (NSString*) strPort
{
	return strPort;
}

- (void) elementUpdated:(HidElement*) element
{
	if ([element string] != NULL)
		[port sendTo:(char*) [[element string] cStringUsingEncoding:NSMacOSRomanStringEncoding] 
				types:"f", [element value]];
}

@end
