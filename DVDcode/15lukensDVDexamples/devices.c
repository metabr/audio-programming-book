//devices.c
#include <stdio.h>
#include "include/portmidi.h"#include "include/porttime.h"

int main()
{
	int cnt, i;
	const PmDeviceInfo *info;
	
	Pm_Initialize();
	
	if(cnt = Pm_CountDevices())	//return the number of devices
	{
		for(i=0; i < cnt; i++)
		{
			info = Pm_GetDeviceInfo(i); //gets device info
			if(info->output)  //gets device info
				printf("%d: %s \n", i, info->name);    	}	}
	else printf("ERROR: no device found\n");
	
	Pm_Terminate();
	
	return 0;
}