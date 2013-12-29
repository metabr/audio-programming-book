//buffer.c
#include <stdio.h>
#include <stdlib.h>
#include "include/portmidi.h"#include "include/porttime.h"

int main()
{
	int cnt, i, length, totalIn, deviceNum = -1;
	long status, data1, data2;
	const PmDeviceInfo *info;
	PmError retval;
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
			{
				printf("%d: %s \n", i, info->name);
				totalIn++;
			}    	}
    	
    	while(deviceNum < 0 || deviceNum > totalIn)
    	{
    		printf("Select an input device:\n");
    		scanf("%d", &deviceNum);
    	}
    	    	
    	retval = Pm_OpenInput(&stream, deviceNum, NULL, 512, NULL, NULL);
    	if(retval != pmNoError)
    	{
    		printf("ERROR: cannot open device number: %d", deviceNum);
    		return 1;
    	}	}
	else
	{
		printf("ERROR: no device found\n");
		return 1;
	}
	
	printf("Press ctrl-c to quit.\n\n");
	
	while(1)
	{
		if(Pm_Poll(stream))
		{			length = Pm_Read(stream, event, sizeof(long));						for(i=0;i<length;i++)			{				status = Pm_MessageStatus(event[i].message);				data1 = Pm_MessageData1(event[i].message);				data2 = Pm_MessageData2(event[i].message);				printf("Status: 0x%x, Data byte 1: %d, Data byte 2: %d\n", status, data1, data2);			}		}
	}
			
	Pm_Close(stream);
	Pm_Terminate();
	
	return 0;
}