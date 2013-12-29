#import "HidFinder.h"

@implementation HidFinder

+ (NSDictionary*) getUsageStrings {
	if (gUsageStrings)
		return gUsageStrings;
	else
	{
		NSString* usageTablesPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"usageTables" ofType:@"plist"];
		gUsageStrings = [[NSDictionary dictionaryWithContentsOfFile:usageTablesPath] retain];
		return gUsageStrings;
	}
}

- (id) init
{
	self = [super init];
		
	[self buildDeviceList];
	
	[self setDelegate:delegate];
	[self enableDevicePolling:YES withTimeInterval:0.05];

	return self;
}

- (void) buildDeviceList
{
	currentDevice = deviceList = NULL;

    IOReturn result = kIOReturnSuccess;
    mach_port_t masterPort = NULL;

    result = IOMasterPort (bootstrap_port, &masterPort);

	if (result == kIOReturnSuccess)
	{
		NSMutableDictionary* hidMatchDictionary = NULL;
			
		// Set up a matching dictionary to search I/O Registry by class name for all HID class devices.
		hidMatchDictionary = IOServiceMatching (kIOHIDDeviceKey);

		io_iterator_t hidObjectIterator = NULL;
		
		// Now search I/O Registry for matching devices.
		result = IOServiceGetMatchingServices (masterPort, hidMatchDictionary, &hidObjectIterator);
		if (result != kIOReturnSuccess)
			NSLog (@"Failed to create IO object iterator, error: %d", result);
		else if (hidObjectIterator == NULL) // likely no HID devices which matched selection criteria are connected
			NSLog (@"Warning: Could not find any matching devices, thus iterator creation failed.");

		deviceList = [[NSMutableArray alloc] init];

		if (hidObjectIterator != NULL)
		{
			io_object_t ioHIDDeviceObject = NULL;

			while (ioHIDDeviceObject = IOIteratorNext (hidObjectIterator))
			{
				NSMutableDictionary* hidProperties = 0;
				kern_return_t result = IORegistryEntryCreateCFProperties (ioHIDDeviceObject, &hidProperties, kCFAllocatorDefault, kNilOptions);
				
				HidDevice* device = [[HidDevice alloc] initWithDevice:ioHIDDeviceObject properties:hidProperties];

				[deviceList addObject:device];
				
				device = NULL;
			}

			result = IOObjectRelease (hidObjectIterator); // release the iterator
			if (kIOReturnSuccess != result)
				NSLog (@"IOObjectRelease error with hidObjectIterator.");
		}
	}
	
	if ([deviceList count] > 0)
		currentDevice = [deviceList objectAtIndex:0];
}

- (void) enableDevicePolling:(BOOL)toEnable withTimeInterval:(float)timeInterval
{
		if (toEnable && !pollTimer)
		{
			IOReturn ioReturnValue = NULL;
 
			//open the device
			int i;
			for (i=0; i<[deviceList count]; i++)
				if ( [(HidDevice*) [deviceList objectAtIndex:i] open] != kIOReturnSuccess )
					NSLog(@"Open result = %d\n", ioReturnValue);
				else
					ioReturnValue = kIOReturnSuccess;  // at least one device was successfully opened

			if (ioReturnValue == kIOReturnSuccess)
				pollTimer = [NSTimer scheduledTimerWithTimeInterval:timeInterval target:self selector:@selector(poll:) userInfo:nil repeats:YES];
		}
		else if (!toEnable && pollTimer)
		{
			[pollTimer invalidate];
			pollTimer = NULL;
			int i;
			for (i=0; i<[deviceList count]; i++)
				[[deviceList objectAtIndex:i] close];
		}
}

- (void) setDelegate:(id) delegate
{
	if (deviceList)
	{
		int i;
		for (i=0; i<[deviceList count]; i++)
			[[deviceList objectAtIndex:i] setDelegate:delegate];
	}
}

- (void) poll:(NSTimer*)theTimer
{
	if (deviceList)
	{
		int i;
		for (i=0; i<[deviceList count]; i++)
			[[deviceList objectAtIndex:i] pollAllElements];
	}
}

- (void) setCurrentDevice:(HidDevice*) setCurrentDevice
{
	currentDevice = setCurrentDevice;
}

- (HidDevice*) currentDevice
{
	return currentDevice;
}


// Delegated Method
- (void) elementUpdated:(HidElement*) element
{
	NSLog(@"Name: %@ Value: %f", [element name], [element value]);
}

- (IBAction) save:(id) sender
{
	NSSavePanel *sp;
	int runResult;
	sp = [NSSavePanel savePanel];
	runResult = [sp runModalForDirectory:NSHomeDirectory() file:@""];

	if (runResult == NSOKButton) {
		[[HidFinderData getData] save:[sp filename]];
	}
}

- (IBAction) load:(id) sender
{
	NSOpenPanel * panel = [NSOpenPanel openPanel];
	[panel beginSheetForDirectory:nil
                           file:nil
	                   types:nil
                  modalForWindow:[NSApp mainWindow]
                   modalDelegate:self
                  didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)
                     contextInfo:nil];
}

- (void) openPanelDidEnd:(NSOpenPanel*)panel returnCode:(int)rc contextInfo:(void *) ctx
{
	if (rc == NSOKButton) {
		[[HidFinderData getData] load:[panel filename]];
		[[currentDevice currentElement] refresh];
	}
}

- (IBAction) copyElement:(id) sender
{
	[[HidFinderData getData] copyElement:[currentDevice currentElement]];
}

- (IBAction) pasteElement:(id) sender
{
	[[HidFinderData getData] pasteElement:[currentDevice currentElement]];		
	[[currentDevice currentElement] refresh];
}

@end
