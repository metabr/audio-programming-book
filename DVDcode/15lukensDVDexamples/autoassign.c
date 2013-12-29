//autoassign.c
#include <stdio.h>
#include <stdlib.h>
#include "include/portmidi.h"#include "include/porttime.h"

int main()
{
	int i, cnt, go = 1, dev;
		
	const PmDeviceInfo *info;
	PmError retval;	PortMidiStream *mstream[32];		//PortMIDI stream (struct)
	PmEvent *event;						//PortMIDI event (struct)
	
	event = (PmEvent *)malloc(sizeof(PmEvent));   //allocate memory for the event
	
	Pm_Initialize();	//Init PortMIDI
	
	if (cnt = Pm_CountDevices())		//count the number of midi devices
	{
		for(i = 0; i < cnt; i++)		//loop through every midi device
		{
			info = Pm_GetDeviceInfo(i);
			if(info->input)				//is the device available for input?
				retval = Pm_OpenInput(&mstream[i], i, NULL, 512, NULL, NULL);	//open every midi input device
		}
	}
	
	printf("Play the MIDI device you wish to use\n");
	
	
	//main loop
	while(go)
	{
		for(i = 0; i < cnt; i++)	//loop through every midi device
		{
			if(Pm_Read(mstream[i], event, sizeof(long)) > 0)	//if we receive an event from any midi device at all then
				dev = i, go = 0;								//select it as our main input device
		}
	}
	//end loop
	
	printf("You have selected MIDI device %d: %s\n", dev, Pm_GetDeviceInfo(dev)->name);
	
	//close extra midi streams
	for(i = 0; i < cnt; i++)
	{
		if(i != dev)
			Pm_Close(mstream[i]);
	}
	
	//rest of program goes here.
	
	Pm_Close(mstream[dev]);
	Pm_Terminate();
	
	return 0;
}