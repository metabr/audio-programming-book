
//
//  HidDevice.m
//  HID Finder
//
//  Created by Andrew Beck on 12/27/07.
//

#import "HidDevice.h"


@implementation HidDevice

- (HidDevice*)initWithDevice:(io_object_t) setDevice properties:(NSDictionary*) props;
{
	self = [super init];
	
	isOpen = NO;
	
	[self setProperties:props];
	[self setIoHidDevice:setDevice];
	[self setName:[props objectForKey :(id) CFSTR(kIOHIDProductKey)]];

	[self createInterface];
	
	elementList = [[NSMutableArray alloc] init];
	[self buildElementList:[self properties]];	
	
	if ([elementList count] > 0)
		currentElement = [elementList objectAtIndex:0];

	NSString* propertiesDescription = [[NSString alloc] initWithString:[[self properties] description]];
	[self setRawDump:propertiesDescription];
	
	return self;
}

- (void) createInterface
{
    io_name_t               className;
    IOCFPlugInInterface     **plugInInterface = NULL;
    HRESULT                 plugInResult = S_OK;
    SInt32                  score = 0;
    IOReturn                ioReturnValue = kIOReturnSuccess;
	
	ioReturnValue = IOObjectGetClass([self ioHidDevice], className);

	if (ioReturnValue != kIOReturnSuccess)
		NSLog(@"Couldn't get class name!");
	
	if (className)
		NSLog(@"Found device type %s\n", className);	
	
	
	ioReturnValue = IOCreatePlugInInterfaceForService([self ioHidDevice],
						kIOHIDDeviceUserClientTypeID,
						kIOCFPlugInInterfaceID,
						&plugInInterface,
						&score);
						
	if (ioReturnValue == kIOReturnSuccess)
    {
        //Call a method of the intermediate plug-in to create the device
        //interface
        plugInResult = (*plugInInterface)->QueryInterface(plugInInterface,
                            CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID),
                            (LPVOID) &interface);
		if (plugInResult != S_OK)
			NSLog(@"Couldn't create HID class device interface"); 
		
		(*plugInInterface)->Release(plugInInterface);
    }	
}

- (void) buildElementList:(NSMutableDictionary*) ref
{
	id refElementTop = [ref valueForKey: (id) CFSTR(kIOHIDElementKey)];
    if (refElementTop)
	{
		if ( [refElementTop isKindOfClass:[NSArray class]] )
		{
			int i;
			for (i=0; i<[refElementTop count]; i++)
				if ([[refElementTop objectAtIndex:i] isKindOfClass:[NSDictionary class]])
					[self addElement:[refElementTop objectAtIndex:i]];
		}
	}
}

- (void) addElement:(NSMutableDictionary*) refElement
{
	HidElement* tempElement = NULL;
    long elementType, usagePage, usage;
    id refElementType = [refElement valueForKey: (id) CFSTR(kIOHIDElementTypeKey)];
    id refUsagePage = [refElement valueForKey: (id) CFSTR(kIOHIDElementUsagePageKey)];
    id refUsage = [refElement valueForKey: (id) CFSTR(kIOHIDElementUsageKey)];

    if (refElementType)
		elementType = [refElementType longValue];
	if (refUsagePage)
		usagePage = [refUsagePage longValue];
	if (refUsage)
		usage = [refUsage longValue];
		
    if (elementType)
    {
        // look at types of interest
        if (elementType != kIOHIDElementTypeCollection)
		{
            if (usagePage && usage) // if valid usage and page
			{
				tempElement = [[HidElement alloc] initWithParent:self];
				[tempElement setType:elementType];
				[tempElement setUsagePage:usagePage];
				[tempElement setUsage:usage];
				[tempElement setInfo:refElement];
				
				[elementList addObject:tempElement];
			}
		}
		else
			[self buildElementList:refElement];
	}
}

- (void) setProperties:(NSDictionary*) props
{
	properties = props;
}

- (NSDictionary*) properties
{
	return properties;
}

- (void) setIoHidDevice:(io_object_t) setDevice
{
	ioHidDevice = setDevice;
}

- (io_object_t) ioHidDevice
{
	return ioHidDevice;
}

- (void) setName:(NSString*) setName
{
	name = setName;
}

- (NSString*) name
{
	return name;
}

- (HidElement*) currentElement
{
	return currentElement;
}

- (IOHIDDeviceInterface**) interface
{
	return interface;
}

- (IOReturn) open
{
	IOReturn ioReturnValue = kIOReturnSuccess;
	ioReturnValue = (*interface)->open(interface, 0);
	
	if (ioReturnValue == kIOReturnSuccess)
		isOpen = YES;
	else 
		isOpen = NO;

	return ioReturnValue;
}

- (void) close
{
	if (isOpen)
		(*interface)->close(interface);
}

- (void) pollCurrentElement
{
	if (isOpen && currentElement)
		[currentElement updateValue:interface];
}

- (void) pollAllElements;
{
	if (isOpen && elementList)
	{
		int i;
		for (i=0; i<[elementList count]; i++)
			[[elementList objectAtIndex:i] updateValue:interface];
	}
}

- (void) setDelegate:(id) delegate
{
	if (elementList)
	{
		int i;
		for (i=0; i<[elementList count]; i++)
			[[elementList objectAtIndex:i] setDelegate:delegate];
	}
}

- (void) setCurrentElement:(HidElement*) setCurrentElement
{
	currentElement = setCurrentElement;
}

- (void) setRawDump:(NSString*) setRawDump
{
	rawDump = setRawDump;
}

- (NSString*) rawDump
{
	return rawDump;
}

@end
