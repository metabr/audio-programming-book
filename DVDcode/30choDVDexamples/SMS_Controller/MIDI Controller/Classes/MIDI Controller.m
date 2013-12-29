#import "MIDI Controller.h"

@implementation MIDI_Controller

- (id)init
{
	self = [super initWithWindowNibName:@"MIDI Controller"];
	if(self){
		records = [[NSMutableArray alloc] initWithCapacity:0];
		ref = [[NSMutableDictionary alloc] initWithCapacity:0];
		
	//Creates an output port through which the client may send
	//outgoing MIDI messages to any MIDI destination	
		MIDIClientRef client;
		MIDIClientCreate(CFSTR("MIDI Setup"), NULL, NULL, &client);
		MIDIOutputPortCreate(client, CFSTR("Output port"), &outPort);
	}
	else{
		[super dealloc];
		self = nil;
	}
	return self;
}

- (void)awakeFromNib
{	
	outPorts = [[NSMutableArray arrayWithCapacity:0] retain]; // create new NSMutableArray instance
	
	OSStatus osStatus;
	CFStringRef portName;
	int i;
	
//Get MIDI Output Ports of current system and put them to outPorts array
	
	int numberOfPorts =	MIDIGetNumberOfDestinations(); // .... (1)
	
	for (i = 0; i < numberOfPorts; i++) {
		MIDIEndpointRef outputPort = MIDIGetDestination(i); // .... (2)
		
		osStatus = 
			MIDIObjectGetStringProperty(outputPort, kMIDIPropertyName, &portName); // ... (3)
		
		if(osStatus) 
			fputs("outputPort: MIDIObjectGetStringProperty error \n", stderr);
		
		[outPorts addObject:(NSString *)portName];	// ..... (4)
		CFRelease(portName);						// ..... (5)
	}
	
	
	if(numberOfPorts == 0){
		NSBeep();
		NSRunAlertPanel(@"Close", @"Couldn't find any MIDI Out Device", 
						@"Close", nil, nil);
		[outPorts addObject:@"None"];				// ..... (6)
	}

// reference with the default values of each columns.
	
	[ref setObject:@"1" forKey:@"ch"];				// channel
	[ref setObject:@"Control" forKey:@"message"];	// MIDI message type
	[ref setObject:@"60" forKey:@"data"];			// control number, note number, etc
	[ref setObject:@"0" forKey:@"min"];				// minumum value of control
	[ref setObject:@"127" forKey:@"max"];			// maximum value of control
	[ref setObject:@"X" forKey:@"source"];			// control source
	[ref setObject:@"None" forKey:@"when"];			// message activate option
	[ref setObject:@"None" forKey:@"threshold"];	// threshold for when
	[ref setObject:[outPorts objectAtIndex:0] forKey:@"output"]; // MIDI output port
	[ref setObject:@"1" forKey:@"flag"];			// flag for Note On and Off message
}

/* data source for the NSComboBoxCell
 it reads the data from the representedObject
 the cell is responsible to display and manage the list of options
 (we set representedObject in tableView:willDisplayCell:forTableColumn:row:)
 this is optional, the alternative is to enter a list of values in interface builder */

-(id)comboBoxCell:(NSComboBoxCell*)cell objectValueForItemAtIndex:(NSInteger)index
{
	NSArray *values = [cell representedObject];
	return [values objectAtIndex:index];
}

-(NSInteger)numberOfItemsInComboBoxCell:(NSComboBoxCell*)cell
{
	NSArray *values = [cell representedObject];
	return [values count];
}

/* delegate for the NSTableView
 since there's only one combo box using dataSource, we need to populate it with the proper
 values for the line as set its selection, etc.
 this is optional, the alternative is to set a list of values in interface builder  */

