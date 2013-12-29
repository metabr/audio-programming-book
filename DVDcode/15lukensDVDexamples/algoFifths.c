//algoFifths.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/portmidi.h"#include "include/porttime.h"

#define MD_NOTEON 0x90 //Hex for a note on message on channel 0#define SBYTE(mess,chan) mess | chan

#define INIT_VELOCITY 120

int main()
{
	int cnt, i;
	int dev;
	char chan = 0; //use channel 0    	long timeCount = 0;
	int waitTime, note, drop = 0, first;
	int scale[] = {60, 62, 67, 69, 72}; //user-defined scale
	
	const PmDeviceInfo *info;	PmError retval; //PortMIDI error (enum)	PortMidiStream *mstream; //PortMIDI stream (struct)
		
	Pm_Initialize(); //Init PortMIDI
	
	srand((unsigned)time(0));
	
	//Display available MIDI devices
	if(cnt = Pm_CountDevices())
	{
		for(i=0; i < cnt; i++)
		{
			info = Pm_GetDeviceInfo(i); //gets the device info
			if(info->output)  //this is 1 if the device is for output 
				printf("%d: %s: %s \n", i, info->name, info->interf); //prints the device number, name, and interface    	}	}
	else printf("ERROR: no device found\n");
	
	Pt_Start(1, NULL, NULL); //start timer
	
	printf("\nSelect your MIDI device number: ");
	scanf("%d", &dev);
	
	//open the device number dev	retval = Pm_OpenOutput(&mstream, dev, NULL,512,NULL,NULL, 0);
		//check if it is properly open, print a message if it is not	if(retval != pmNoError)    
	{
		printf("error: %s \n", Pm_GetErrorText(retval));
		goto cleanup;
	}
	printf("Successfully opened device %d: %s\n\n", dev, Pm_GetDeviceInfo(dev)->name);
	printf("Press ctrl-c to quit.\n\n");

	//Loop to output MIDI
	while(1)
	{		
		note = scale[rand() % 5]; //pick a note from the scale.	
		
		drop = 0;
		first = 1;
		waitTime = rand() % 150 + 50; //the time between notes in the sequence
		printf("Time between notes (ms): %d\n", waitTime);
		
		//this loop will play the selected note the first time through
		//if it is not the first time through, a fifth above the original
		//note will be played, with a drop in velocity to create
		//a diminuendo.
		for(i = 0; i < 16; i++)
		{
			timeCount = Pt_Time();
			
			//Turn note on.
			if(first) Pm_WriteShort(mstream, 0, Pm_Message(MD_NOTEON | chan, note, INIT_VELOCITY));
			else Pm_WriteShort(mstream, 0, Pm_Message(MD_NOTEON | chan, note + 7, INIT_VELOCITY - drop));
			
			while(Pt_Time() - timeCount < waitTime); //wait between notes
			
			//Turn note off using note on with velocity of 0 to take advantage of running status
			if(first) Pm_WriteShort(mstream, 0, Pm_Message(MD_NOTEON | chan, note, 0));
			else Pm_WriteShort(mstream, 0, Pm_Message(MD_NOTEON | chan, note + 7, 0));
			
			drop = i * 5; //diminuendo
			first = 0;
		}
		
		//pick a random time to wait before starting a new note sequence.
		waitTime = rand() % 3000+800;
		printf("Time before next sequence (ms): %d\n", waitTime);
		timeCount = Pt_Time();
		while(Pt_Time() - timeCount < waitTime);
	}
	//End Loop

cleanup:

	Pm_Close(mstream); //Close the stream	Pm_Terminate(); //Terminate
	
	return 0;
}