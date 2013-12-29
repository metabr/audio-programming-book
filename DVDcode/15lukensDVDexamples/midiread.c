//midiread.c
#include <stdio.h>
#include <stdlib.h>
#include "include/portmidi.h"#include "include/porttime.h"

int main()
{
	int cnt, i;
	long status, data1, data2;
	const PmDeviceInfo *info;
	PortMidiStream *stream;
	PmEvent *event;
	
	event = (PmEvent *)malloc(sizeof(PmEvent)); //allocate memory for the event
	
	Pm_Initialize();
	
	if(cnt = Pm_CountDevices()) //return the number of devices
	{
		for(i=0; i < cnt; i++)
		{
			info = Pm_GetDeviceInfo(i); //gets device info
			if(info->input)  //if input is 1 
				printf("%d: %s \n", i, info->name);    	}
    	
    	Pm_OpenInput(&stream, 1, NULL, 512, NULL, NULL);	}
	else printf("ERROR: no device found\n");
	
	printf("Press ctrl-c to quit.\n\n");
	
	while(1)
	{
		if(Pm_Read(stream, event, sizeof(long)) > 0) //was there an event?
		{			
			//translate the message to longs
			status = Pm_MessageStatus(event->message);
			data1 = Pm_MessageData1(event->message);
			data2 = Pm_MessageData2(event->message);
			printf("Status: 0x%x, Data byte 1: %d, Data byte 2: %d\n", status, data1, data2);
		}
	}
			
	Pm_Close(stream);
	Pm_Terminate();
	
	return 0;
}