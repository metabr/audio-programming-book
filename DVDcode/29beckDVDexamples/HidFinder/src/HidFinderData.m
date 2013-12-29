//
//  HidFinderData.h
//  HID Finder
//
//  Created by Andrew Beck on 12/29/07.
//

#import "HidFinderData.h"
#import "HidFinder.h"

@implementation HidFinderData

+ (HidFinderData*) getData
{
	if (gData)
		return gData;
	else
		return gData = [[HidFinderData alloc] init];
}

- (id) init
{
	self = [super init];
	
	data = [[NSMutableDictionary alloc] init];
	
	return self;
}

- (void) setValue:(id) value ofKey:(NSString*) key from:(id) sender
{
	[[self getRoot:sender] setObject:value forKey:key];
}

- (id) getValueOfKey:(NSString*) key from:(id) sender
{
	return [[self getRoot:sender] valueForKey:key];
}	

- (NSMutableDictionary*) getRoot:(id) sender
{
	NSMutableDictionary* ret;

	if ([sender isKindOfClass:[HidElement class]])
	{
		ret = [[self getRoot:[sender parent]] valueForKey:[NSString stringWithFormat:@"%@%d", [sender name], [sender cookie]]];
		if (ret)
			return ret;
		ret = [[NSMutableDictionary alloc] init];
		[[self getRoot:[sender parent]] setObject:ret forKey:[NSString stringWithFormat:@"%@%d", [sender name], [sender cookie]]];
		return ret;
	} else if ([sender isKindOfClass:[HidDevice class]])
	{
		ret = [data valueForKey:[sender name]];
		if (ret)
			return ret;
		ret = [[NSMutableDictionary alloc] init];
		[data setObject:ret forKey:[sender name]];
		return ret;
	}
	
	return data;
}

- (void) save:(NSString*) filename
{
	[data writeToFile:filename atomically:YES];
}

- (void) load:(NSString*) filename
{
	data = [[NSMutableDictionary alloc] initWithContentsOfFile:filename];
}

- (void) copyElement:(HidElement*) element
{
	copyBuf = [[NSMutableDictionary alloc] initWithDictionary:[self getRoot:element]];
}

- (void) pasteElement:(HidElement*) element
{
	[[self getRoot:[element parent]] setObject:[[NSMutableDictionary alloc] initWithDictionary:copyBuf]
			forKey:[NSString stringWithFormat:@"%@%d", [element name], [element cookie]]];
}

@end