-(void)tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)index
{	
	if([[tableColumn identifier] isEqual:@"output"] && [cell isKindOfClass:[NSComboBoxCell class]])
	{
		[cell setRepresentedObject:outPorts];
		[cell reloadData];
	}
	
	if([[tableColumn identifier] isEqual:@"min"] | [[tableColumn identifier] isEqual:@"threshold"] )
	{
		if ([[cell objectValue] isEqual:@"Velocity"] | [[cell objectValue] isEqual:@"None"] ){
			[cell setEditable:NO];
			[cell setTextColor:[NSColor grayColor]];
		}
		else{
			[cell setEditable:YES];
			[cell setTextColor:[NSColor blackColor]];
		}
	}
}

/* data source for the NSTableView
 the table is responsible to display and record the user selection
 (as opposed to the list of choices)
 this is required */

-(NSInteger)numberOfRowsInTableView:(NSTableView*)tableView
{
	return [records count];
}

-(id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)index
{
	return [[records objectAtIndex:index] objectForKey:[tableColumn identifier]];
}

-(void)tableView:(NSTableView*)tableView setObjectValue:(id)value forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)index
{
	NSString *iden = [tableColumn identifier];
	
	if([iden isEqual:@"ch"] | [iden isEqual:@"source"] | [iden isEqual:@"threshold"] | [iden isEqual:@"output"] ) // ... (1)
		[[records objectAtIndex:index] setObject:value forKey:iden];		
	
	if([iden isEqual:@"data"] | [iden isEqual:@"min"] | [iden isEqual:@"max"]){						// ..... (2)
		if(([value intValue] <= 0 && ![value isEqual:@"0"]) | [value intValue] > 127 )
			NSRunAlertPanel(@"Close", @"Wrong Value", @"Close", nil, nil);
		else
			[[records objectAtIndex:index] setObject:value forKey:iden];
	}
	
	if([iden isEqual:@"message"]){																	// ..... (3)
		if([value isEqual:@"Note On"] | [value isEqual:@"Note Off"]){
			[[records objectAtIndex:index] setObject:@"Velocity" forKey:@"min"];
			[[records objectAtIndex:index] setObject:@"100" forKey:@"max"];
		}
		else{
			[[records objectAtIndex:index] setObject:@"0" forKey:@"min"];
			[[records objectAtIndex:index] setObject:@"127" forKey:@"max"];
		}
		[[records objectAtIndex:index] setObject:value forKey:iden];
	}
	
	if([iden isEqual:@"when"]){																		// ..... (4)
		if([value isEqual:@"None"])
			[[records objectAtIndex:index] setObject:@"None" forKey:@"threshold"];
		else
			[[records objectAtIndex:index] setObject:@"0" forKey:@"threshold"];

		[[records objectAtIndex:index] setObject:value forKey:iden];
	}
	
	[tableView reloadData];																			// ..... (5)
}

- (IBAction)addRemove:(id)sender
{
	NSMutableDictionary *temp;
	
	switch([sender selectedSegment]){
		case 0:
			temp = [ref mutableCopy];	// ... (1)
			[records addObject:temp];
			[temp release];				// ... (2)
			[mTableView reloadData];
			break;
		case 1:
			if([mTableView selectedRow]>=0){
				[records removeObjectAtIndex:[mTableView selectedRow]];
				[mTableView reloadData];
			}
			break;
	}
}

- (void) sendMIDIData:(Byte *)data To:(int)indexOfOutPort	
{	
	OSStatus osStatus;
	MIDIPacketList pktlist;
	MIDIPacket *curPacket = MIDIPacketListInit( &pktlist ); 
	
	MIDIPacketListAdd(&pktlist, 
					  sizeof(MIDIPacketList), 
					  curPacket, 
					  0, 
					  3, 
					  data);
	
	osStatus = MIDISend(outPort, MIDIGetDestination(indexOfOutPort), &pktlist );
	
	if(osStatus) 
		fputs("outputPort: MIDISend error \n", stderr);
}

