//midioutput.c
#include <stdio.h>
#include <stdlib.h>
#include "include/portmidi.h"#include "include/porttime.h"

#define MD_NOTEON 0x90#define MD_NOTEOFF 0x80

int main()
{
	int cnt, i, time, totalOut, deviceNum = -1;
	int chan, note, velocity;
	const PmDeviceInfo *info;
	PmError retval;
	PortMidiStream *stream;
		
	Pm_Initialize();
	
	if(cnt = Pm_CountDevices()) //return the number of devices
	{
		for(i=0; i < cnt; i++)
		{
			info = Pm_GetDeviceInfo(i); //gets device info
			if(info->output)  //if output is 1 
			{
				printf("%d: %s \n", i, info->name);
				totalOut++;
			}    	}
    	
    	while(deviceNum < 0 || deviceNum > totalOut)
    	{
    		printf("Select an output device:\n");
    		scanf("%d", &deviceNum);
    	}
    	    	
    	retval = Pm_OpenOutput(&stream, deviceNum, NULL, 512, NULL, NULL, 0);
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
	
	Pt_Start(1, NULL, NULL); //start PortTimer
	
	for(i=0;i<20;i++)
	{
		//channel, note, and velocity values to pass to Pm_WriteShort
		chan = 0;
		note = 60;
		velocity = 120;
		
		//write a MIDI message
		Pm_WriteShort(stream, 0, Pm_Message(MD_NOTEON | chan, note, velocity));
				time = Pt_Time(NULL);		while(Pt_Time(NULL)  - time < 100); // 1/10 second
		
		Pm_WriteShort(stream, 0, Pm_Message(MD_NOTEOFF | chan, note, velocity));
	}
			
	Pm_Close(stream);
	Pm_Terminate();
	
	return 0;
}