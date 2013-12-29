//
//  HidDevice.m
//  HID Finder
//
//  Created by Andrew Beck on 12/27/07.
//

#import "HidElement.h"
#import "HidFinder.h"

@implementation HidElement

- (id) initWithParent:(id) setParent
{
	self = [super init];
	
	parent = setParent;

	return self;
}

- (float) scaledValue
{
	float ret = value;
	ret = (ret - min) / (max-min);
	return ret;
}

- (void) updateValue:(IOHIDDeviceInterface**) interface
{
	if ([self isEnabled])
	{
		HRESULT				result;
		IOHIDEventStruct	hidEvent;

		result = (*interface)->getElementValue(interface,
					[self cookie], &hidEvent);
		if (result)
			NSLog(@"getElementValue error = %lx", result);
		if (value != hidEvent.value)
		{
			[self setValue:hidEvent.value];

			if (delegate)
				[delegate elementUpdated:self];
		}
	}
}

- (void) setValue:(long) setValue
{
	value = setValue;
}

- (float) value
{
	if ([self isScaled])
		return [self scaledValue];
	else
		return value;
}

- (void) setInfo:(NSDictionary*) refElement
{

	id ref = [refElement valueForKey:(id) CFSTR(kIOHIDElementNameKey)];
	if (ref)
	{
		char* tempName;
		if (CFStringGetCString (ref, tempName, 256, CFStringGetSystemEncoding ()))
			[self setName:[[NSString alloc] initWithCString:tempName encoding:NSMacOSRomanStringEncoding]];
	}

	if (![self name])
	{
		NSDictionary* usageStrings = [HidFinder getUsageStrings];
		NSString* tempName = NULL;
		tempName = [[usageStrings objectForKey:[NSString stringWithFormat:@"0x%04X", usagePage]]
												objectForKey:[NSString stringWithFormat:@"0x%04X", usage]];


		if (tempName)
			[self setName:[[NSString alloc] initWithString:tempName]];
		else
			[self setName:[[NSString alloc] initWithFormat:@"Usage Page: 0x%x Usage: 0x%x", usagePage, usage]];
	}

	ref = [refElement valueForKey:(id) CFSTR(kIOHIDElementCookieKey)];
	if (ref && [ref isKindOfClass:[NSNumber class]])
		[self setCookie:(IOHIDElementCookie) [ref longValue]];
	else
		[self setCookie:(IOHIDElementCookie) 0];

	ref = [refElement valueForKey:(id) CFSTR(kIOHIDElementMinKey)];
	if (ref && [ref isKindOfClass:[NSNumber class]])
		[self setMin:[ref longValue]];
	else
		[self setMin:0];
		
	ref = [refElement valueForKey:(id) CFSTR(kIOHIDElementMaxKey)];
	if (ref && [ref isKindOfClass:[NSNumber class]])
		[self setMax:[ref longValue]];
	else
		[self setMax:0];

	ref = [refElement valueForKey:(id) CFSTR(kIOHIDElementSizeKey)];
	if (ref && [ref isKindOfClass:[NSNumber class]])
		[self setSize:[ref longValue]];
	else
		[self setSize:0];
	
	[self setIsScaled:NO];
	[self setIsEnabled:NO];
	[self setString:@""];

}

- (void) refresh
{
	[self setIsScaled:[self isScaled]];
	[self setIsEnabled:[self isEnabled]];
	[self setString:[self string]];
}

- (void) setMin:(long) setMin
{
	min = setMin;
}

- (long) min
{
	return min;
}

- (void) setMax:(long) setMax
{
	max = setMax;
}

- (long) max
{
	return max;
}

- (void) setSize:(long) setSize
{
	size = setSize;
}

- (long) size
{
	return size;
}

- (void) setUsage:(long) setUsage
{
	usage = setUsage;
}

- (long) usage
{
	return usage;
}

- (void) setUsagePage:(long) setUsagePage
{
	usagePage = setUsagePage;
}

- (long) usagePage
{
	return usagePage;
}

- (void) setType:(long) setType
{
	type = setType;
}

- (long) type
{
	return type;
}

- (void) setCookie:(id) setCookie
{
	cookie = setCookie;
}

- (id) cookie
{
	return cookie;
}

- (void) setIsScaled:(BOOL) setIsScaled
{
	[[HidFinderData getData] setValue:[NSNumber numberWithBool:setIsScaled] ofKey:@"isScaled" from:self];
//	isScaled = setIsScaled;
}

- (BOOL) isScaled
{
	return [[[HidFinderData getData] getValueOfKey:@"isScaled" from:self] boolValue];
//	return isScaled;
}

- (void) setIsEnabled:(BOOL) setIsEnabled
{
	[[HidFinderData getData] setValue:[NSNumber numberWithBool:setIsEnabled] ofKey:@"isEnabled" from:self];
//	isEnabled = setIsEnabled;
}

- (BOOL) isEnabled
{
	return [[[HidFinderData getData] getValueOfKey:@"isEnabled" from:self] boolValue];
}

- (void) setString:(NSString*) setString
{
	[[HidFinderData getData] setValue:[NSString stringWithString:setString] ofKey:@"string" from:self];
}

- (NSString*) string
{
	return [[HidFinderData getData] getValueOfKey:@"string" from:self];
}

- (void) setDelegate:(id) setDelegate
{
	delegate = setDelegate;
}

- (void) setName:(NSString*) setName
{
	name = setName;
}

- (NSString*) name
{
	return name;
}

- (id) parent
{
	return parent;
}

@end