- (void)sendMIDIWithX:(int)x Y:(int)y Z:(int)z
{
	Byte data[3];	// 3 bytes MIDI message container
	NSString *message, *source, *when;
	int ch, min, max, threshold, indexOfOutPort;
	int xyz, source_range;	
	int i;
	
	for(i = 0; i < [records count]; i++){
		
		source_range = 513; // default control range. (-256 to 256)
		
	// read current row data from Table View	
		ch = [[[records objectAtIndex:i] objectForKey:@"ch"] intValue];
		message = [[records objectAtIndex:i] objectForKey:@"message"];
		data[1] = [[[records objectAtIndex:i] objectForKey:@"data"] intValue]; // first Data Byte of MIDI message
		min = [[[records objectAtIndex:i] objectForKey:@"min"] intValue];
		max = [[[records objectAtIndex:i] objectForKey:@"max"] intValue];
		source = [[records objectAtIndex:i] objectForKey:@"source"];
		when = [[records objectAtIndex:i] objectForKey:@"when"];
		threshold = [[[records objectAtIndex:i] objectForKey:@"threshold"] intValue];
		indexOfOutPort = [outPorts indexOfObject:[[records objectAtIndex:i] objectForKey:@"output"]];
	
	// data from Sudden Motion Sensor Controller
		if([source isEqual: @"X"]) xyz = x;
		if([source isEqual: @"Y"]) xyz = y;
		if([source isEqual: @"Z"]) xyz = z;
		
		if (xyz > 256) xyz = 256;	// to prevent error
		if (xyz < -256) xyz = -256; // to prevent error
	
	// message activate options	
		if([when isEqual:@"None"]){	
			[[records objectAtIndex:i] setObject:@"1" forKey:@"flag"];
			xyz = xyz + 256;
		}
		
		if([when isEqual:@"Over"]){
			if(threshold > xyz) 
			{	
				[[records objectAtIndex:i] setObject:@"0" forKey:@"flag"];
				continue;
			}
			else
				source_range = 256 - threshold; // range of threshold to 256
		}
		
		if([when isEqual:@"Under"]){
			if(threshold < xyz) 
			{
				[[records objectAtIndex:i] setObject:@"0" forKey:@"flag"];
				continue;
			}
			else
				source_range = 256 + threshold; // range of -256 to threshold
		}
	
	// generate Status Byte by user selected MIDI message type.	
		if([message isEqual: @"Note On"]){
			if ([[[records objectAtIndex:i] objectForKey:@"flag"] isEqual:@"1"])
				continue;
			data[0] = NOTE_ON + ch -1;		// Note On Status Byte
			[[records objectAtIndex:i] setObject:@"1" forKey:@"flag"];
			min = max;
		}
		
		if([message isEqual: @"Note Off"]){
			if ([[[records objectAtIndex:i] objectForKey:@"flag"] isEqual:@"1"])
				continue;
			data[0] = NOTE_OFF + ch -1;		// Note Off Status Byte
			[[records objectAtIndex:i] setObject:@"1" forKey:@"flag"];
			min = max;
		}
		
		if([message isEqual: @"Control"])
			data[0] = CONTROL_MSG + ch -1;		// Control Status Byte
		
		if([message isEqual: @"Pitch Bend"])
			data[0] = PITCHBEND_MSG + ch -1;	// Pitch Bend Status Byte
		
	// second Byte of Data Byte	
		data[2] =(int)abs(max-min)*abs(xyz-threshold)/source_range; // calculate the scale of control
		
		if(max<min)
			data[2] = min - data[2]; // if max is smaller than min reverse the scale.
		else
			data[2] = data[2] + min; // when min is smaller than max

		[self sendMIDIData:data To:indexOfOutPort];		// send MIDI data to Output port.
	}
}

-(void)dealloc
{
	printf("MIDI Controller Deallocating ... \n");
	
	[ref release];
	[records release];
	[outPorts release];
	[super dealloc];
}

@end
