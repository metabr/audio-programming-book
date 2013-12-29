//devicesWithStream.c
#include <stdio.h>
#include "include/portmidi.h"#include "include/porttime.h"

int main()
{
	int cnt, i;
	const PmDeviceInfo *info;
	PortMidiStream *stream;
	
	Pm_Initialize();
	
	if(cnt = Pm_CountDevices()) //return the number of devices
	{
		for(i=0; i < cnt; i++)
		{
			info = Pm_GetDeviceInfo(i); //gets device info
			if(info->input)  //if input is 1 
				printf("%d: %s \n", i, info->name);    	}
    	
    	Pm_OpenInput(&stream, 0, NULL, 512, NULL, NULL);	}
	else printf("ERROR: no device found\n");
	
	Pm_Close(stream);
	Pm_Terminate();
	
	return 0;
}